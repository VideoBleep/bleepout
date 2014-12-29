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
                         Optional<bool>* result) const {
  if (!assertType(val, Json::booleanValue))
    result->unset();
  else
    result->set(val.asBool());
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         Optional<float>* result) const {
  if (!assertType(val, Json::realValue))
    result->unset();
  else
    result->set(val.asFloat());
}

template<>
bool EnumTypeTraits<ofLogLevel>::parseString(const std::string &str, ofLogLevel *result, const ofLogLevel &defaultVal) {
  if (str == "Verbose")
    *result = OF_LOG_VERBOSE;
  else if (str == "Notice")
    *result = OF_LOG_NOTICE;
  else if (str == "Warning")
    *result = OF_LOG_WARNING;
  else if (str == "Error")
    *result = OF_LOG_ERROR;
  else if (str == "FatalError")
    *result = OF_LOG_FATAL_ERROR;
  else if (str == "Silent")
    *result = OF_LOG_SILENT;
  else {
    *result = defaultVal;
    return false;
  }
  return true;
}

template<>
bool EnumTypeTraits<ofLogLevel>::toString(const ofLogLevel &value, std::string* result) {
  switch (value) {
    case OF_LOG_VERBOSE:
      *result = "Verbose";
      break;
    case OF_LOG_NOTICE:
      *result = "Notice";
      break;
    case OF_LOG_WARNING:
      *result = "Warning";
      break;
    case OF_LOG_ERROR:
      *result = "Error";
      break;
    case OF_LOG_FATAL_ERROR:
      *result = "FatalError";
      break;
    case OF_LOG_SILENT:
      *result = "Silent";
      break;
    default:
      return false;
  }
  return true;
}

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
