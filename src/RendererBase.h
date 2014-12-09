//
//  RendererBase.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__RendererBase__
#define __bleepout__RendererBase__

#include "RoundManager.h"
#include "GameObjectCollection.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"

class RendererBase {
public:
  virtual ~RendererBase() {}
    
  virtual void init() {}

  virtual void draw(RoundController& round);

protected:
  virtual void drawBrick(RoundController& round, Brick& brick) = 0;
  virtual void drawPaddle(RoundController& round, Paddle& paddle) = 0;
  virtual void drawBall(RoundController& round, Ball& ball) = 0;
};

#endif /* defined(__bleepout__RendererBase__) */
