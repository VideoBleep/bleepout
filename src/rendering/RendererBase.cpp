//
//  RendererBase.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "RendererBase.h"
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

RendererBase::RendererBase(RoundState& state,
                           const RoundConfig& config,
                           const BleepoutParameters& appParams)
: RoundComponent(state, config, appParams) { }

void RendererBase::draw() {
  for (auto& obj : _state.bricks()) {
    if (obj && obj->visible())
      drawBrick(*obj);
  }
  for (auto& obj : _state.paddles()) {
    if (obj && obj->visible())
      drawPaddle(*obj);
  }
  for (auto& obj : _state.balls()) {
    if (obj && obj->visible())
      drawBall(*obj);
  }
  for (auto& obj : _state.walls()) {
    if (obj && obj->visible())
      drawWall(*obj);
  }
  for (auto& obj : _state.modifiers()) {
    if (obj && obj->visible())
      drawModifier(*obj);
  }
  for (auto& obj : _state.animations()) {
    if (obj && obj->visible())
      drawAnimation(*obj);
  }
}

void RendererBase::drawAnimation(AnimationObject &animation) {
  animation.draw();
}