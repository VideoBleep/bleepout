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

class EmptyEventArgs : public Outputable {
public:
  void output(std::ostream& os) const override;
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

  virtual void output(std::ostream& os) const override;
private:
  Modifier* _modifier;
  Paddle* _paddle;
};

class RoundStateEventArgs : public Outputable {
public:
  RoundStateEventArgs(RoundState& state) : _state(state) { }
  
  RoundState& state() { return _state; }
  
  virtual void output(std::ostream& os) const override;
  
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
  
  virtual void output(std::ostream& os) const override;
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
  
  virtual void output(std::ostream& os) const override;
private:
  Brick* _brick;
  Ball* _ball;
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
  
  virtual void output(std::ostream& os) const override;
private:
  Modifier* _modifier;
  GameObject* _target;
};

class ModifierRemovedEventArgs
: public RoundStateEventArgs {
public:
  ModifierRemovedEventArgs(RoundState& state, const ModifierSpec& modifierSpec, GameObject* target)
  : RoundStateEventArgs(state), _modifierSpec(modifierSpec)
  , _target(target) { }
  
  const ModifierSpec& modifierSpec() const { return _modifierSpec; }
  GameObject* target() { return _target; }
  
  virtual void output(std::ostream& os) const override;
private:
  const ModifierSpec& _modifierSpec;
  GameObject* _target;
};

class BallStateEventArgs : public RoundStateEventArgs {
public:
  BallStateEventArgs(RoundState& state, Ball* ball)
  : RoundStateEventArgs(state), _ball(ball) { }
  
  Ball* ball() { return _ball; }
  const Ball* ball() const { return _ball; }
  
  virtual void output(std::ostream& os) const override;
private:
  Ball* _ball;
};

class PlayerStateEventArgs : public RoundStateEventArgs {
public:
  PlayerStateEventArgs(RoundState& state, Player* player)
  : RoundStateEventArgs(state), _player(player) { }
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  virtual void output(std::ostream& os) const override;
private:
  Player* _player;
};

class PlayerEventArgs : public Outputable {
public:
  PlayerEventArgs(ofPtr<Player> player)
  : _player(player) { }
  ofPtr<Player>& player() { return _player; }
  const ofPtr<Player>& player() const { return _player; }
  virtual void output(std::ostream& os) const override;
private:
  ofPtr<Player> _player;
};

class RequestEventArgs {
public:
  RequestEventArgs() : _handled(false) { }
  
  void markHandled() { _handled = true; }
  bool handled() const { return _handled; }
private:
  bool _handled;
};

class StartRoundEventArgs
: public RequestEventArgs
, public Outputable {
public:
  StartRoundEventArgs(ofPtr<RoundConfig> config,
                      std::list<ofPtr<Player> > players)
  : RequestEventArgs(), _config(config) , _players(players) { }
  
  ofPtr<RoundConfig>& config() { return _config; }
  const ofPtr<RoundConfig>& config() const { return _config; }
  std::list<ofPtr<Player> >& players() { return _players; }
  const std::list<ofPtr<Player> >& players() const { return _players; }
  
  void output(std::ostream& os) const override;
private:
  ofPtr<RoundConfig> _config;
  std::list<ofPtr<Player> > _players;
};

class EndRoundEventArgs
: public RequestEventArgs
, public Outputable {
public:
  EndRoundEventArgs() : RequestEventArgs() { }
  void output(std::ostream& os) const override;
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
  bool loggingEnabled() const { return _logLevel != OF_LOG_SILENT; }
  void toggleLogging(ofLogLevel level) {
    _logLevel = loggingEnabled() ? OF_LOG_SILENT : level;
  }
protected:
  void logEvent(const char* name, const Outputable& event) const;
private:
  ofLogLevel _logLevel;
};

#endif /* defined(__bleepout__GameEvents__) */
