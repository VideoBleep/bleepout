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
  ofRect(brick.body->GetPosition().x, brick.body->GetPosition().y,
         brick.getWidth(), brick.getHeight());
}

void SimpleRenderer::drawPaddle(RoundController& round, Paddle &paddle) {
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(0, 255, 0);
  ofRect(paddle.body->GetPosition().x, paddle.body->GetPosition().y,
         paddle.getWidth(), paddle.getHeight());
}

void SimpleRenderer::drawBall(RoundController& round, Ball &ball) {
  ball.draw();
}