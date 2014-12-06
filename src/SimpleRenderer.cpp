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
  auto rect = brick.rect();
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(255, 0, 0);
  ofRect(rect.body->GetPosition().x, rect.body->GetPosition().y,
         rect.getWidth(), rect.getHeight());
}

void SimpleRenderer::drawPaddle(RoundController& round, Paddle &paddle) {
  auto rect = paddle.rect();
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofSetColor(0, 255, 0);
  ofRect(rect.body->GetPosition().x, rect.body->GetPosition().y,
         rect.getWidth(), rect.getHeight());
}

void SimpleRenderer::drawBall(RoundController& round, Ball &ball) {
  ball.circle().draw();
}