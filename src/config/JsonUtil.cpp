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
  ofVec2f temp;
  if (!readJsonVal(val["x"], &temp.x) ||
      !readJsonVal(val["y"], &temp.y))
    return false;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, ofVec3f* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  ofVec3f temp;
  if (!readJsonVal(val["x"], &temp.x) ||
      !readJsonVal(val["y"], &temp.y) ||
      !readJsonVal(val["z"], &temp.z))
    return false;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, ofColor* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  ofColor temp;
  if (!readJsonVal(val["r"], &temp.r) ||
      !readJsonVal(val["g"], &temp.g) ||
      !readJsonVal(val["b"], &temp.b))
    return false;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, BrickSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  BrickSpec temp;
  if (!readJsonVal(val["elevation"], &temp.elevation) ||
      !readJsonVal(val["heading"], &temp.heading) ||
      !readJsonVal(val["color"], &temp.color))
    return false;
  if (!readJsonVal(val["value"], &temp.value))
    temp.value = 1;
  if (!readJsonVal(val["lives"], &temp.lives))
    temp.lives = 1;
  if (!readJsonVal(val["speed"], &temp.speed))
    temp.speed = 0;
  if (!readJsonVal(val["stopHeading"], &temp.stopHeading))
    temp.stopHeading = -1;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, BrickRingSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  BrickRingSpec temp;
  if (!readJsonVal(val["elevation"], &temp.elevation) ||
      !readJsonVal(val["color"], &temp.color) ||
      !readJsonVal(val["count"], &temp.count) ||
      !readJsonVal(val["phase"], &temp.phase))
    return false;
  if (!readJsonVal(val["value"], &temp.value))
    temp.value = 1;
  if (!readJsonVal(val["lives"], &temp.lives))
    temp.lives = 1;
  if (!readJsonVal(val["speed"], &temp.speed))
    temp.speed = 0;
  if (!readJsonVal(val["stopHeading"], &temp.stopHeading))
    temp.stopHeading = -1;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, WallSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  WallSpec temp;
  if (!readJsonVal(val["elevation"], &temp.elevation) ||
      !readJsonVal(val["heading"], &temp.heading) ||
      !readJsonVal(val["size"], &temp.size))
    return false;
  if (!readJsonVal(val["isExit"], &temp.isExit))
    temp.isExit = false;
  if (!readJsonVal(val["speed"], &temp.speed))
    temp.speed = 0;
  if (!readJsonVal(val["stopHeading"], &temp.stopHeading))
    temp.stopHeading = -1;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, CurvedWallSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  CurvedWallSpec temp;
  if (!readJsonVal(val["elevation1"], &temp.elevation1) ||
      !readJsonVal(val["heading1"], &temp.heading1) ||
      !readJsonVal(val["elevation2"], &temp.elevation2) ||
      !readJsonVal(val["heading2"], &temp.heading2) ||
      !readJsonVal(val["width"], &temp.width))
    return false;
  if (!readJsonVal(val["isExit"], &temp.isExit))
    temp.isExit = false;
  if (!readJsonVal(val["speed"], &temp.speed))
    temp.speed = 0;
  if (!readJsonVal(val["stopHeading"], &temp.stopHeading))
    temp.stopHeading = -1;
  *result = temp;
  return true;
}

template<>
bool readJsonVal(const Json::Value& val, BallSpec* result) {
  if (!assertType(val, Json::objectValue))
    return false;
  BallSpec temp;
  if (!readJsonVal(val["elevation"], &temp.elevation) ||
      !readJsonVal(val["heading"], &temp.heading))
    return false;
  *result = temp;
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
