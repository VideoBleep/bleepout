 //
//  SimpleRenderer.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__SimpleRenderer__
#define __bleepout__SimpleRenderer__

#include "RendererBase.h"
#include "RoundManager.h"

class SimpleRenderer : public RendererBase {
public:
  virtual ~SimpleRenderer() {}
  virtual void draw(RoundController& round) override;
protected:
  virtual void drawBrick(RoundController& round, Brick& brick) override;
  virtual void drawPaddle(RoundController& round, Paddle& paddle) override;
  virtual void drawBall(RoundController& round, Ball& ball) override;
};

#endif /* defined(__bleepout__SimpleRenderer__) */
