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
  brick.rect().draw();
}

void SimpleRenderer::drawPaddle(RoundController& round, Paddle &paddle) {
  paddle.rect().draw();
}

void SimpleRenderer::drawBall(RoundController& round, Ball &ball) {
  ball.circle().draw();
}