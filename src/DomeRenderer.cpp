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
}

void DomeRenderer::draw(RoundState &state, RoundConfig& config) {
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
    }
    
    _cam.end();
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