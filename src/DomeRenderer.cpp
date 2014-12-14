//
//  DomeRenderer.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "DomeRenderer.h"
#include "PhysicsUtil.h"

namespace {
    void drawBoxObject(PhysicsObject& object, ofColor edgeColor, ofColor fillColor) {
        ofPushMatrix();
        ofPushStyle();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofVec3f dims = object.getSize();
        ofTranslate(object.getPosition());
        ofRotateY(object.getRotation());
        ofNoFill();
        ofSetLineWidth(1.5);
        ofSetColor(edgeColor);
        ofDrawBox(ofVec3f::zero(), dims.x + 0.1, dims.y + 0.1, dims.z + 0.1);
        ofFill();
        ofSetColor(fillColor);
        ofDrawBox(ofVec3f::zero(), dims.x, dims.y, dims.z);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    void drawText(string text, ofColor color, ofTrueTypeFont& font, float size, float radius, float elevation, float heading) {
        ofPushMatrix();
        ofPushStyle();
        
        ofVec3f pos = sphericalToCartesian(radius, elevation, heading);
        ofTranslate(pos);
        ofRotateY(360 - heading - 90);
        ofRotateX(elevation);
        
        float scale = size/(font.getSize() * 1.0);
        ofScale(scale, scale);

        ofRectangle rect = font.getStringBoundingBox(text, 0, 0);
        ofTranslate(-rect.width/2, -rect.height/2, 0);
        
        ofSetColor(color);
        font.drawStringAsShapes(text, 0, 0);
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    template<typename T>
    void drawBoundingBoxes(GameObjectCollection<T> objects) {
        ofPushStyle();
        for (auto const &obj : objects) {
            if (!obj->alive()) {
                continue;
            }
            ofSetColor(255, 0, 0);
            ofNoFill();
            BoundingBox bbox = obj->getBoundingBox();
            ofDrawBox(bbox.center.x, bbox.center.y, bbox.center.z,
                      bbox.halfwidths.x * 2, bbox.halfwidths.y * 2, bbox.halfwidths.z * 2);
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
            if (t) {
                float r = t->getRadius();

                ofSetColor(color);
                
                ofPushMatrix();
                ofVec3f normal(0, 0, -1);
                ofVec3f orbitNormal = t->u().crossed(t->w()).normalized();
                ofVec3f axis = normal.crossed(orbitNormal);
                float angle = acos(normal.dot(orbitNormal));
                ofRotate(angle * 180/PI, axis.x, axis.y, axis.z);
                ofNoFill();
                ofCircle(0, 0, 0, r);
                ofPopMatrix();
                
                if (showVectors) {
                    ofFill();
                    ofDrawArrow(ofVec3f::zero(), t->u() * r, 2);
                    ofLine(ofVec3f::zero(), t->v() * r);
                    ofDrawArrow(ofVec3f::zero(), t->u().crossed(t->v()).normalize() * r/4.0, 2);
                }
            }
        }
        ofPopStyle();
    }
    
}


void DomeRenderer::setup() {
    ofEnableDepthTest();
    ofSetCircleResolution(64);
    _cam.setTarget(ofVec3f(0.0, 25.0, 0.0));
    _cam.setRotation(0.0, 0.66);
    _cam.setupPerspective(false);

    _debugGraphics = false;
    _drawTrajectories = false;
    
    _font.loadFont("PixelSplitter-Bold.ttf", 50, false, false, true);
    _extras.setup();
}

void DomeRenderer::draw(RoundState &state, RoundConfig& config) {
    
    _cam.setDistance(config.domeRadius() * 2.1);
    _cam.begin();

    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(80, 80, 110);
    ofNoFill();
    ofRotateX(90);
    ofCircle(0, 0, 0, config.domeRadius());

    ofPopStyle();
    ofPopMatrix();
    
    RendererBase::draw(state, config);
    
    if (_debugGraphics) {
        drawBoundingBoxes(state.balls());
        drawBoundingBoxes(state.paddles());
        drawBoundingBoxes(state.bricks());
        drawBoundingBoxes(state.walls());
        drawTrajectories(state.balls(), ofColor(255, 0, 0), true);
    } else if (_drawTrajectories) {
        drawTrajectories(state.balls(), ofColor(180, 180, 200), false);
    }
    
    if (state.message.text.length()) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < state.message.trails + 1; j++) {
                ofColor color = state.message.color;
                float h, s, b;
                color.getHsb(h, s, b);
                color.setHsb(h + j * 16, s, b + j * 32);
                drawText(state.message.text,
                         color,
                         _font,
                         state.message.size - (j * 1.5),
                         config.domeRadius() + config.domeMargin() * (1.25 + j * 0.1),
                         15 - (j * 1.1),
                         30 + i * 120);
            }
        }
    }
  
    _extras.draw(state, config);
  
    _cam.end();
    
    ofDrawBitmapString("command + mouse to rotate camera\ncommand + t to show trajectories\ncommand + d to show physics debugging info", 10, ofGetHeight() - 35);

}

void DomeRenderer::keyPressed(int key) {
    if (key == 'd') {
        _debugGraphics = !_debugGraphics;
    } else if (key == 't') {
        _drawTrajectories = !_drawTrajectories;
    }
}

void DomeRenderer::mousePressed(int x, int y, int button) {
    _cam.mousePressed(x, y, button);
}

void DomeRenderer::mouseReleased(int x, int y, int button) {
    _cam.mouseReleased(x, y, button);
}

void DomeRenderer::mouseDragged(int x, int y, int button) {
    _cam.mouseDragged(x, y, button);
}

void DomeRenderer::drawBrick(RoundState& round, Brick &brick) {
    drawBoxObject(brick, ofColor(255, 255, 255), brick.getColor());
}

void DomeRenderer::drawPaddle(RoundState& round, Paddle &paddle) {
    drawBoxObject(paddle, ofColor(0, 0, 0), paddle.getColor());
}

void DomeRenderer::drawWall(RoundState& round, Wall &wall) {
    drawBoxObject(wall, ofColor(80, 80, 80), ofColor(98, 98, 98));
}

void DomeRenderer::drawBall(RoundState& round, Ball &ball) {
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofTranslate(ball.getPosition());
    ofRotateX(360 * ball.getTrajectory()->getTime());
    ofRotateY(45);
    ofSetLineWidth(8.0);
    ofSetColor(ball.getColor());
    ofCircle(ofVec3f::zero(), ball.getSize().x / 2.0 + 0.05);
    ofFill();
    ofSetColor(255, 255, 255);
    ofDrawSphere(ofVec3f::zero(), ball.getSize().x / 2.0);

    ofPopStyle();
    ofPopMatrix();
}