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
#include <ofLog.h>
#include <iostream>
#include "GameObject.h"
#include "Player.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "Wall.h"
#include "GameState.h"
#include "Common.h"

template<typename T>
void outputField(std::ostream& os, const char* label, const T* obj) {
  os << label << ": ";
  if (obj)
    obj->output(os);
  else
    os << "(null)";
}

template<typename T>
void outputField(std::ostream& os, const T* obj) {
  outputField(os, T::typeName(), obj);
}

template<typename T>
class ObjectEventArgs : public Outputable {
public:
  explicit ObjectEventArgs(T* obj) : _object(obj) { }
  
  T* object() { return _object; }
  const T* object() const { return _object; }
  
  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, object());
    os << ")";
  }
private:
  T* _object;
};

template<typename T>
class BallHitObjectEventArgs : public ObjectEventArgs<T> {
public:
  BallHitObjectEventArgs(Ball* ball, T* object)
  : _ball(ball), ObjectEventArgs<T>(object) { }
  
  Ball* ball() { return _ball; }
  const Ball* ball() const { return _ball; }
  
  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, ball());
    os << " ";
    outputField(os, ObjectEventArgs<T>::object());
    os << ")";
  }
private:
  Ball* _ball;
};

typedef BallHitObjectEventArgs<Paddle> BallHitPaddleEventArgs;
typedef BallHitObjectEventArgs<Brick> BallHitBrickEventArgs;
typedef BallHitObjectEventArgs<Wall> BallHitWallEventArgs;
typedef BallHitObjectEventArgs<Ball> BallHitBallEventArgs;

class EventSource {
public:
  void enableLogging(ofLogLevel level) { _logLevel = level; }
  void disableLogging() { _logLevel = OF_LOG_SILENT; }
  bool loggingEnabled() const {
    return _logLevel != OF_LOG_SILENT;
  }
protected:
  void logEvent(const char* name, const Outputable& event) const {
    if (loggingEnabled()) {
      ofLog(_logLevel) << "EVENT{" << name << "}: " << event;
    }
  }
private:
  ofLogLevel _logLevel = OF_LOG_SILENT;
};

class CollisionEventSource : public EventSource {
public:
  ofEvent<BallHitPaddleEventArgs> ballHitPaddleEvent;
  ofEvent<BallHitBrickEventArgs> ballHitBrickEvent;
  ofEvent<BallHitWallEventArgs> ballHitWallEvent;
  ofEvent<BallHitBallEventArgs> ballHitBallEvent;
  
  template<typename Listener>
  void attachListener(Listener& listener) {
    ofAddListener(ballHitPaddleEvent, &listener, &Listener::onBallHitPaddle);
    ofAddListener(ballHitBrickEvent, &listener, &Listener::onBallHitBrick);
    ofAddListener(ballHitBallEvent, &listener, &Listener::onBallHitBall);
    ofAddListener(ballHitWallEvent, &listener, &Listener::onBallHitWall);
  }
  template<typename Listener>
  void detachListener(Listener& listener) {
    ofRemoveListener(ballHitPaddleEvent, &listener, &Listener::onBallHitPaddle);
    ofRemoveListener(ballHitBrickEvent, &listener, &Listener::onBallHitBrick);
    ofRemoveListener(ballHitBallEvent, &listener, &Listener::onBallHitBall);
    ofRemoveListener(ballHitWallEvent, &listener, &Listener::onBallHitWall);
  }
  
protected:
  void notifyBallHitPaddle(Ball* ball, Paddle* paddle) {
    BallHitPaddleEventArgs e(ball, paddle);
    ofNotifyEvent(ballHitPaddleEvent, e);
    logEvent("BallHitPaddle", e);
  }
  void notifyBallHitBrick(Ball* ball, Brick* brick) {
    BallHitBrickEventArgs e(ball, brick);
    ofNotifyEvent(ballHitBrickEvent, e);
    logEvent("BallHitBrick", e);
  }
  void notifyBallHitWall(Ball* ball, Wall* wall) {
    BallHitWallEventArgs e(ball, wall);
    ofNotifyEvent(ballHitWallEvent, e);
    logEvent("BallHitWall", e);
  }
  void notifyBallHitBall(Ball* ball, Ball* otherBall) {
    BallHitBallEventArgs e(ball, otherBall);
    ofNotifyEvent(ballHitBallEvent, e);
    logEvent("BallHitBall", e);
  }
};

