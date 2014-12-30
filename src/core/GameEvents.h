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
#include "RoundResults.h"

class EmptyEventArgs : public Outputable {
public:
  virtual ~EmptyEventArgs() {}
  void output(std::ostream& os) const override;
};

class CollisionEventArgs : public Outputable {
public:
  CollisionEventArgs(GameObject* a, GameObject *b)
  : _a(a), _b(b) { }
  virtual ~CollisionEventArgs() {}
  
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
  virtual ~ModifierHitPaddleEventArgs() {}
  
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
  virtual ~RoundStateEventArgs() {}
  
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
  virtual ~BallOwnerChangedEventArgs() {}
  
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

class BrickHitEventArgs : public RoundStateEventArgs {
public:
  BrickHitEventArgs(RoundState& state, Brick* brick, Ball* ball)
  : RoundStateEventArgs(state)
  , _brick(brick), _ball(ball) { }
  
  virtual ~BrickHitEventArgs() {}
  
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
  virtual ~ModifierEventArgs() {}
  
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
  virtual ~ModifierRemovedEventArgs() {}
  
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
  
  virtual ~BallStateEventArgs() {}
  
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
  virtual ~PlayerStateEventArgs() {}
  
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  virtual void output(std::ostream& os) const override;
private:
  Player* _player;
};

class PlayerEventArgs : public Outputable {
public:
  PlayerEventArgs(Player* player)
  : _player(player) { }
  virtual ~PlayerEventArgs() {}
  Player* player() { return _player; }
  const Player* player() const { return _player; }
  virtual void output(std::ostream& os) const override;
private:
  Player* _player;
};

class RequestEventArgs {
public:
  RequestEventArgs() : _handled(false) { }
  
  void markHandled() { _handled = true; }
  bool handled() const { return _handled; }
private:
  bool _handled;
};

class SpawnBallEventArgs
: public RequestEventArgs
, public Outputable {
public:
  SpawnBallEventArgs(BallSpec ballSpec)
  : RequestEventArgs(), _ballSpec(ballSpec) { }
  virtual ~SpawnBallEventArgs() {}
  void output(std::ostream& os) const override;
  BallSpec& ballSpec() { return _ballSpec; }
private:
  BallSpec _ballSpec;
};

class StartRoundEventArgs
: public RequestEventArgs
, public Outputable {
public:
  StartRoundEventArgs(std::list<ofPtr<RoundConfig> > configs,
                      std::list<ofPtr<Player> > players)
  : RequestEventArgs(), _configs(configs) , _players(players) { }
  
  virtual ~StartRoundEventArgs() {}
  
  std::list<ofPtr<RoundConfig> >& configs() { return _configs; }
  const std::list<ofPtr<RoundConfig> >& config() const { return _configs; }
  std::list<ofPtr<Player> >& players() { return _players; }
  const std::list<ofPtr<Player> >& players() const { return _players; }
  
  void output(std::ostream& os) const override;
private:
  std::list<ofPtr<RoundConfig> > _configs;
  std::list<ofPtr<Player> > _players;
};

class EndRoundEventArgs
: public RequestEventArgs
, public Outputable {
public:
  EndRoundEventArgs(RoundEndReason reason)
  : RequestEventArgs(), _reason(reason) { }
  virtual ~EndRoundEventArgs() {}
  void output(std::ostream& os) const override;
  RoundEndReason reason() const { return _reason; }
private:
  RoundEndReason _reason;
};

class RoundEndedEventArgs
: public Outputable {
public:
  RoundEndedEventArgs(const RoundResults& results)
  : _results(results) { }
  virtual ~RoundEndedEventArgs() {}
  
  const RoundResults& results() const { return _results; }
  
  void output(std::ostream& os) const override;
private:
  const RoundResults& _results;
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

class TimerEventArgs : public Outputable {
public:
  TimerEventArgs(float currentTime, float remainingTime)
  : _currentTime(currentTime), _remainingTime(remainingTime) { }
  virtual ~TimerEventArgs() {}
  
  float currentTime() const { return _currentTime; }
  float remainingTime() const { return _remainingTime; }
  void output(std::ostream& os) const override;
private:
  float _currentTime;
  float _remainingTime;
};

class EventSource {
public:
  EventSource() : _logLevel(OF_LOG_SILENT) {}
  virtual ~EventSource() {}
  void enableLogging(ofLogLevel level) { _logLevel = level; }
  void disableLogging() { _logLevel = OF_LOG_SILENT; }
  bool loggingEnabled() const { return _logLevel != OF_LOG_SILENT; }
  void toggleLogging(ofLogLevel level) {
    _logLevel = loggingEnabled() ? OF_LOG_SILENT : level;
  }
  virtual const char* eventSourceName() const = 0;
protected:
  void logEvent(const char* name, const Outputable& event) const;
private:
  ofLogLevel _logLevel;
};

#endif /* defined(__bleepout__GameEvents__) */
