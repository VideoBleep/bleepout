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

class RoundConfig;
class Brick;
class RoundState;

class Modifier : public GameObject, public PhysicsObject {
public:
  Modifier(ModifierType modifierType);
  
  virtual void setup(const RoundConfig& config, const Brick& spawner);
  
  ModifierType modifierType() const { return _modifierType; }
  virtual float duration() const { return 0; }
  
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

  static Modifier* createModifier(const ModifierSpec& spec);
  
private:
  const ModifierType _modifierType;
  bool _visible;
  bool _physical;
};

template<>
struct GameObjectTypeTraits<Modifier> {
  static const GameObjectType typeId = GAME_OBJECT_MODIFIER;
  static const char typeName[];
};

//template<typename ModInstanceType, typename TargetType>
//class ModifierSlot {
//public:
//  ModifierSlot(ModifierType type) : _type(type) { }
//  
//  ModifierType type() const { return _type; }
//  
//  bool active() const { return _active; }
//  
//  bool update(TargetType& target, RoundState& state) {
//    if (_endTime > 0.0f && time >= _endTime) {
//      unapply(target);
//      _active = false;
//      return false;
//    }
//    return true;
//  }
//  
//  void add(ModInstanceType& mod, TargetType& target, RoundState& state) {
//    apply(mod, target);
//    float dur = mod.duration();
//    if (dur <= 0) {
//      _endTime = 0;
//    } else {
//      _endTime = state.time + dur;
//    }
//    _active = true;
//  }
//  
//protected:
//  void apply(ModInstanceType& mod, TargetType& target);
//  void unapply(TargetType& target);
//private:
//  const ModifierType _type;
//  bool _active;
//  float _endTime;
//};

#endif /* defined(__bleepout__Modifier__) */
