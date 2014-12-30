//
//  RendererBase.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__RendererBase__
#define __bleepout__RendererBase__

#include "RoundComponent.h"
#include "GameState.h"

class LogicController;

class RendererBase : public RoundComponent {
public:
  RendererBase(RoundState& state);
  virtual ~RendererBase() {}
  
  virtual void setup() {}
  virtual void attachTo(LogicController& roundEvents) {}
  virtual void detachFrom(LogicController& roundEvents) {}
  
  virtual void update() {}
  virtual void draw();
  
  virtual void keyPressed(int key) {}
  virtual void mousePressed(int x, int y, int button) {}
  virtual void mouseReleased(int x, int y, int button) {}
  virtual void mouseDragged(int x, int y, int button) {}
  virtual void mouseMoved(int x, int y) {}
  
protected:
  virtual void drawBrick(Brick& brick) = 0;
  virtual void drawPaddle(Paddle& paddle) = 0;
  virtual void drawBall(Ball& ball) = 0;
  virtual void drawWall(Wall& wall) = 0;
  virtual void drawModifier(Modifier& modifier) = 0;
  virtual void drawAnimation(AnimationObject& animation);
};

#endif /* defined(__bleepout__RendererBase__) */
