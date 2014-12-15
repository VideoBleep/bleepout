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
  
};

class Modifier : public GameObject {
public:
  Modifier(ModifierType modifierType)
  : GameObject(GAME_OBJECT_MODIFIER)
  , _modifierType(modifierType) { }
  
  ModifierType modifierType() const { return _modifierType; }
  virtual float duration() { return 0; }
private:
  const ModifierType _modifierType;
};

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

class ModifiableGameObject : public GameObject {
public:
  ModifiableGameObject(GameObjectType type)
  : GameObject(type) { }
  
  virtual bool applyModifier(Modifier& mod) {
    return false;
  }
};

#endif /* defined(__bleepout__Modifier__) */
