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
    
  _extras.setup();
  
  ofLight light;
  light.setDiffuseColor(ofColor(225, 225, 255));
  light.setSpecularColor(ofColor(220, 220, 255));
  light.setPointLight();
  lights.push_back(light);
  light.setDiffuseColor(ofColor(255, 225, 255));
  light.setSpecularColor(ofColor(188, 220, 255));
  light.setPointLight();
  lights.push_back(light);
  light.setDiffuseColor(ofColor(225, 255, 225));
  light.setSpecularColor(ofColor(235, 220, 188));
  light.setPointLight();
  lights.push_back(light);
  
  wallMaterial.setDiffuseColor(ofColor(80, 80, 90));
  wallMaterial.setAmbientColor(ofColor(98, 98, 118));
  wallMaterial.setEmissiveColor(ofColor(68, 60, 98));
  wallMaterial.setShininess(10.0);
  wallMaterial.setSpecularColor(ofColor(98, 98, 160, 255));
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
  for (int i = 0; i < lights.size(); i++) {
    lights[i].setPosition(sphericalToCartesian(appParams.domeRadius * (0.25 + 0.85 * sin(t)), 25 + 15 * sin(t/2.0), i * 120 + 120 * cos(t/3.0)));
    lights[i].setAttenuation(0.25, 0.007, 0.0);
    lights[i].enable();
  }
  
  ofPushMatrix();
  ofPushStyle();
  
  ofSetColor(80, 80, 110);
  ofNoFill();
  ofRotateX(90);
  ofSetLineWidth(1.5);
  ofCircle(0, 0, 0, appParams.domeRadius);
  
  ofPopStyle();
  ofPopMatrix();
  
  RendererBase::draw();
  
  if (appParams.debugGraphics) {
    drawBoundingBoxes(_state.balls());
    drawBoundingBoxes(_state.paddles());
    drawBoundingBoxes(_state.bricks());
    drawBoundingBoxes(_state.walls());
    drawTrajectories(_state.balls(), ofColor(255, 0, 0, 100), true);
  } else if (appParams.drawTrajectories) {
    drawTrajectories(_state.balls(), ofColor(180, 180, 200, 180), false);
  }
  
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
    
    sweep.generate();
    drawGenMesh(sweep, wallMaterial, ofColor(80, 80, 90), 1.5);
  }
  
  for (int i = 0; i < lights.size(); i++) {
    lights[i].setAttenuation(0,0,0);
  }
  
  if (appParams.drawExtras)
    _extras.draw();

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
  ofSetLineWidth(lineWidth);
    
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
  drawBoxObject(paddle, ofColor(0, 0, 0), paddle.getColor());
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
  
  ofVec3f stack = pos.normalized() * 0.01 * order;
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
  
  if (!appParams.allLasers && !ball.isLaser()) {
    
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(ball.getPosition());
    ofRotateX(360 * ball.getTrajectory()->getTime());
    ofRotateY(45);
    ofSetLineWidth(8.0);
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
    
    if (appParams.drawComets) {
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
      
      ofEnableBlendMode(OF_BLENDMODE_ADD);
      
      ofSetColor(255, 255, 255, 255);
      ofSetLineWidth(1.5);
      glBegin(GL_LINE_STRIP);
      ot->history.emitPoints();
      glEnd();
      
      ofColor c = ball.getColor();
      c.a = 172;
      ofSetColor(c);
      ofSetLineWidth(5.0);
      glBegin(GL_LINE_STRIP);
      ot->history.emitPoints();
      glEnd();
      
      ofEnableBlendMode(OF_BLENDMODE_ALPHA);
      
      c.a = 127;
      ofSetColor(c);
      ofSetLineWidth(20.0);
      glBegin(GL_LINE_STRIP);
      ot->history.emitPoints();
      glEnd();
      
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
  ofSetLineWidth(8.0);
  ofFill();
  ofSetColor(modifier.spec().color);
  ofDrawCylinder(ofVec3f::zero(), modifier.getSize().x / 2.0, modifier.getSize().y * 2.0);
  
  ofPopMatrix();
  
  ofPopStyle();
}