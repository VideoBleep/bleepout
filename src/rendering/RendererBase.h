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

class RoundConfig;
class RoundStateEventSource;
class RoundController;

class RendererBase {
public:
  virtual ~RendererBase() {}
    
  virtual void setup(RoundConfig& config) {}
  virtual void attachTo(RoundStateEventSource& roundEvents) {}
  virtual void detachFrom(RoundStateEventSource& roundEvents) {}

  virtual void update() {}
  virtual void draw(RoundState& state);

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
  virtual void drawModifier(RoundState& state, Modifier& modifier) = 0;
  virtual void drawAnimation(RoundState& state, AnimationObject& animation);
};

#endif /* defined(__bleepout__RendererBase__) */
