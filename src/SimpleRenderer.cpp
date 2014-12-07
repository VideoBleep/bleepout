//
//  SimpleRenderer.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "SimpleRenderer.h"

void SimpleRenderer::draw(RoundState &state) {
  ofBackground(255, 255, 255);
  RendererBase::draw(state);
}

void SimpleRenderer::drawBrick(RoundState& state, Brick &brick) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(255, 0, 0);
  ofRect(brick.body->GetPosition().x, brick.body->GetPosition().y,
         brick.getWidth(), brick.getHeight());
}

void SimpleRenderer::drawPaddle(RoundState& state, Paddle &paddle) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(0, 255, 0);
  ofRect(paddle.getPosition().x, paddle.getPosition().y,
         paddle.getWidth(), paddle.getHeight());
}

void SimpleRenderer::drawBall(RoundState& state, Ball &ball) {
  ball.draw();
}