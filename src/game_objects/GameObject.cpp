//
//  GameObject.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "GameObject.h"
#include "Logging.h"

static GameObjectId nextId() {
  static GameObjectId lastId = 0;
  return ++lastId;
}

GameObject::GameObject(GameObjectType t)
: _type(t)
, _id(nextId())
, _alive(true) {
#ifdef LOG_CONSTRUCTION_DESTRUCTION
  ofLogNotice() << "Create<" << _type << ">#" << _id;
#endif // LOG_CONSTRUCTION_DESTRUCTION
}

GameObject::~GameObject() {
#ifdef LOG_CONSTRUCTION_DESTRUCTION
  ofLogNotice() << "Destroy<" << _type << ">#" << _id;
#endif // LOG_CONSTRUCTION_DESTRUCTION
}

