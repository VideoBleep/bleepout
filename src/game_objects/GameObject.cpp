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
  if (str == GameObjectTypeTraits<Brick>::typeName)
    *result = GAME_OBJECT_BRICK;
  else if (str == GameObjectTypeTraits<Paddle>::typeName)
    *result = GAME_OBJECT_PADDLE;
  else if (str == GameObjectTypeTraits<Ball>::typeName)
    *result = GAME_OBJECT_BALL;
  else if (str == GameObjectTypeTraits<Player>::typeName)
    *result = GAME_OBJECT_PLAYER;
  else if (str == GameObjectTypeTraits<Wall>::typeName)
    *result = GAME_OBJECT_WALL;
  else if (str == GameObjectTypeTraits<AnimationObject>::typeName)
    *result = GAME_OBJECT_ANIMATION;
  else if (str == GameObjectTypeTraits<Modifier>::typeName)
    *result = GAME_OBJECT_MODIFIER;
  else {
    *result = defaultVal;
    return false;
  }
  return true;
}

template<>
bool EnumTypeTraits<GameObjectType>::toString(const GameObjectType &value, std::string* result) {
  switch (value) {
    case GAME_OBJECT_BRICK:
      *result = GameObjectTypeTraits<Brick>::typeName;
      break;
    case GAME_OBJECT_PADDLE:
      *result = GameObjectTypeTraits<Paddle>::typeName;
      break;
    case GAME_OBJECT_BALL:
      *result = GameObjectTypeTraits<Ball>::typeName;
      break;
    case GAME_OBJECT_PLAYER:
      *result = GameObjectTypeTraits<Player>::typeName;
      break;
    case GAME_OBJECT_WALL:
      *result = GameObjectTypeTraits<Wall>::typeName;
      break;
    case GAME_OBJECT_ANIMATION:
      *result = GameObjectTypeTraits<AnimationObject>::typeName;
      break;
    case GAME_OBJECT_MODIFIER:
      *result = GameObjectTypeTraits<Modifier>::typeName;
      break;
    case GAME_OBJECT_OTHER:
    default:
      return false;
  }
  return true;
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

template<>
Json::Value toJsonVal(const GameObjectType& type) {
  std::string result;
  if (!enumToString(type, &result))
    return Json::Value::null;
  return Json::Value(result);
}

