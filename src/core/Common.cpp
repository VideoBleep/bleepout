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
