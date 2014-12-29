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

#define FOR_EACH_OBJ_TYPE(X) \
  X(GAME_OBJECT_BRICK, "Brick") \
  X(GAME_OBJECT_PADDLE, "Paddle") \
  X(GAME_OBJECT_BALL, "Ball") \
  X(GAME_OBJECT_PLAYER, "Player") \
  X(GAME_OBJECT_WALL, "Wall") \
  X(GAME_OBJECT_ANIMATION, "AnimationObject") \
  X(GAME_OBJECT_MODIFIER, "Modifier")

template<>
bool EnumTypeTraits<GameObjectType>::parseString(const std::string &str, GameObjectType *result, const GameObjectType &defaultVal) {
  FOR_EACH_OBJ_TYPE(ENUM_PARSE_CASE)
  *result = defaultVal;
  return false;
}

template<>
bool EnumTypeTraits<GameObjectType>::toString(const GameObjectType &value, std::string* result) {
  switch (value) {
    FOR_EACH_OBJ_TYPE(ENUM_TOSTR_CASE)
    case GAME_OBJECT_OTHER:
    default:
      return false;
  }
  return true;
}

#undef FOR_EACH_OBJ_TYPE

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

