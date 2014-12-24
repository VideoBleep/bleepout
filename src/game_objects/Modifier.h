//
//  Modifier.h
//  bleepout
//
//  Created by tekt on 12/15/14.
//
//

#ifndef __bleepout__Modifier__
#define __bleepout__Modifier__

#include "GameObject.h"
#include "ObjectSpecs.h"
#include "PhysicsObject.h"
#include "BleepoutConfig.h"

class Brick;
class RoundState;

class Modifier : public GameObject, public PhysicsObject {
public:
  Modifier(const ModifierSpec &spec);
  
  virtual void setup(const RoundConfig& config, const Brick& spawner);
  
  ModifierType modifierType() const { return _spec.type; }
  
  virtual bool applyToTarget(RoundState& state, GameObject& target) = 0;
  
  virtual bool visible() const override {
    return this->alive() && _visible;
  }
  void show() { _visible = false; }
  void hide() { _visible = false; }
  
  virtual bool physical() const override {
    return this->alive() && _physical;
  }
  void dematerialize() { _physical = false; }
  void materialize() { _physical = true; }
  virtual void output(std::ostream& os) const override = 0;

  const ModifierSpec& spec() const { return _spec; }

  static Modifier* createModifier(const ModifierSpec& spec);

  virtual void tick() override;

protected:
  const ModifierSpec& _spec;

private:
  bool _visible;
  bool _physical;
};

template<>
struct GameObjectTypeTraits<Modifier> {
  static const GameObjectType typeId = GAME_OBJECT_MODIFIER;
  static const char typeName[];
};

class ModifierSlot {
public:
  ModifierSlot() : _spec(NULL), _endTime(-1) { }
  void set(const ModifierSpec &spec, float time) {
    _spec = &spec;
    _endTime = time + spec.duration;
  }
  const ModifierSpec* clear() {
    const ModifierSpec* s = _spec;
    _spec = NULL;
    return s;
  }
  const ModifierSpec* spec() const { return _spec; }
  bool active() const { return !!_spec; }
  bool checkExpiration(float time) const {
    return time >= _endTime;
  }
  
private:
  const ModifierSpec* _spec;
  float _endTime;
};

#endif /* defined(__bleepout__Modifier__) */
