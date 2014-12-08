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
#include <ofLog.h>

template<typename T>
class ObjectEventArgs {
public:
  ObjectEventArgs(T* obj) : _object(obj) { }
  
  T* object() { return _object; }
private:
  T* _object;
};

template<typename T>
class BallHitObjectEventArgs : public ObjectEventArgs<T> {
public:
  BallHitObjectEventArgs(Ball* ball, T* object)
  : _ball(ball), ObjectEventArgs<T>(object) { }

  Ball* ball() { return _ball; }
private:
  Ball* _ball;
};

typedef BallHitObjectEventArgs<Paddle> BallHitPaddleEventArgs;
typedef BallHitObjectEventArgs<Brick> BallHitBrickEventArgs;
typedef BallHitObjectEventArgs<Wall> BallHitWallEventArgs;
typedef BallHitObjectEventArgs<Ball> BallHitBallEventArgs;

class CollisionLogger;

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
  template<typename Listener>
  void removeListener(Listener& listener) {
    ofRemoveListener(ballHitPaddleEvent, &listener, &Listener::onBallHitPaddle);
    ofRemoveListener(ballHitBrickEvent, &listener, &Listener::onBallHitBrick);
    ofRemoveListener(ballHitWallEvent, &listener, &Listener::onBallHitWall);
    ofRemoveListener(ballHitBallEvent, &listener, &Listener::onBallHitBall);
  }
  
  void enableLogging(ofLogLevel level);
  void disableLogging();
protected:
  void notifyBallHitPaddle(Ball* ball, Paddle* paddle) {
    BallHitPaddleEventArgs e(ball, paddle);
    ofNotifyEvent(ballHitPaddleEvent, e);
  }
  void notifyBallHitBrick(Ball* ball, Brick* brick) {
    BallHitBrickEventArgs e(ball, brick);
    ofNotifyEvent(ballHitBrickEvent, e);
  }
  void notifyBallHitWall(Ball* ball, Wall* wall) {
    BallHitWallEventArgs e(ball, wall);
    ofNotifyEvent(ballHitWallEvent, e);
  }
  void notifyBallHitBall(Ball* ball, Ball* otherBall) {
    BallHitBallEventArgs e(ball, otherBall);
    ofNotifyEvent(ballHitBallEvent, e);
  }
private:
  ofPtr<CollisionLogger> _logger;
};

class BallOwnerChangedEventArgs {
public:
  BallOwnerChangedEventArgs(Ball* ball,
                            Player* player,
                            Player* previousPlayer)
  : _ball(ball), _player(player), _previousPlayer(previousPlayer) {}

  Ball* ball() { return _ball; }
  Player* player() { return _player; }
  Player* previousPlayer() { return _previousPlayer; }
private:
  Ball* _ball;
  Player* _player;
  Player* _previousPlayer;
};

class BrickDestroyedEventArgs {
public:
  BrickDestroyedEventArgs(Brick* brick, Ball* ball)
  : _brick(brick), _ball(ball) { }
  
  Brick* brick() { return _brick; }
  Ball* ball() { return _ball; }
private:
  Brick* _brick;
  Ball* _ball;
};

class RoundStateEventSource {
public:
  ofEvent<BallOwnerChangedEventArgs> ballOwnerChangedEvent;
  ofEvent<BrickDestroyedEventArgs> brickDestroyedEvent;
  ofEvent<ObjectEventArgs<Player> > playerScoreChangedEvent;
  ofEvent<ObjectEventArgs<Ball> > ballDestroyedEvent;
  ofEvent<ObjectEventArgs<Ball> > ballRespawnedEvent;
  ofEvent<ObjectEventArgs<Player> > playerLostEvent;
  ofEvent<ofEventArgs> roundEndedEvent;
  
protected:
  void notifyBallOwnerChanged(Ball* ball, Player* player, Player* previousPlayer) {
    BallOwnerChangedEventArgs e(ball, player, previousPlayer);
    ofNotifyEvent(ballOwnerChangedEvent, e);
  }
  void notifyBrickDestroyed(Brick* brick, Ball* ball) {
    BrickDestroyedEventArgs e(brick, ball);
    ofNotifyEvent(brickDestroyedEvent, e);
  }
  void notifyPlayerScoreChanged(Player* player) {
    ObjectEventArgs<Player> e(player);
    ofNotifyEvent(playerScoreChangedEvent, e);
  }
  void notifyBallDestroyed(Ball* ball) {
    ObjectEventArgs<Ball> e(ball);
    ofNotifyEvent(ballDestroyedEvent, e);
  }
  void notifyBallRespawned(Ball* ball) {
    ObjectEventArgs<Ball> e(ball);
    ofNotifyEvent(ballRespawnedEvent, e);
  }
  void notifyPlayerLost(Player* player) {
    ObjectEventArgs<Player> e(player);
    ofNotifyEvent(playerLostEvent, e);
  }
  void notifyRoundEnded() {
    ofEventArgs e;
    ofNotifyEvent(roundEndedEvent, e);
  }
};

#endif /* defined(__bleepout__GameEvents__) */
