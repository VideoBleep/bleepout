//
//  Common.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "Common.h"
#include "JsonUtil.h"

template<>
ofVec2f getInterpolated(const ofVec2f& a, const ofVec2f& b, float amount) {
  return a.getInterpolated(b, amount);
}
template<>
ofVec3f getInterpolated(const ofVec3f& a, const ofVec3f& b, float amount) {
  return a.getInterpolated(b, amount);
}
template<>
ofColor getInterpolated(const ofColor& a, const ofColor& b, float amount) {
  return a.getLerped(b, amount);
}
template<>
float getInterpolated(const float& a, const float& b, float amount) {
  return ofLerp(a, b, amount);
}

std::ostream& operator<<(std::ostream& os, const Outputable& obj) {
  obj.output(os);
  return os;
}

//template<typename T>
//Json::Value toJsonVal(const Optional<T>& val) {
//  T v;
//  if (val.tryGet(&v, NULL))
//    return toJsonVal(v);
//  return Json::Value::null;
//}

template<>
Json::Value toJsonVal(const Optional<bool>& val) {
  bool v;
  if (val.tryGet(&v, NULL))
    return toJsonVal(v);
  return Json::Value::null;
}

template<>
Json::Value toJsonVal(const Optional<float>& val) {
  float v;
  if (val.tryGet(&v, NULL))
    return toJsonVal(v);
  return Json::Value::null;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         Optional<bool>* result,
                         const Optional<bool>& defaultVal) const {
  if (!assertType(val, Json::booleanValue))
    result->unset();
  else
    result->set(val.asBool());
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         Optional<float>* result,
                         const Optional<float>& defaultVal) const {
  if (!assertType(val, Json::realValue))
    result->unset();
  else
    result->set(val.asFloat());
}

#define FOR_EACH_LOG_LEVEL(X) \
  X(OF_LOG_VERBOSE, "Verbose") \
  X(OF_LOG_NOTICE, "Notice") \
  X(OF_LOG_WARNING, "Warning") \
  X(OF_LOG_ERROR, "Error") \
  X(OF_LOG_FATAL_ERROR, "FatalError") \
  X(OF_LOG_SILENT, "Silent")

template<>
bool EnumTypeTraits<ofLogLevel>::parseString(const std::string &str, ofLogLevel *result, const ofLogLevel &defaultVal) {
  FOR_EACH_LOG_LEVEL(ENUM_PARSE_CASE)
  *result = defaultVal;
  return false;
}

template<>
bool EnumTypeTraits<ofLogLevel>::toString(const ofLogLevel &value, std::string* result) {
  switch (value) {
    FOR_EACH_LOG_LEVEL(ENUM_TOSTR_CASE)
    default:
      return false;
  }
  return true;
}

#undef FOR_EACH_LOG_LEVEL

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ofLogLevel *result,
                         const ofLogLevel &defaultVal) const {
  if (!assertType(val, Json::stringValue)) {
    *result = defaultVal;
  } else {
    parseEnumString(val.asString(), result);
  }
}

template<>
Json::Value toJsonVal(const ofLogLevel& type) {
  std::string result;
  if (!enumToString(type, &result))
    return Json::Value::null;
  return Json::Value(result);
}
