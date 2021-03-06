//
//  DomeRenderer.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "DomeRenderer.h"
#include "PhysicsUtil.h"
#include "OrbitalTrajectory.h"
#include "RendererUtil.h"
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"


namespace {
  
  template<typename T>
  void drawBoundingBoxes(GameObjectCollection<T> objects) {
    ofPushStyle();
    for (auto const &obj : objects) {
      if (!obj->alive()) {
        continue;
      }
      ofSetColor(255, 0, 0);
      ofNoFill();
      if (obj->getWorld()) {
        BoundingBox bbox = obj->getWorld()->getObjBoundingBox(obj.get());
        ofDrawBox(bbox.center.x, bbox.center.y, bbox.center.z,
                  bbox.halfwidths.x * 2, bbox.halfwidths.y * 2, bbox.halfwidths.z * 2);
      }
    }
    ofPopStyle();
  }
  
  template<typename T>
  void drawTrajectories(GameObjectCollection<T> objects, const ofColor& color, bool showVectors) {
    ofPushStyle();
    for (auto const &obj : objects) {
      if (!obj->alive()) {
        continue;
      }
      auto t = obj->getTrajectory();
      OrbitalTrajectory* ot = (OrbitalTrajectory*)t;
      if (ot) {
        float r = ot->getRadius();
        
        ofSetColor(color);
        
        ofPushMatrix();
        ofVec3f normal(0, 0, -1);
        ofVec3f orbitNormal = ot->u().crossed(ot->w()).normalized();
        ofVec3f axis = normal.crossed(orbitNormal);
        float angle = acos(normal.dot(orbitNormal));
        ofRotate(angle * 180/PI, axis.x, axis.y, axis.z);
        ofNoFill();
        ofCircle(0, 0, 0, r);
        ofPopMatrix();
        
        if (showVectors) {
          ofFill();
          ofDrawArrow(ofVec3f::zero(), ot->u() * r, 2);
          ofLine(ofVec3f::zero(), ot->v() * r);
          ofDrawArrow(ofVec3f::zero(), ot->u().crossed(ot->v()).normalize() * r/4.0, 2);
        }
      }
    }
    ofPopStyle();
  }
  
}

DomeRenderer::DomeRenderer(RoundState& state)
: RendererBase(state), _extras(state) { }

void DomeRenderer::setup() {
  ofEnableDepthTest();
  ofSetCircleResolution(64);
    
#ifndef RADOME
  _cam.setTarget(ofVec3f(0.0, 25.0, 0.0));
  _cam.setRotation(0.0, 0.66);
  _cam.setupPerspective(false);
#endif
  
  ofLight light;
  light.setDiffuseColor(ofColor(225, 225, 255));
  light.setSpecularColor(ofColor(220, 220, 255));
  light.setPointLight();
  movingLights.push_back(light);
  light.setDiffuseColor(ofColor(255, 225, 255));
  light.setSpecularColor(ofColor(188, 220, 255));
  light.setPointLight();
  movingLights.push_back(light);
  light.setDiffuseColor(ofColor(225, 255, 225));
  light.setSpecularColor(ofColor(235, 220, 188));
  light.setPointLight();
  movingLights.push_back(light);
  
  ofLight ambient;
  ambient.setAmbientColor(ofColor(70, 70, 70));
  staticLights.push_back(ambient);
  
  wallMaterial.setDiffuseColor(ofColor(80, 80, 90));
  wallMaterial.setAmbientColor(ofColor(98, 98, 118));
  wallMaterial.setEmissiveColor(ofColor(68, 60, 98));
  wallMaterial.setShininess(10.0);
  wallMaterial.setSpecularColor(ofColor(98, 98, 160, 255));
    
  ofSetSphereResolution(10);
  ofSetCylinderResolution(10, 4);
}

void DomeRenderer::setupExtras() {
  _extras.setup();
}

void DomeRenderer::update() {
  _extras.update();
}

