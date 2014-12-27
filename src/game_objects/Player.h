//
//  Player.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Player__
#define __bleepout__Player__

#include "GameObject.h"
#include <iostream>
#include <ofTypes.h>
#include <Connection.h>
#include "Modifier.h"
#include "Common.h"
#include <list>

class Paddle;
class RoundState;
struct ModifierSpec;

class Player : public GameObject {
public:
  Player();
  Player(ofxLibwebsockets::Connection* conn);
  
  void setPaddle(Paddle* paddle) { _paddle = paddle; }
  Paddle* paddle() { return _paddle; }
  const Paddle* paddle() const { return _paddle; }
  
  int score() const { return _score; }
  int adjustScore(int add) {
    _score += add;
    return _score;
  }
  
  int lives() const { return _lives; }
  void setLives(int lives) {
    if (lives <= 0) {
      _lives = 0;
      kill();
    } else {
      _lives = lives;
      revive();
    }
  }
  int adjustLives(int amount) {
    setLives(_lives + amount);
    return _lives;
  }
  
  virtual bool physical() const override { return false; }
  virtual bool visible() const override { return false; }
  
  const Counters<ModifierType>& modifierCounters() const { return _modifierCounters; }
  
  void send(std::string message) { _conn->send(message); }
  void output(std::ostream& os) const override;
  
  ofxLibwebsockets::Connection* connection() { return _conn; }
  void connection(ofxLibwebsockets::Connection* conn) { _conn = conn; }
  
  void enqueueBallModifier(const ModifierSpec& modifierSpec);
  bool tryDequeueBallModifier(ModifierSpec* modifierSpec);
  
  void incrementModifierCount(ModifierType type);
  
private:
  void init();
  
  ofxLibwebsockets::Connection* _conn;
  Paddle* _paddle;
  int _score;
  int _lives;
  
  std::list<ModifierSpec> _ballModifierQueue;
  Counters<ModifierType> _modifierCounters;
};

template<>
struct GameObjectTypeTraits<Player> {
  static const GameObjectType typeId = GAME_OBJECT_PLAYER;
  static const char typeName[];
};

class ExtraLifeModifier : public Modifier {
public:
  ExtraLifeModifier(const ModifierSpec& spec) : Modifier(spec) { }
  virtual bool applyToTarget(RoundState& state, GameObject& target) override;
  virtual void output(std::ostream& os) const override;
};

#endif /* defined(__bleepout__Player__) */
