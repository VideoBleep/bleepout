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

enum ModifierType {
  MODIFIER_UNKNOWN
};

class Modifier : public GameObject {
public:
  Modifier(ModifierType modifierType)
  : GameObject(GAME_OBJECT_MODIFIER)
  , _modifierType(modifierType)
  , _visible(false)
  , _physical(false) { }
  
  ModifierType modifierType() const { return _modifierType; }
  virtual float duration() { return 0; }
  
  virtual bool applyToTarget(GameObject& target) = 0;
  
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

//template<typename T>
//class TypedModifier : public Modifier {
//public:
//  TypedModifier(ModifierType modifierType)
//  : Modifier(modifierType) { }
//  
//  virtual bool applyToTarget(GameObject& target) override {
//    if (target.type() != GameObjectTypeTraits<T>::typeId) {
//      return false;
//    }
//    return applyToTypedTarget(static_cast<T&>(target));
//  }
//protected:
//  virtual bool applyToTypedTarget(T& target) = 0;
//};

template<typename ModInstanceType, typename TargetType>
class ModifierSlot {
public:
  ModifierSlot(ModifierType type) : _type(type) { }
  
  ModifierType type() const { return _type; }
  
  bool active() const { return _active; }
  
  bool update(TargetType& target, float time) {
    if (_endTime > 0.0f && time >= _endTime) {
      unapply(target);
      _active = false;
      return false;
    }
    return true;
  }
  
  void add(ModInstanceType& mod, TargetType& target, float time) {
    apply(mod, target);
    float dur = mod.duration();
    if (dur <= 0) {
      _endTime = 0;
    } else {
      _endTime = time + dur;
    }
    _active = true;
  }
  
protected:
  void apply(ModInstanceType& mod, TargetType& target);
  void unapply(TargetType& target);
private:
  const ModifierType _type;
  bool _active;
  float _endTime;
};

//class ModifiableGameObject : public GameObject {
//public:
//  ModifiableGameObject(GameObjectType type)
//  : GameObject(type) { }
//  
//  virtual bool applyModifier(Modifier& mod) {
//    return false;
//  }
//};

#endif /* defined(__bleepout__Modifier__) */
