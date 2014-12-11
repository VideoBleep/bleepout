//
//  DomeRenderer.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "DomeRenderer.h"

void DomeRenderer::setup() {
    _cam.setTarget(ofVec3f(0.0, 25.0, 0.0));
    _cam.setRotation(0.66, 0.5);
    _cam.setupPerspective(false);
    ofEnableDepthTest();
    ofSetCircleResolution(64);
    _debugGraphics = false;
}

void DomeRenderer::draw(RoundState &state, RoundConfig& config) {

    ofSetColor(80, 80, 80);
    ofDrawBitmapString("command + mouse to rotate camera\ncommand + d for physics debugging info", 10, 20);
    
    _cam.setDistance(config.domeRadius() * 2);
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
        for (auto const &obj : state.paddles()) {
            ofSetColor(255, 0, 0);
            ofNoFill();
            BoundingBox bbox = obj->getBoundingBox();
            ofDrawBox(bbox.center.x, bbox.center.y, bbox.center.z,
                      bbox.halfwidths.x * 2, bbox.halfwidths.y * 2, bbox.halfwidths.z * 2);
        }
        
        for (auto const &obj : state.balls()) {
            ofSetColor(255, 0, 0);
            ofNoFill();
            BoundingBox bbox = obj->getBoundingBox();
            ofDrawBox(bbox.center.x, bbox.center.y, bbox.center.z,
                      bbox.halfwidths.x * 2, bbox.halfwidths.y * 2, bbox.halfwidths.z * 2);
            
            auto t = obj->getTrajectory();
            float r = t->getRadius();

            ofPushMatrix();
            ofVec3f normal(0, 0, -1);
            ofVec3f orbitNormal = t->u().crossed(t->w()).normalized();
            ofVec3f axis = normal.crossed(orbitNormal);
            float angle = acos(normal.dot(orbitNormal));
            ofRotate(angle * 180/PI, axis.x, axis.y, axis.z);
            ofCircle(0, 0, 0, r);
            ofPopMatrix();
            
            ofFill();
            ofDrawArrow(ofVec3f::zero(), t->u() * r, 2);
            ofLine(ofVec3f::zero(), t->v() * r);
        }
    }
    
    _cam.end();
}

void DomeRenderer::keyPressed(int key) {
    if (key == 'd') {
        _debugGraphics = !_debugGraphics;
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
    ofPushMatrix();
    ofPushStyle();
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofVec3f dims = brick.getSize();
    ofSetColor(255, 0, 0);
    ofDrawBox(brick.getPosition(), dims.x, dims.y, dims.z);

    ofPopStyle();
    ofPopMatrix();
}

void DomeRenderer::drawPaddle(RoundState& round, Paddle &paddle) {
    ofPushMatrix();
    ofPushStyle();
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofVec3f dims = paddle.getSize();
    ofTranslate(paddle.getPosition());
    ofRotateY(paddle.getRotation() * 180/PI);
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofDrawBox(ofVec3f::zero(), dims.x + 0.1, dims.y + 0.1, dims.z + 0.1);
    ofFill();
    ofSetColor(0, 200, 60);
    ofDrawBox(ofVec3f::zero(), dims.x, dims.y, dims.z);
    
    ofPopStyle();
    ofPopMatrix();
}

void DomeRenderer::drawBall(RoundState& round, Ball &ball) {
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofTranslate(ball.getPosition());
    ofRotateX(45);
    ofCircle(ofVec3f::zero(), ball.getSize().x / 2.0 + 0.1);
    ofFill();
    ofSetColor(255, 255, 255);
    ofDrawSphere(ofVec3f::zero(), ball.getSize().x / 2.0);

    ofPopStyle();
    ofPopMatrix();
}