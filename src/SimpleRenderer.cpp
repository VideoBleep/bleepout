//
//  SimpleRenderer.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "SimpleRenderer.h"

void SimpleRenderer::draw(RoundController &round) {
  ofBackground(255, 255, 255);
  RendererBase::draw(round);
}

void SimpleRenderer::drawBrick(RoundController& round, Brick &brick) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(255, 0, 0);
  ofRect(brick.getPosition().x, brick.getPosition().y,
         brick.getSize().x, brick.getSize().y);
}

void SimpleRenderer::drawPaddle(RoundController& round, Paddle &paddle) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(0, 255, 0);
  ofRect(paddle.getPosition().x, paddle.getPosition().y,
         paddle.getSize().x, paddle.getSize().y);
}

void SimpleRenderer::drawBall(RoundController& round, Ball &ball) {
  ofCircle(ball.getPosition().x, ball.getPosition().y, ball.getRadius());
}