class RoundStateEventArgs : public Outputable {
public:
  RoundStateEventArgs(RoundState& state) : _state(state) { }
  
  RoundState& state() { return _state; }
  
  virtual void output(std::ostream& os) const override {
    os << "()";
  }
  
private:
  RoundState& _state;
};

class BallOwnerChangedEventArgs : public RoundStateEventArgs {
public:
  BallOwnerChangedEventArgs(RoundState& state,
                            Ball* ball,
                            Player* player,
                            Player* previousPlayer)
  : RoundStateEventArgs(state)
  , _ball(ball), _player(player)
  , _previousPlayer(previousPlayer) {}
  
  Ball* ball() { return _ball; }
  const Ball* ball() const { return _ball; }
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  
  Player* previousPlayer() { return _previousPlayer; }
  const Player* previousPlayer() const { return _previousPlayer; }
  
  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, ball());
    os << " ";
    outputField(os, player());
    os << " ";
    outputField(os, "previousPlayer", previousPlayer());
    os << ")";
  }
private:
  Ball* _ball;
  Player* _player;
  Player* _previousPlayer;
};

class BrickDestroyedEventArgs : public RoundStateEventArgs {
public:
  BrickDestroyedEventArgs(RoundState& state, Brick* brick, Ball* ball)
  : RoundStateEventArgs(state)
  , _brick(brick), _ball(ball) { }
  
  Brick* brick() { return _brick; }
  const Brick* brick() const { return _brick; }
  Ball* ball() { return _ball; }
  const Ball* ball() const { return _ball; }
  
  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, brick());
    os << " ";
    outputField(os, ball());
    os << ")";
  }
private:
  Brick* _brick;
  Ball* _ball;
};

template<typename T>
class RoundStateObjectEventArgs
: public RoundStateEventArgs {
public:
  RoundStateObjectEventArgs(RoundState& state, T* obj)
  : RoundStateEventArgs(state)
  , _object(obj) { }
  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, _object);
    os << ")";
  }
  T* object() { return _object; }
  const T* object() const { return _object; }
private:
  T* _object;
};

typedef RoundStateObjectEventArgs<Player> PlayerEventArgs;
typedef RoundStateObjectEventArgs<Ball> BallEventArgs;

class RoundStateEventLogger;


class RoundStateEventSource : public EventSource {
public:
  ofEvent<BallOwnerChangedEventArgs> ballOwnerChangedEvent;
  ofEvent<BrickDestroyedEventArgs> brickDestroyedEvent;
  ofEvent<RoundStateEventArgs> allBricksDestroyedEvent;
  ofEvent<PlayerEventArgs > playerScoreChangedEvent;
  ofEvent<BallEventArgs> ballDestroyedEvent;
  ofEvent<BallEventArgs> ballRespawnedEvent;
  ofEvent<PlayerEventArgs> playerLostEvent;
  ofEvent<PlayerEventArgs> playerLivesChangedEvent;
  ofEvent<RoundStateEventArgs> roundEndedEvent;
  
  template<typename Listener>
  void attachListener(Listener& listener) {
    ofAddListener(ballOwnerChangedEvent, &listener, &Listener::onBallOwnerChanged);
    ofAddListener(brickDestroyedEvent, &listener, &Listener::onBrickDestroyed);
    ofAddListener(playerScoreChangedEvent, &listener, &Listener::onPlayerScoreChanged);
    ofAddListener(ballDestroyedEvent, &listener, &Listener::onBallDestroyed);
    ofAddListener(ballRespawnedEvent, &listener, &Listener::onBallRespawned);
    ofAddListener(playerLostEvent, &listener, &Listener::onPlayerLost);
    ofAddListener(playerLivesChangedEvent, &listener, &Listener::onPlayerLivesChanged);
    ofAddListener(roundEndedEvent, &listener, &Listener::onRoundEnded);
  }
  
