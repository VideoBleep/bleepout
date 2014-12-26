//
//  GameObject.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef __bleepout__GameObject__
#define __bleepout__GameObject__

#include <iostream>
#include <ofColor.h>
#include "Common.h"

//#define LOG_CONSTRUCTION_DESTRUCTION


class RoundState;
struct ModifierSpec;

class GameObject {
public:
  explicit GameObject(GameObjectType t);
  virtual ~GameObject();
  
  GameObjectType type() const { return _type; }
  GameObjectId id() const { return _id; }
  bool alive() const { return _alive; }
  virtual bool visible() const { return this->alive(); }
  virtual bool physical() const { return this->alive(); }
  
  void kill() { _alive = false; }
  void revive() { _alive = true; }
  
  virtual const ofColor& getColor() const { return _color; }
  virtual void setColor(const ofColor& color) { _color = color; }
  
  virtual void output(std::ostream& os) const = 0;
private:
  GameObjectId _id;
  GameObjectType _type;
  bool _alive;
protected:
  ofColor _color;
};

template<typename T>
struct GameObjectTypeTraits {
};

#endif /* defined(__bleepout__GameObject__) */
