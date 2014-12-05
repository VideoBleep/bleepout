//
//  GameObject.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "GameObject.h"

static GameObjectId nextId() {
  static GameObjectId lastId = 0;
  return ++lastId;
}

GameObject::GameObject(GameObjectType t)
  : _key(t, nextId()) {}
