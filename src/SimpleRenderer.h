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
#include "GameState.h"

class SimpleRenderer : public RendererBase {
public:
  virtual ~SimpleRenderer() {}
  virtual void draw(GameState& state) override;
protected:
  virtual void drawBrick(GameState& state, Brick& brick) override;
  virtual void drawPaddle(GameState& state, Paddle& paddle) override;
  virtual void drawBall(GameState& state, Ball& ball) override;
};

#endif /* defined(__bleepout__SimpleRenderer__) */
