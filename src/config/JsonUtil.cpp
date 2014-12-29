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

template<>
Json::Value toJsonVal(const ofVec3f& val) {
  Json::Value obj(Json::arrayValue);
  obj[0u] = val.x;
  obj[1] = val.y;
  obj[2] = val.z;
  return obj;
}

template<>
Json::Value toJsonVal(const ofColor& val) {
  Json::Value obj(Json::arrayValue);
  obj[0u] = val.r;
  obj[1] = val.g;
  obj[2] = val.b;
  if (val.a != ofColor::limit()) {
    obj[3] = val.a;
  }
  return obj;
}

template<>
Json::Value toJsonVal(const float& val) {
  return Json::Value(val);
}

template<>
Json::Value toJsonVal(const bool& val) {
  return Json::Value(val);
}

template<>
Json::Value toJsonVal(const int& val) {
  return Json::Value(val);
}

template<>
Json::Value toJsonVal(const unsigned char& val) {
  return Json::Value((int)val);
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

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ofVec3f *result,
                         const ofVec3f& defaultVal) const {
  if (val.isArray()) {
    readVal(val[0u], &result->x, defaultVal.x);
    readVal(val[1], &result->y, defaultVal.y);
    readVal(val[2], &result->z, defaultVal.z);
  } if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    readVal(val["x"], &result->x, defaultVal.x);
    readVal(val["y"], &result->y, defaultVal.y);
    readVal(val["z"], &result->z, defaultVal.z);
  }
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ofColor *result,
                         const ofColor& defaultVal) const {
  if (val.isArray()) {
    readVal(val[0u], &result->r, defaultVal.r);
    readVal(val[1], &result->g, defaultVal.g);
    readVal(val[2], &result->b, defaultVal.b);
    readVal(val[3], &result->a, defaultVal.a);
  } else if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    readVal(val["r"], &result->r, defaultVal.r);
    readVal(val["g"], &result->g, defaultVal.g);
    readVal(val["b"], &result->b, defaultVal.b);
    readVal(val["a"], &result->a, defaultVal.a);
  }
}
