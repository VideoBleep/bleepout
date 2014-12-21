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
#include <list>
#include "GameObject.h"
#include "Player.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "Wall.h"
#include "GameState.h"
#include "Common.h"
#include "Logging.h"
#include "BleepoutConfig.h"

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

class CollisionEventArgs : public Outputable {
public:
  CollisionEventArgs(GameObject* a, GameObject *b)
  : _a(a), _b(b) { }
  
  GameObject* a() { return _a; }
  const GameObject* a() const { return _a; }
  GameObject* b() { return _b; }
  const GameObject* b() const { return _b; }
  
  virtual void output(std::ostream& os) const override;
private:
  GameObject* _a;
  GameObject* _b;
};

class ModifierHitPaddleEventArgs : public Outputable {
public:
  ModifierHitPaddleEventArgs(Modifier* modifier, Paddle* paddle)
  : _modifier(modifier), _paddle(paddle) { }
  
  Modifier* modifier() { return _modifier; }
  const Modifier* modifier() const { return _modifier; }
  Paddle* paddle() { return _paddle; }
  const Paddle* paddle() const { return _paddle; }

  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, modifier());
    os << " ";
    outputField(os, paddle());
    os << ")";
  }
private:
  Modifier* _modifier;
  Paddle* _paddle;
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

class ModifierEventArgs
: public RoundStateEventArgs {
public:
  ModifierEventArgs(RoundState& state, Modifier* modifier,
                    GameObject* target)
  : RoundStateEventArgs(state)
  , _modifier(modifier)
  , _target(target) { }
  
  Modifier* modifier() { return _modifier; }
  GameObject* target() { return _target; }
  
  virtual void output(std::ostream& os) const override {
    os << "(";
    outputField(os, _modifier);
    os << " ";
    outputField(os, "target", _target);
    os << ")";
  }
private:
  Modifier* _modifier;
  GameObject* _target;
};

typedef RoundStateObjectEventArgs<Player> PlayerEventArgs;
typedef RoundStateObjectEventArgs<Ball> BallEventArgs;

class StartRoundEventArgs : public Outputable {
public:
  StartRoundEventArgs(ofPtr<RoundConfig> config,
                      std::list<ofPtr<Player> > players)
  : _config(config) , _players(players) { }
  
  ofPtr<RoundConfig>& config() { return _config; }
  const ofPtr<RoundConfig>& config() const { return _config; }
  std::list<ofPtr<Player> >& players() { return _players; }
  const std::list<ofPtr<Player> >& players() const { return _players; }
  
  virtual void output(std::ostream& os) const override {
    os << "(";
    os << "config:" << config()->name() << ", ";
    os << "players:" << players().size();
    os << ")";
  }
private:
  ofPtr<RoundConfig> _config;
  std::list<ofPtr<Player> > _players;
};
  
class PlayerYawPitchRollEventArgs {
public:
  PlayerYawPitchRollEventArgs(Player* player, float yaw,
                              float pitch, float roll)
  : _player(player), _yaw(yaw), _pitch(pitch), _roll(roll) { }
  
  Player* player() { return _player; }
  float yaw() const { return _yaw; }
  float pitch() const { return _pitch; }
  float roll() const { return _roll; }
  
private:
  Player* _player;
  float _yaw;
  float _pitch;
  float _roll;
};
  
class EventSource {
public:
  EventSource() : _logLevel(OF_LOG_SILENT) {}
  void enableLogging(ofLogLevel level) { _logLevel = level; }
  void disableLogging() { _logLevel = OF_LOG_SILENT; }
  bool loggingEnabled() const {
    return _logLevel != OF_LOG_SILENT;
  }
protected:
  void logEvent(const char* name, const Outputable& event) const;
private:
  ofLogLevel _logLevel;
};
  
class PlayerEventSource : public EventSource {
public:
  ofEvent<PlayerEventArgs> playerAddedEvent;
  ofEvent<PlayerEventArgs> playerRemovedEvent;
protected:
  void notifyPlayerAdded(RoundState& state, Player* player);
  void notifyPlayerRemoved(RoundState& state, Player* player);
};

class ControlEventSource : public EventSource {
public:
  ofEvent<PlayerYawPitchRollEventArgs> playerYawPitchRollEvent;
protected:
  void notifyPlayerYawPitchRoll(Player* player, float yaw,
                                float pitch, float roll);
};

#endif /* defined(__bleepout__GameEvents__) */
