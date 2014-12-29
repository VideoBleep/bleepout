//
//  GameObject.cpp
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#include "GameObject.h"
#include "Logging.h"
#include "JsonUtil.h"

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

template<>
bool EnumTypeTraits<GameObjectType>::parseString(const std::string &str, GameObjectType *result, const GameObjectType &defaultVal) {
  if (str == "brick")
    *result = GAME_OBJECT_BRICK;
  else if (str == "paddle")
    *result = GAME_OBJECT_PADDLE;
  else if (str == "ball")
    *result = GAME_OBJECT_BALL;
  else if (str == "player")
    *result = GAME_OBJECT_PLAYER;
  else if (str == "wall")
    *result = GAME_OBJECT_WALL;
  else if (str == "animation")
    *result = GAME_OBJECT_ANIMATION;
  else if (str == "wall")
    *result = GAME_OBJECT_MODIFIER;
  else {
    *result = defaultVal;
    return false;
  }
  return true;
}

template<>
std::string EnumTypeTraits<GameObjectType>::toString(const GameObjectType &value) {
  switch (value) {
    case GAME_OBJECT_BRICK:
      return GameObjectTypeTraits<Brick>::typeName;
    case GAME_OBJECT_PADDLE:
      return GameObjectTypeTraits<Paddle>::typeName;
    case GAME_OBJECT_BALL:
      return GameObjectTypeTraits<Ball>::typeName;
    case GAME_OBJECT_PLAYER:
      return GameObjectTypeTraits<Player>::typeName;
    case GAME_OBJECT_WALL:
      return GameObjectTypeTraits<Wall>::typeName;
    case GAME_OBJECT_ANIMATION:
      return GameObjectTypeTraits<AnimationObject>::typeName;
    case GAME_OBJECT_MODIFIER:
      return GameObjectTypeTraits<Modifier>::typeName;
    case GAME_OBJECT_OTHER:
    default:
      return std::string("Unknown{") + ofToString((int)value) + "}";
  }
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         GameObjectType *result,
                         const GameObjectType &defaultVal) const {
  if (!assertType(val, Json::stringValue)) {
    *result = defaultVal;
  } else {
    parseEnumString(val.asString(), result);
  }
}

