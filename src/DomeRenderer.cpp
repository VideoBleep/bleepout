//
//  DomeRenderer.cpp
//  bleepout
//
//  Created by Dewb on 12/8/14.
//
//

#include "DomeRenderer.h"


void DomeRenderer::draw(RoundState &round) {
    ofBackground(255, 255, 255);
    RendererBase::draw(round);
}

void DomeRenderer::drawBrick(RoundState& round, Brick &brick) {
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255, 0, 0);
    ofVec3f dims = brick.getSize();
    ofDrawBox(brick.getPosition(), dims.x, dims.y, dims.z);
}

void DomeRenderer::drawPaddle(RoundState& round, Paddle &paddle) {
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0, 255, 0);
    ofVec3f dims = paddle.getSize();
    ofDrawBox(paddle.getPosition(), dims.x, dims.y, dims.z);
}

void DomeRenderer::drawBall(RoundState& round, Ball &ball) {
    ofDrawSphere(ball.getPosition(), ball.getSize().x);
    
}