//
//  SimpleRenderer.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "SimpleRenderer.h"

void SimpleRenderer::draw(RoundState &state, RoundConfig& config) {
  //ofBackground(255, 255, 255);
  RendererBase::draw(state, config);
}

void SimpleRenderer::drawBrick(RoundState& state, Brick &brick) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(255, 0, 0);
  ofRect(brick.getPosition().x, brick.getPosition().y,
         brick.getSize().x, brick.getSize().y);
}

void SimpleRenderer::drawPaddle(RoundState& state, Paddle &paddle) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(0, 255, 0);
  ofRect(paddle.getPosition().x, paddle.getPosition().y,
         paddle.getSize().x, paddle.getSize().y);
}

void SimpleRenderer::drawBall(RoundState& state, Ball &ball) {
  ofCircle(ball.getPosition().x, ball.getPosition().y, ball.getSize().x);
}