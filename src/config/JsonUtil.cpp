//
//  JsonUtil.cpp
//  bleepout
//
//  Created by tekt on 12/11/14.
//
//

#include "JsonUtil.h"

#include <fstream>

bool readJsonFile(std::string path, Json::Value* obj) {
  path = ofToDataPath(path);
  std::ifstream fis(path.c_str());
  Json::Reader reader;
  if (!reader.parse(fis, *obj)) {
    ofLogError() << "error loading json from: " << path
                 << ": " << reader.getFormattedErrorMessages();
    return false;
  }
  if (!obj->isObject()) {
    ofLogError() << "invalid type for root (expected object): "
                 << obj->toStyledString();
    return false;
  }
}

void writeJsonFile(std::string path, const Json::Value& obj) {
  path = ofToDataPath(path);
  std::ofstream fos(path.c_str());
  Json::StyledStreamWriter writer;
  writer.write(fos, obj);
}

bool assertType(const Json::Value& val, Json::ValueType type) {
  if (!val.isConvertibleTo(type) || val.isNull()) {
    ofLogError() << "cannot conver value to " << type << ": " << val;
    return false;
  }
  return true;
}

template<typename T>
static bool readJsonValImpl(const Json::Value& val, T* result, Json::ValueType type, T (Json::Value::*converter)() const) {
  if (!assertType(val, type))
    return false;
  *result = (T)(val.*converter)();
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, float* result) {
  if (!assertType(val, Json::realValue))
    return false;
  *result = val.asFloat();
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, int* result) {
  if (!assertType(val, Json::intValue))
    return false;
  *result = val.asInt();
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, unsigned char* result) {
  if (!assertType(val, Json::intValue))
    return false;
  *result = (unsigned char)val.asInt();
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, bool* result) {
  if (!assertType(val, Json::booleanValue))
    return false;
  *result = val.asBool();
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, std::string* result) {
  if (!assertType(val, Json::booleanValue))
    return false;
  *result = val.asString();
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, ofVec2f* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["x"], &result->x) ||
      !readJsonVal(val["y"], &result->y))
    return false;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, ofVec3f* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["x"], &result->x) ||
      !readJsonVal(val["y"], &result->y) ||
      !readJsonVal(val["z"], &result->z))
    return false;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, ofColor* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["r"], &result->r) ||
      !readJsonVal(val["g"], &result->g) ||
      !readJsonVal(val["b"], &result->b))
    return false;
  if (!readJsonVal(val["a"], &result->a))
    result->a = 255;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, BrickSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["elevation"], &result->elevation) ||
      !readJsonVal(val["heading"], &result->heading) ||
      !readJsonVal(val["color"], &result->color))
    return false;
  if (!readJsonVal(val["value"], &result->value))
    result->value = 1;
  if (!readJsonVal(val["lives"], &result->lives))
    result->lives = 1;
  if (!readJsonVal(val["speed"], &result->speed))
    result->speed = 0;
  if (!readJsonVal(val["stopHeading"], &result->stopHeading))
    result->stopHeading = -1;
  if (!readJsonVal(val["modifierName"], &result->modifierName))
    result->modifierName = "";
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, BrickRingSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["elevation"], &result->elevation) ||
      !readJsonVal(val["color"], &result->color) ||
      !readJsonVal(val["count"], &result->count) ||
      !readJsonVal(val["phase"], &result->phase))
    return false;
  if (!readJsonVal(val["value"], &result->value))
    result->value = 1;
  if (!readJsonVal(val["lives"], &result->lives))
    result->lives = 1;
  if (!readJsonVal(val["speed"], &result->speed))
    result->speed = 0;
  if (!readJsonVal(val["stopHeading"], &result->stopHeading))
    result->stopHeading = -1;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, WallSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["elevation"], &result->elevation) ||
      !readJsonVal(val["heading"], &result->heading) ||
      !readJsonVal(val["size"], &result->size))
    return false;
  if (!readJsonVal(val["isExit"], &result->isExit))
    result->isExit = false;
  if (!readJsonVal(val["speed"], &result->speed))
    result->speed = 0;
  if (!readJsonVal(val["stopHeading"], &result->stopHeading))
    result->stopHeading = -1;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, CurvedWallSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["elevation1"], &result->elevation1) ||
      !readJsonVal(val["heading1"], &result->heading1) ||
      !readJsonVal(val["elevation2"], &result->elevation2) ||
      !readJsonVal(val["heading2"], &result->heading2) ||
      !readJsonVal(val["width"], &result->width))
    return false;
  if (!readJsonVal(val["isExit"], &result->isExit))
    result->isExit = false;
  if (!readJsonVal(val["speed"], &result->speed))
    result->speed = 0;
  if (!readJsonVal(val["stopHeading"], &result->stopHeading))
    result->stopHeading = -1;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, BallSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  if (!readJsonVal(val["elevation"], &result->elevation) ||
      !readJsonVal(val["heading"], &result->heading))
    return false;
  return true;
}