void DomeRenderer::draw() {
  auto& appParams = BleepoutParameters::get();
    
#ifndef RADOME
  _cam.setDistance(appParams.domeRadius * 2.1);
  _cam.begin();
#endif
    
  float t = ofGetElapsedTimef() * 0.3;
  for (int i = 0; i < movingLights.size(); i++) {
    movingLights[i].setPosition(sphericalToCartesian(appParams.domeRadius * (0.25 + 0.85 * sin(t)), 25 + 15 * sin(t/2.0), i * 120 + 120 * cos(t/3.0)));

#ifdef RADOME
    movingLights[i].setAttenuation(0.008, 0.005, 0.0);
#else
    movingLights[i].setAttenuation(0.35, 0.25, 0.0);
#endif
    
    movingLights[i].enable();
  }
  for (int i = 0; i < staticLights.size(); i++) {
    staticLights[i].enable();
  }
  
  
#ifndef RADOME
  // draw circle on ground plane to represent dome extents
  ofPushMatrix();
  ofPushStyle();
  
  ofSetColor(80, 80, 110);
  ofNoFill();
  ofRotateX(90);
  LineWidthAdjuster::setLineWidth(1.5);
  ofCircle(0, 0, 0, appParams.domeRadius);
  
  ofPopStyle();
  ofPopMatrix();
#endif
  
  RendererBase::draw();
  
  if (appParams.debugGraphics) {
    drawBoundingBoxes(_state.balls());
    drawBoundingBoxes(_state.paddles());
    drawBoundingBoxes(_state.bricks());
    drawBoundingBoxes(_state.walls());
    drawBoundingBoxes(_state.modifiers());
    drawTrajectories(_state.balls(), ofColor(255, 0, 0, 100), true);
  } else if (appParams.drawTrajectories) {
    drawTrajectories(_state.balls(), ofColor(180, 180, 200, 180), false);
  }
  if (_state.hasConfig()) {
    for (auto& cw : _state.config().curvedWallSets()) {
      float r = appParams.domeRadius + appParams.domeMargin;
      float d = cw.width / 4.0;
      int steps = 20;
      
      Sweep sweep;
      sweep.startFace.addPoint(sphericalToCartesian(r, cw.elevation1, cw.heading1 - d));
      sweep.startFace.addPoint(sphericalToCartesian(r, cw.elevation1, cw.heading1 + d));
      sweep.startFace.addPoint(sphericalToCartesian(r + cw.width, cw.elevation1, cw.heading1 + d));
      sweep.startFace.addPoint(sphericalToCartesian(r + cw.width, cw.elevation1, cw.heading1 - d));
      sweep.path.addPoint(sphericalToCartesian(r, cw.elevation1, cw.heading1));
      for (int i = 0; i < steps; i++) {
        float s = i / ((steps - 1) * 1.0);
        sweep.path.addPoint(sphericalToCartesian(r,
                                                 lerp(cw.elevation1, cw.elevation2, s),
                                                 lerp(cw.heading1, cw.heading2, s)));
      }
      
      sweep.generate(false);
      drawGenMesh(sweep, wallMaterial, ofColor(80, 80, 90), 1.5);
    }
  }

  
  if (appParams.drawExtras) {
    ofDisableLighting();
    _extras.draw();
    ofEnableLighting();
  }

#ifndef RADOME
  _cam.end();
  
  ofDrawBitmapString("command + mouse to rotate camera\ncommand + T to show trajectories\ncommand + D to show physics debugging info\ncommand + L for laser mode\ncommand + C for comet mode\nE to toggle exits\nB to spawn new ball", 10, ofGetHeight() - 90);
#endif
  
}


void DomeRenderer::drawGenMesh(const GenMesh& gm, ofMaterial& mat, const ofColor& edgeColor, float lineWidth) {
  mat.begin();
  gm.mesh->draw();
  mat.end();
  
  ofSetColor(edgeColor);
  LineWidthAdjuster::setLineWidth(lineWidth);
    
#ifndef RADOME
  ofTranslate(_cam.getLookAtDir().normalized() * -0.2);
#else
  ofTranslate(gm.mesh->getVertex(0).normalized() * -0.2);
#endif
  gm.outline->draw();
}

#ifndef RADOME

void DomeRenderer::mousePressed(int x, int y, int button) {
  _cam.mousePressed(x, y, button);
}

void DomeRenderer::mouseReleased(int x, int y, int button) {
  _cam.mouseReleased(x, y, button);
}

void DomeRenderer::mouseDragged(int x, int y, int button) {
  _cam.mouseDragged(x, y, button);
}

#endif

void DomeRenderer::drawBrick(Brick &brick) {
  ofColor edgeColor = ofColor::white;
  ofColor fillColor = brick.getColor();
  float lineWidth = 1.5f;
  bool alphaBlending = false;
  if (brick.maxLives() > 1) {
    fillColor.a = ofMap(brick.lives(), 1, brick.maxLives(),
                        63, 255);
    edgeColor = ofColor((unsigned char)ofMap(brick.lives(),
                                             1, brick.maxLives(),
                                             255, 128));
    edgeColor.a = fillColor.a;
    alphaBlending = true;
    float lineWidth = ofMap(brick.lives(), 1, 4,
                            3.0f, 15.0f, true);
  }
  drawBoxObject(brick, edgeColor, fillColor, NULL, lineWidth, alphaBlending);
}

void DomeRenderer::drawPaddle(Paddle &paddle) {
  ofColor color = paddle.getColor();
  if (paddle.player().modifierCounts()[MODIFIER_SUPERCHARGED_BALL] > 0) {
    color = ofColor(255, 255 * sin(_state.time * 7.5), 0);
  }
  drawBoxObject(paddle, ofColor(0, 0, 0), color);
}

void DomeRenderer::drawWall(Wall &wall) {
  drawBoxObject(wall, ofColor(80, 80, 90), ofColor(98, 98, 118), &wallMaterial, 1.5);
}

