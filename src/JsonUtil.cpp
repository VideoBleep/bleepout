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

static const Json::Value* getNumeric(const Json::Value& obj, const char* property) {
  if (!obj.isMember(property)) {
    ofLogError() << "property not found '" << property << "'";
    return NULL;
  }
  const Json::Value& val = obj[property];
  if (!val.isNumeric()) {
    ofLogError() << "invalid type for '" << property << "' "
                 << "(expected number): " << val.toStyledString();
  }
  return &val;
}

static const Json::Value* getTypedValue(const Json::Value& obj, const char* property, Json::ValueType type) {
  if (!obj.isMember(property)) {
    ofLogError() << "property not found '" << property << "'";
    return NULL;
  }
  const Json::Value& val = obj[property];
  if (val.type() != type) {
    ofLogError() << "invalid type for '" << property << "' "
                 << "(expected " << type << "): " << val.toStyledString();
  }
  return &val;
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
Json::Value toJsonObj(const ofVec2f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  return obj;
}

template<>
Json::Value toJsonObj(const ofVec3f& val) {
  Json::Value obj(Json::objectValue);
  obj["x"] = val.x;
  obj["y"] = val.y;
  obj["z"] = val.z;
  return obj;
}

template<>
Json::Value toJsonObj(const ofColor& val) {
  Json::Value obj(Json::objectValue);
  obj["r"] = val.r;
  obj["g"] = val.g;
  obj["b"] = val.b;
  return obj;
}

template<>
Json::Value toJsonObj(const std::vector<ofColor>& vals) {
  Json::Value arr(Json::arrayValue);
  arr.resize(vals.size());
  for (Json::ArrayIndex i = 0; i < vals.size(); i++) {
    arr[i] = toJsonObj(vals[i]);
  }
  return arr;
}

template<>
Json::Value toJsonObj(const BrickSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  obj["color"] = toJsonObj(spec.color);
  return obj;
}

template<>
Json::Value toJsonObj(const BrickRingSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["color"] = toJsonObj(spec.color);
  obj["count"] = spec.count;
  obj["phase"] = spec.phase;
  return obj;
}

template<>
Json::Value toJsonObj(const WallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  obj["size"] = toJsonObj(spec.size);
  obj["isExit"] = toJsonObj(spec.isExit);
  return obj;
}

template<>
Json::Value toJsonObj(const CurvedWallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation1"] = spec.elevation1;
  obj["heading1"] = spec.heading1;
  obj["elevation2"] = spec.elevation2;
  obj["heading2"] = spec.heading2;
  obj["width"] = toJsonObj(spec.width);
  obj["isExit"] = toJsonObj(spec.isExit);
  return obj;
}

template<>
Json::Value toJsonObj(const BallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  return obj;
}