  template<typename Listener>
  void detachListener(Listener& listener) {
    ofRemoveListener(ballOwnerChangedEvent, &listener, &Listener::onBallOwnerChanged);
    ofRemoveListener(brickDestroyedEvent, &listener, &Listener::onBrickDestroyed);
    ofRemoveListener(playerScoreChangedEvent, &listener, &Listener::onPlayerScoreChanged);
    ofRemoveListener(ballDestroyedEvent, &listener, &Listener::onBallDestroyed);
    ofRemoveListener(ballRespawnedEvent, &listener, &Listener::onBallRespawned);
    ofRemoveListener(playerLostEvent, &listener, &Listener::onPlayerLost);
    ofRemoveListener(playerLivesChangedEvent, &listener, &Listener::onPlayerLivesChanged);
    ofRemoveListener(roundEndedEvent, &listener, &Listener::onRoundEnded);
  }
protected:
  void notifyBallOwnerChanged(RoundState& state, Ball* ball, Player* player, Player* previousPlayer) {
    BallOwnerChangedEventArgs e(state, ball, player, previousPlayer);
    ofNotifyEvent(ballOwnerChangedEvent, e);
    logEvent("BallOwnerChanged", e);
  }
  void notifyBrickDestroyed(RoundState& state, Brick* brick, Ball* ball) {
    BrickDestroyedEventArgs e(state, brick, ball);
    ofNotifyEvent(brickDestroyedEvent, e);
    logEvent("BrickDestroyed", e);
  }
  void notifyAllBricksDestroyed(RoundState& state) {
    RoundStateEventArgs e(state);
    ofNotifyEvent(allBricksDestroyedEvent, e);
    logEvent("AllBricksDestroyed", e);
  }
  void notifyPlayerScoreChanged(RoundState& state, Player* player) {
    PlayerEventArgs e(state, player);
    ofNotifyEvent(playerScoreChangedEvent, e);
    logEvent("PlayerScoreChanged", e);
  }
  void notifyBallDestroyed(RoundState& state, Ball* ball) {
    BallEventArgs e(state, ball);
    ofNotifyEvent(ballDestroyedEvent, e);
    logEvent("BallDestroyed", e);
  }
  void notifyBallRespawned(RoundState& state, Ball* ball) {
    BallEventArgs e(state, ball);
    ofNotifyEvent(ballRespawnedEvent, e);
    logEvent("BallRespawned", e);
  }
  void notifyPlayerLost(RoundState& state, Player* player) {
    PlayerEventArgs e(state, player);
    ofNotifyEvent(playerLostEvent, e);
    logEvent("PlayerLost", e);
  }
  void notifyPlayerLivesChanged(RoundState& state, Player* player) {
    PlayerEventArgs e(state, player);
    ofNotifyEvent(playerLivesChangedEvent, e);
    logEvent("PlayerLivesChanged", e);
  }
  void notifyRoundEnded(RoundState& state) {
    RoundStateEventArgs e(state);
    ofNotifyEvent(roundEndedEvent, e);
    logEvent("RoundEnded", e);
  }
};

class PlayerEventSource {
public:
  ofEvent<PlayerEventArgs> playerAddedEvent;
  ofEvent<PlayerEventArgs> playerRemovedEvent;
  
  template<typename Listener>
  void attachListener(Listener& listener) {
    ofAddListener(playerAddedEvent, &listener, &Listener::onPlayerAdded);
    ofAddListener(playerRemovedEvent, &listener, &Listener::onPlayerRemoved);
  }
  template<typename Listener>
  void detachListener(Listener& listener) {
    ofRemoveListener(playerAddedEvent, &listener, &Listener::onPlayerAdded);
    ofRemoveListener(playerRemovedEvent, &listener, &Listener::onPlayerRemoved);
  }
protected:
  void notifyPlayerAdded(RoundState& state, Player* player) {
    PlayerEventArgs e(state, player);
    ofNotifyEvent(playerAddedEvent, e);
  }
  void notifyPlayerRemoved(RoundState& state, Player* player) {
    PlayerEventArgs e(state, player);
    ofNotifyEvent(playerRemovedEvent, e);
  }
};

#endif /* defined(__bleepout__GameEvents__) */