void drawCometTail(Ball& ball, float width, float length, int order, const ofColor& color) {
  ofVec3f pos = ball.getPosition();
  ofDisableLighting();
  ofSetColor(color);
  
  float s = 0.03;
  ofVec3f vel = ball.getVelocity().normalized();
  ofVec3f perpVec = vel.crossed(pos).normalized();
  ofVec3f jitter = perpVec * ofRandom(-s, s) + vel.normalized() * ofRandom(-s, s);
  vel += jitter;
  
  ofVec3f stack = pos.normalized() * 0.5 * order;
  ofVec3f tailPt = pos - vel * length + stack;
  ofVec3f headPt = pos + vel * 2.2 * width + stack;
  ofVec3f offsetVec = perpVec * 1.2 * width;
  ofVec3f topPt = pos + offsetVec + stack;
  ofVec3f botPt = pos - offsetVec + stack;
  ofBeginShape();
  ofSetCurveResolution(5);
  ofCurveVertex(tailPt);
  ofCurveVertex(tailPt);
  ofCurveVertex(topPt);
  ofCurveVertex(headPt);
  ofCurveVertex(botPt);
  ofCurveVertex(tailPt);
  ofEndShape();
  
  ofEnableLighting();
}

void DomeRenderer::drawBall(Ball &ball) {
  auto& appParams = BleepoutParameters::get();
  
  if (!appParams.allLasers) {
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(ball.getPosition());
    ofRotateX(360 * ball.getTrajectory()->getTime());
    ofRotateY(45);
    LineWidthAdjuster::setLineWidth(8.0);
    if (ball.player() != NULL) {
      ofFill();
      ofSetColor(ball.getColor());
      ofSetCylinderResolution(16, 1);
      ofDrawCylinder(ofVec3f::zero(), ball.getSize().x * 0.5 + 0.308, ball.getSize().x * 0.315);
    }
    ofFill();
    ofSetColor(255, 255, 255);
    ofDrawSphere(ofVec3f::zero(), ball.getSize().x / 2.0);
    
    ofPopMatrix();
    
    if (appParams.drawComets || ball.isSupercharged()) {
      drawCometTail(ball, 6.8, 50,  0, ofColor(255, 120, 30, 200));
      drawCometTail(ball, 5.0, 30, -1, ofColor(255, 200, 50, 200));
      drawCometTail(ball, 5.0, 30,  1, ofColor(255, 200, 50, 200));
      drawCometTail(ball, 3.0, 18,  2, ofColor(255, 255, 90, 200));
      drawCometTail(ball, 3.0, 18, -2, ofColor(255, 255, 90, 200));
    }
    
    ofPopStyle();
    
  } else {
    OrbitalTrajectory* ot = (OrbitalTrajectory*)ball.getTrajectory();
    if (ot) {
      ofPushStyle();
      ofPushMatrix();
      ofDisableLighting();
      
      ofEnableBlendMode(OF_BLENDMODE_ADD);
      
      ofSetColor(255, 255, 255, 255);
      glBegin(GL_QUAD_STRIP);
      ot->history.emitQuadStrip(1.2, -0.5);
      glEnd();
      
      ofColor c = ball.getColor();
      //c.a = 172;
      ofSetColor(c);
      glBegin(GL_QUAD_STRIP);
      ot->history.emitQuadStrip(3.0, 0.0);
      glEnd();
      
      ofEnableBlendMode(OF_BLENDMODE_ALPHA);
      
      c *= 0.5;
      ofSetColor(c);
      glBegin(GL_QUAD_STRIP);
      ot->history.emitQuadStrip(7.0, 0.5);
      glEnd();
      
      ofEnableLighting();
      ofPopMatrix();
      ofPopStyle();
      
    }
    
  }
  
}

void DomeRenderer::drawModifier(Modifier &modifier) {
  ofPushStyle();
  ofPushMatrix();
  
  ofTranslate(modifier.getPosition());
  ofRotateX(360 * modifier.getTrajectory()->getTime());
  ofRotateY(45);

  if (modifier.spec().beneficial) {
    float length = modifier.getSize().y;
    float r = modifier.getSize().x * 0.5;
    float d = r * 1.25;
    ofFill();
    ofSetColor(200, 200, 200);
    ofDrawCylinder(ofVec3f::zero(), r * 0.8, length * 0.9);
    ofSetColor(modifier.spec().color);
    ofDrawCylinder(ofVec3f(0, (length - d) * -0.5, 0), r, d);
    ofDrawCylinder(ofVec3f(0, (length - d) *  0.5, 0), r, d);
  } else {
    float r = (modifier.getSize().x + modifier.getSize().y) * 0.25;
    ofDisableLighting();
    ofFill();
    ofSetColor(40, 40, 50);
    drawTetrahedron(ofVec3f::zero(), r);
    drawTetrahedron(ofVec3f::zero(), -r);
    ofNoFill();
    LineWidthAdjuster::setLineWidth(2.5);
    ofSetColor(modifier.spec().color);
    drawTetrahedron(ofVec3f::zero(), r);
    drawTetrahedron(ofVec3f::zero(), -r);
    ofEnableLighting();
  }
  
  ofPopMatrix();
  ofPopStyle();
}