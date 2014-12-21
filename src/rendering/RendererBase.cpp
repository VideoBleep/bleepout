//
//  RendererBase.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "RendererBase.h"


void RendererBase::draw(RoundState &state) {
  for (auto& obj : state.bricks()) {
    if (obj && obj->visible())
      drawBrick(state, *obj);
  }
  for (auto& obj : state.paddles()) {
    if (obj && obj->visible())
      drawPaddle(state, *obj);
  }
  for (auto& obj : state.balls()) {
    if (obj && obj->visible())
      drawBall(state, *obj);
  }
  for (auto& obj : state.walls()) {
    if (obj && obj->visible())
      drawWall(state, *obj);
  }
  for (auto& obj : state.modifiers()) {
    if (obj && obj->visible())
      drawModifier(state, *obj);
  }
  for (auto& obj : state.animations()) {
    if (obj && obj->visible())
      drawAnimation(state, *obj);
  }
}

void RendererBase::drawAnimation(RoundState &state, AnimationObject &animation) {
  animation.draw(state.config());
}