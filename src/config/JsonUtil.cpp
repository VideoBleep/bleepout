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
  return true;
}

void writeJsonFile(std::string path, const Json::Value& obj) {
  path = ofToDataPath(path);
  std::ofstream fos(path.c_str());
  Json::StyledStreamWriter writer;
  writer.write(fos, obj);
}

//template<typename T>
//static bool readJsonValImpl(const Json::Value& val, T* result, Json::ValueType type, T (Json::Value::*converter)() const) {
//  if (!assertType(val, type))
//    return false;
//  *result = (T)(val.*converter)();
//  return true;
//}

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
Json::Value toJsonVal(const ModifierSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["type"] = toJsonVal(spec.type);
  obj["amount"] = spec.amount;
  return obj;
}



bool JsonLoader::readFile(std::string path, Json::Value *result) const {
  path = ofToDataPath(path);
  std::ifstream fis(path.c_str());
  Json::Reader reader;
  if (!reader.parse(fis, *result)) {
    ofLog(_logLevel) << "error loading json from: " << path
    << ": " << reader.getFormattedErrorMessages();
    return false;
  }
  if (!result->isObject()) {
    ofLog(_logLevel) << "invalid type for root (expected object): "
    << result->toStyledString();
    return false;
  }
  return true;
}

bool JsonLoader::assertType(const Json::Value &val, Json::ValueType type) const {
  if (!val.isConvertibleTo(type) || val.isNull()) {
    ofLog(_logLevel) << "cannot convert value to " << type
    << ": " << val;
    return false;
  }
  return true;
}

template<>
void JsonLoader::readVal(const Json::Value &val, float *result,
                         const float& defaultVal) const {
  if (!assertType(val, Json::realValue))
    *result = defaultVal;
  else
    *result = val.asFloat();
}

template<>
void JsonLoader::readVal(const Json::Value &val, int *result,
                         const int& defaultVal) const {
  if (!assertType(val, Json::intValue))
    *result = defaultVal;
  else
    *result = val.asInt();
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         unsigned char *result,
                         const unsigned char& defaultVal) const {
  if (!assertType(val, Json::uintValue))
    *result = defaultVal;
  else
    *result = (unsigned char)val.asUInt();
}

template<>
void JsonLoader::readVal(const Json::Value &val, bool *result,
                         const bool& defaultVal) const {
  if (!assertType(val, Json::booleanValue))
    *result = defaultVal;
  else
    *result = val.asBool();
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         std::string *result,
                         const std::string& defaultVal) const {
  if (!assertType(val, Json::stringValue))
    *result = defaultVal;
  else
    *result = val.asString();
}

#define R_JPROP(property) readVal(val[#property], &result->property, defaultVal.property)

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ofVec3f *result,
                         const ofVec3f& defaultVal) const {
  if (val.isArray()) {
    readVal(val[0], &result->x, defaultVal.x);
    readVal(val[1], &result->y, defaultVal.y);
    readVal(val[2], &result->z, defaultVal.z);
  } if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(x);
    R_JPROP(y);
    R_JPROP(z);
  }
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ofColor *result,
                         const ofColor& defaultVal) const {
  if (val.isArray()) {
    readVal(val[0], &result->r, defaultVal.r);
    readVal(val[1], &result->g, defaultVal.g);
    readVal(val[2], &result->b, defaultVal.b);
    readVal(val[3], &result->a, defaultVal.a);
  } else if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(r);
    R_JPROP(g);
    R_JPROP(b);
    R_JPROP(a);
  }
}
