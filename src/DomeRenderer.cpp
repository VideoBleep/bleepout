//
//  DomeRenderer.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "DomeRenderer.h"


void DomeRenderer::draw(RoundController &round) {
    ofBackground(255, 255, 255);
    RendererBase::draw(round);
}

void DomeRenderer::drawBrick(RoundController& round, Brick &brick) {
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255, 0, 0);
    ofVec3f dims = brick.getDimensions();
    ofDrawBox(brick.getPosition(), dims.x, dims.y, dims.z);
}

void DomeRenderer::drawPaddle(RoundController& round, Paddle &paddle) {
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0, 255, 0);
    ofVec3f dims = paddle.getDimensions();
    ofDrawBox(paddle.getPosition(), dims.x, dims.y, dims.z);
}

void DomeRenderer::drawBall(RoundController& round, Ball &ball) {
    ofDrawSphere(ball.getPosition(), ball.getRadius());
    
}