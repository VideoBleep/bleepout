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
#include "BleepoutConfig.h"

class RoundController;

class RendererBase {
public:
  virtual ~RendererBase() {}
    
  virtual void setup(RoundController& roundController) {}

  virtual void update() {}
  virtual void draw(RoundState& state, RoundConfig& config);

  virtual void keyPressed(int key) {}
  virtual void mousePressed(int x, int y, int button) {}
  virtual void mouseReleased(int x, int y, int button) {}
  virtual void mouseDragged(int x, int y, int button) {}
  virtual void mouseMoved(int x, int y) {}

protected:
  virtual void drawBrick(RoundState& state, Brick& brick) = 0;
  virtual void drawPaddle(RoundState& state, Paddle& paddle) = 0;
  virtual void drawBall(RoundState& state, Ball& ball) = 0;
  virtual void drawWall(RoundState& state, Wall& wall) = 0;
};

#endif /* defined(__bleepout__RendererBase__) */