template<>
Json::Value toJsonVal(const ofVec2f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  return obj;
}

template<>
Json::Value toJsonVal(const ofVec3f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  obj["z"] = val.z;
  return obj;
}

template<>
Json::Value toJsonVal(const ofColor& val) {
  Json::Value obj(Json::objectValue);
  obj["r"] = val.r;
  obj["g"] = val.g;
  obj["b"] = val.b;
  obj["a"] = val.a;
  return obj;
}

template<typename T>
Json::Value toJsonVal(const T& val) {
  return Json::Value(val);
}

template<>
Json::Value toJsonVal(const BrickSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  obj["color"] = toJsonVal(spec.color);
  obj["value"] = spec.value;
  obj["lives"] = spec.lives;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  obj["modifierName"] = spec.modifierName;
  return obj;
}

template<>
Json::Value toJsonVal(const BrickRingSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["color"] = toJsonVal(spec.color);
  obj["value"] = spec.value;
  obj["lives"] = spec.lives;
  obj["count"] = spec.count;
  obj["phase"] = spec.phase;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  return obj;
}

template<>
Json::Value toJsonVal(const WallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  obj["size"] = toJsonVal(spec.size);
  obj["isExit"] = toJsonVal(spec.isExit);
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  return obj;
}

template<>
Json::Value toJsonVal(const CurvedWallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation1"] = spec.elevation1;
  obj["heading1"] = spec.heading1;
  obj["elevation2"] = spec.elevation2;
  obj["heading2"] = spec.heading2;
  obj["width"] = toJsonVal(spec.width);
  obj["isExit"] = toJsonVal(spec.isExit);
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  return obj;
}

template<>
Json::Value toJsonVal(const BallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  return obj;
}

template<>
Json::Value toJsonVal(const ModifierType& type) {
  switch (type) {
    case MODIFIER_EXTRA_LIFE:
      return "extra_life";
    case MODIFIER_PADDLE_WIDTH:
      return "paddle_width";
    case MODIFIER_NONE:
    default:
      return Json::Value(Json::nullValue);
  }
}

template<>
bool readJsonVal(const Json::Value& val, ModifierType* result) {
  if (val.isNull()) {
    *result = MODIFIER_NONE;
    return true;
  }
  if (!assertType(val, Json::stringValue))
    return false;
  std::string str = val.asString();
  if (str.empty()) {
    *result = MODIFIER_NONE;
    return true;
  }
  if (str == "extra_life") {
    *result = MODIFIER_EXTRA_LIFE;
    return true;
  }
  if (str == "paddle_width") {
    *result = MODIFIER_PADDLE_WIDTH;
    return true;
  }
  return false;
}

template<>
Json::Value toJsonVal(const ModifierSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["type"] = toJsonVal(spec.type);
  obj["amount"] = spec.amount;
  return obj;
}

template<>
bool readJsonVal(const Json::Value& val, ModifierSpec* result) {
  if (val.isNull()) {
    result->type = MODIFIER_NONE;
    return true;
  }
  if (!readJsonVal(val["type"], &result->type))
    return false;
  if (!readJsonVal(val["amount"], &result->amount))
    result->amount = 0;
  return true;
}
