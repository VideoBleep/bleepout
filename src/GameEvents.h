//
//  GameEvents.h
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#ifndef __bleepout__GameEvents__
#define __bleepout__GameEvents__

#include <ofTypes.h>
#include "GameObject.h"
#include "Player.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "Wall.h"

template<typename T>
class BallHitObjectEventArgs {
public:
  BallHitObjectEventArgs(Ball& ball, T& object)
  : _ball(ball), _object(object) { }

  Ball& ball() { return _ball; }
  T& object() { return _object; }
private:
  Ball& _ball;
  T& _object;
};

class PlayerEventArgs {
public:
  PlayerEventArgs(ofPtr<Player> player) : _player(player) { }
private:
  ofPtr<Player> _player;
};

typedef BallHitObjectEventArgs<Paddle> BallHitPaddleEventArgs;
typedef BallHitObjectEventArgs<Brick> BallHitBrickEventArgs;
typedef BallHitObjectEventArgs<Wall> BallHitWallEventArgs;
typedef BallHitObjectEventArgs<Ball> BallHitBallEventArgs;

//class CollisionEventListener {
//  virtual void onBallHitPaddle(BallHitPaddleEventArgs& e) = 0;
//  virtual void onBallHitBrick(BallHitBrickEventArgs& e) = 0;
//  virtual void onBallHitWall(BallHitWallEventArgs& e) = 0;
//  virtual void onBallHitBall(BallHitBallEventArgs& e) = 0;
//};

class CollisionLogger {
  void onBallHitPaddle(BallHitPaddleEventArgs& e);
  void onBallHitBrick(BallHitBrickEventArgs& e);
  void onBallHitWall(BallHitWallEventArgs& e);
  void onBallHitBall(BallHitBallEventArgs& e);
};

class CollisionEventSource {
public:
  ofEvent<BallHitPaddleEventArgs> ballHitPaddleEvent;
  ofEvent<BallHitBrickEventArgs> ballHitBrickEvent;
  ofEvent<BallHitWallEventArgs> ballHitWallEvent;
  ofEvent<BallHitBallEventArgs> ballHitBallEvent;
  
  template<typename Listener>
  void attachListener(Listener& listener) {
    ofAddListener(ballHitPaddleEvent, &listener, &Listener::onBallHitPaddle);
    ofAddListener(ballHitBrickEvent, &listener, &Listener::onBallHitBrick);
    ofAddListener(ballHitWallEvent, &listener, &Listener::onBallHitWall);
    ofAddListener(ballHitBallEvent, &listener, &Listener::onBallHitBall);
  }
protected:
  void notifyBallHitPaddle(Ball& ball, Paddle& paddle);
  void notifyBallHitBrick(Ball& ball, Brick& brick);
  void notifyBallHitWall(Ball& ball, Wall& wall);
  void notifyBallHitBall(Ball& ball, Ball& otherBall);
};

#endif /* defined(__bleepout__GameEvents__) */
