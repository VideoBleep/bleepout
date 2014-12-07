//
//  RendererBase.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__RendererBase__
#define __bleepout__RendererBase__

#include "GameState.h"
#include "GameObjectCollection.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"

class RendererBase {
public:
  virtual ~RendererBase() {}

  virtual void draw(RoundState& state);

protected:
  virtual void drawBrick(RoundState& state, Brick& brick) = 0;
  virtual void drawPaddle(RoundState& state, Paddle& paddle) = 0;
  virtual void drawBall(RoundState& state, Ball& ball) = 0;
};

#endif /* defined(__bleepout__RendererBase__) */
