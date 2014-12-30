//
//  ObjectSpecs.cpp
//  bleepout
//
//  Created by tekt on 12/17/14.
//
//

#include "ObjectSpecs.h"
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"
#include "JsonUtil.h"

#define FOR_EACH_SPEC_TYPE(X) \
  X(SPEC_BRICK, "Brick") \
  X(SPEC_BRICK_RING, "BrickRing") \
  X(SPEC_BRICK_QUADS, "BrickQuads") \
  X(SPEC_WALL, "Wall") \
  X(SPEC_WALL_RING, "WallRing") \
  X(SPEC_CURVED_BRICK_COLUMN, "CurvedBrickColumn") \
  X(SPEC_CURVED_WALL, "CurvedWall") \
  X(SPEC_BALL, "Ball") \
  X(SPEC_MODIFIER, "Modifier") \
  X(SPEC_MESSAGE, "Message") \
  X(SPEC_RING_SET, "RingSet")

template<>
bool EnumTypeTraits<SpecType>::parseString(const std::string &str, SpecType *result, const SpecType &defaultVal) {
  FOR_EACH_SPEC_TYPE(ENUM_PARSE_CASE)
  *result = defaultVal;
  return false;
}

template<>
bool EnumTypeTraits<SpecType>::toString(const SpecType &value, std::string* result) {
  switch (value) {
    FOR_EACH_SPEC_TYPE(ENUM_TOSTR_CASE)
    default:
      return false;
  }
  return true;
}

#undef FOR_EACH_SPEC_TYPE

template<>
void JsonLoader::readVal(const Json::Value &val,
                         SpecType *result,
                         const SpecType &defaultVal) const {
  if (!assertType(val, Json::stringValue)) {
    *result = defaultVal;
  } else {
    parseEnumString(val.asString(), result);
  }
}

template<>
Json::Value toJsonVal(const SpecType& type) {
  std::string result;
  if (!enumToString(type, &result))
    return Json::Value::null;
  return Json::Value(result);
}

void BrickRingSpec::buildSpecs(const RoundConfig &config, std::vector<BrickSpec> *specs) const {
  BrickSpec prototype = BrickSpec()
    .setElevation(elevation)
    .setSize(size)
    .setColor(color)
    .setValue(value)
    .setLives(lives)
    .setSpeed(speed);
  for (int i = 0; i < count; i++) {
    float heading = i * 360 / (count * 1.0) + phase;
    BrickSpec brick = BrickSpec()
      .copyFrom(prototype)
      .setHeading(heading)
      .setStopHeading(stopHeading < 0 ? -1 : (heading + stopHeading));
    if (!modifierName.empty()) {
      if (modifierChance >= 1 || ofRandomuf() >= modifierChance) {
        brick.setModifier(modifierName);
      }
    }
    specs->push_back(brick);
  }
}

void BrickQuadsSpec::buildSpecs(const RoundConfig &config, std::vector<BrickSpec> *specs) const {
  BrickRingSpec ring;
  ring.setSize(size)
    .setSpeed(speed)
    .setModifier(modifierName, modifierChance)
    .setCount(count)
    .setStopHeading(stopHeading);
  ring.setElevation(elevation - elevationSpacing)
    .setColor(color1)
    .setPhase(-headingSpacing)
    .setValue(2).setLives(2);
  ring.buildSpecs(config, specs);
  ring.setElevation(elevation - elevationSpacing)
    .setColor(color2)
    .setPhase(headingSpacing)
    .setValue(1).setLives(1);
  ring.buildSpecs(config, specs);
  ring.setElevation(elevation + elevationSpacing)
    .setColor(color1)
    .setPhase(-headingSpacing)
    .setValue(2).setLives(2);
  ring.buildSpecs(config, specs);
  ring.setElevation(elevation + elevationSpacing)
    .setColor(color2)
    .setPhase(headingSpacing)
    .setValue(1).setLives(1);
  ring.buildSpecs(config, specs);
}

void WallRingSpec::buildSpecs(const RoundConfig &config,
                              std::vector<WallSpec> *specs) const {
  WallSpec prototype = WallSpec()
    .setElevation(elevation)
    .setSize(size)
    .setIsExit(isExit)
    .setSpeed(speed)
    .setVisible(visible);
  for (int i = 0; i < count; i++) {
    float heading = i * 360 / (count * 1.0) + phase;
    specs->push_back(WallSpec()
                    .copyFrom(prototype)
                    .setHeading(heading)
                    .setStopHeading(stopHeading < 0 ? -1
                                    : (heading + stopHeading)));
  }

}

void CurvedWallSpec::buildSpecs(const RoundConfig &config, std::vector<WallSpec> *specs) const {
  const auto& appParams = BleepoutParameters::get();
  float r = appParams.domeRadius + appParams.domeMargin;
  float theta = elevation1;
  float phi = heading1;
  float dtheta = elevation2 - elevation1;
  float dphi = heading2 - heading1;
  int steps = floor(max((r * dtheta * PI/180.0) / width, (r * dphi * PI/180.0) / width));
  dtheta /= steps * 1.0;
  dphi /= steps * 1.0;
  WallSpec prototype = WallSpec()
    .setSize(ofVec3f(width))
    .setIsExit(isExit)
    .setSpeed(speed)
    .setVisible(false);
  for (int i = 0; i < steps; i++) {
    specs->push_back(WallSpec()
                    .copyFrom(prototype)
                    .setElevation(theta)
                    .setHeading(phi)
                    .setStopHeading(stopHeading < 0 ? -1
                                    : (phi + stopHeading)));
    theta += dtheta;
    phi += dphi;
  }
}

static void applyBrickColumnStripeProperties(const CurvedBrickColumnSpec::StripeSpec& stripe, BrickSpec& brick) {
  brick.setValue(stripe.value)
       .setLives(stripe.lives);
  if (!stripe.modifierName.empty()) {
    if (stripe.modifierChance >= 1 ||
        ofRandomuf() >= stripe.modifierChance) {
      brick.setModifier(stripe.modifierName);
    }
  }
}

void CurvedBrickColumnSpec::buildSpecs(const RoundConfig &config, std::vector<BrickSpec> *specs) const {
  const auto& appParams = BleepoutParameters::get();
  float r = appParams.domeRadius + appParams.domeMargin;
  float theta = elevation1;
  float phi = heading1;
  float dtheta = elevation2 - elevation1;
  float dphi = heading2 - heading1;
  dtheta /= (float)count;
  dphi /= (float)count;
  BrickSpec prototype = BrickSpec()
    .setSize(size)
    .setSpeed(speed);
  for (int row = 0; row < count; row++) {
    float percentage = row / (float)(count - 1);
    BrickSpec brick = BrickSpec()
      .copyFrom(prototype)
      .setElevation(theta)
      .setHeading(phi)
      .setStopHeading(stopHeading < 0 ? -1
                      : (phi + stopHeading))
      .setColor(color1.getLerped(color2, percentage));
    if (row % 3 == 0)
      applyBrickColumnStripeProperties(stripe1, brick);
    else if (row % 3 == 1)
      applyBrickColumnStripeProperties(stripe2, brick);
    else //if (row % 3 == 2)
      applyBrickColumnStripeProperties(stripe3, brick);
    specs->push_back(brick);
    theta += dtheta;
    phi += dphi;
  }
}

#define R_JPROP(property) readVal(val[#property], &result->property, defaultVal.property)

template<>
void JsonLoader::readVal(const Json::Value &val,
                         BrickSpec *result,
                         const BrickSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    result->copyFrom(defaultVal);
  } else {
    R_JPROP(elevation);
    R_JPROP(heading);
    R_JPROP(size);
    R_JPROP(value);
    R_JPROP(lives);
    R_JPROP(color);
    R_JPROP(speed);
    R_JPROP(stopHeading);
    R_JPROP(modifierName);
  }
}

template<>
Json::Value toJsonVal(const BrickSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_BRICK);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  obj["size"] = toJsonVal(spec.size);
  obj["value"] = spec.value;
  obj["lives"] = spec.lives;
  obj["color"] = toJsonVal(spec.color);
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  obj["modifierName"] = spec.modifierName;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         BrickRingSpec *result,
                         const BrickRingSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    result->copyFrom(defaultVal);
  } else {
    R_JPROP(elevation);
    R_JPROP(size);
    R_JPROP(color);
    R_JPROP(value);
    R_JPROP(lives);
    R_JPROP(count);
    R_JPROP(phase);
    R_JPROP(speed);
    R_JPROP(stopHeading);
    R_JPROP(modifierName);
    R_JPROP(modifierChance);
  }
}

template<>
Json::Value toJsonVal(const BrickRingSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_BRICK_RING);
  obj["elevation"] = spec.elevation;
  obj["size"] = toJsonVal(spec.size);
  obj["color"] = toJsonVal(spec.color);
  obj["value"] = spec.value;
  obj["lives"] = spec.lives;
  obj["count"] = spec.count;
  obj["phase"] = spec.phase;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  obj["modifierName"] = spec.modifierName;
  obj["modifierChance"] = spec.modifierChance;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         BrickQuadsSpec *result,
                         const BrickQuadsSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    result->copyFrom(defaultVal);
  } else {
    R_JPROP(elevation);
    R_JPROP(color1);
    R_JPROP(color2);
    R_JPROP(count);
    R_JPROP(elevationSpacing);
    R_JPROP(headingSpacing);
    R_JPROP(size);
    R_JPROP(speed);
    R_JPROP(stopHeading);
    R_JPROP(modifierName);
    R_JPROP(modifierChance);
  }
}

template<>
Json::Value toJsonVal(const BrickQuadsSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_BRICK_QUADS);
  obj["elevation"] = spec.elevation;
  obj["color1"] = toJsonVal(spec.color1);
  obj["color2"] = toJsonVal(spec.color2);
  obj["count"] = spec.count;
  obj["elevationSpacing"] = spec.elevationSpacing;
  obj["headingSpacing"] = spec.headingSpacing;
  obj["size"] = toJsonVal(spec.size);
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  obj["modifierName"] = spec.modifierName;
  obj["modifierChance"] = spec.modifierChance;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         WallSpec *result,
                         const WallSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    result->copyFrom(defaultVal);
  } else {
    R_JPROP(elevation);
    R_JPROP(heading);
    R_JPROP(size);
    R_JPROP(isExit);
    R_JPROP(speed);
    R_JPROP(stopHeading);
    R_JPROP(visible);
  }
}

template<>
Json::Value toJsonVal(const WallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_WALL);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  obj["size"] = toJsonVal(spec.size);
  if (spec.isExit)
    obj["isExit"] = spec.isExit;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  obj["visible"] = spec.visible;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         WallRingSpec *result,
                         const WallRingSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    result->copyFrom(defaultVal);
  } else {
    R_JPROP(elevation);
    R_JPROP(size);
    R_JPROP(isExit);
    R_JPROP(count);
    R_JPROP(phase);
    R_JPROP(speed);
    R_JPROP(stopHeading);
    R_JPROP(visible);
  }
}

template<>
Json::Value toJsonVal(const WallRingSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_WALL_RING);
  obj["elevation"] = spec.elevation;
  obj["size"] = toJsonVal(spec.size);
  if (spec.isExit)
    obj["isExit"] = spec.isExit;
  obj["count"] = spec.count;
  obj["phase"] = spec.phase;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  obj["visible"] = spec.visible;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         CurvedBrickColumnSpec::StripeSpec *result,
                         const CurvedBrickColumnSpec::StripeSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(value);
    R_JPROP(lives);
    R_JPROP(modifierName);
    R_JPROP(modifierChance);
  }
}

template<>
Json::Value toJsonVal(const CurvedBrickColumnSpec::StripeSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["value"] = spec.value;
  obj["lives"] = spec.lives;
  obj["modifierName"] = spec.modifierName;
  obj["modifierChance"] = spec.modifierChance;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         CurvedBrickColumnSpec *result,
                         const CurvedBrickColumnSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    result->copyFrom(defaultVal);
  } else {
    R_JPROP(elevation1);
    R_JPROP(heading1);
    R_JPROP(elevation2);
    R_JPROP(heading2);
    R_JPROP(color1);
    R_JPROP(color2);
    R_JPROP(size);
    R_JPROP(stripe1);
    R_JPROP(stripe2);
    R_JPROP(stripe3);
    R_JPROP(count);
    R_JPROP(phase);
    R_JPROP(speed);
    R_JPROP(stopHeading);
  }
}

template<>
Json::Value toJsonVal(const CurvedBrickColumnSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_CURVED_BRICK_COLUMN);
  obj["elevation1"] = spec.elevation1;
  obj["heading1"] = spec.heading1;
  obj["elevation2"] = spec.elevation2;
  obj["heading2"] = spec.heading2;
  obj["color1"] = toJsonVal(spec.color1);
  obj["color2"] = toJsonVal(spec.color2);
  obj["size"] = toJsonVal(spec.size);
  obj["stripe1"] = toJsonVal(spec.stripe1);
  obj["stripe2"] = toJsonVal(spec.stripe2);
  obj["stripe3"] = toJsonVal(spec.stripe3);
  obj["count"] = spec.count;
  obj["phase"] = spec.phase;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         CurvedWallSpec *result,
                         const CurvedWallSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(elevation1);
    R_JPROP(heading1);
    R_JPROP(elevation2);
    R_JPROP(heading2);
    R_JPROP(width);
    R_JPROP(isExit);
    R_JPROP(speed);
    R_JPROP(stopHeading);
  }
}

template<>
Json::Value toJsonVal(const CurvedWallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_CURVED_WALL);
  obj["elevation1"] = spec.elevation1;
  obj["heading1"] = spec.heading1;
  obj["elevation2"] = spec.elevation2;
  obj["heading2"] = spec.heading2;
  obj["width"] = toJsonVal(spec.width);
  if (spec.isExit)
    obj["isExit"] = spec.isExit;
  obj["speed"] = spec.speed;
  obj["stopHeading"] = spec.stopHeading;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         BallSpec *result,
                         const BallSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(elevation);
    R_JPROP(heading);
  }
}

template<>
Json::Value toJsonVal(const BallSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_BALL);
  obj["elevation"] = spec.elevation;
  obj["heading"] = spec.heading;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ModifierSpec *result,
                         const ModifierSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(type);
    R_JPROP(name);
    R_JPROP(amount);
    R_JPROP(duration);
    R_JPROP(color);
    R_JPROP(beneficial);
  }
}

template<>
Json::Value toJsonVal(const ModifierSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_MODIFIER);
  obj["name"] = spec.name;
  obj["type"] = toJsonVal(spec.type);
  obj["amount"] = spec.amount;
  obj["duration"] = spec.duration;
  obj["color"] = toJsonVal(spec.color);
  obj["beneficial"] = spec.beneficial;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         MessageSpec *result,
                         const MessageSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(text);
    R_JPROP(color);
    R_JPROP(size);
    R_JPROP(trails);
    R_JPROP(delay);
    R_JPROP(duration);
  }
}

template<>
Json::Value toJsonVal(const MessageSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_MESSAGE);
  obj["text"] = spec.text;
  obj["color"] = toJsonVal(spec.color);
  obj["size"] = spec.size;
  obj["trails"] = spec.trails;
  obj["delay"] = spec.delay;
  obj["duration"] = spec.duration;
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ValuePulserSpec<float> *result,
                         const ValuePulserSpec<float>& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(minRate);
    R_JPROP(maxRate);
    R_JPROP(changeInterval);
    R_JPROP(startValue);
  }
}

template<>
Json::Value toJsonVal(const ValuePulserSpec<float>& spec) {
  Json::Value obj(Json::objectValue);
  obj["minRate"] = toJsonVal(spec.minRate);
  obj["maxRate"] = toJsonVal(spec.maxRate);
  obj["changeInterval"] = toJsonVal(spec.changeInterval);
  obj["startValue"] = toJsonVal(spec.startValue);
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         ValuePulserSpec<ofVec3f> *result,
                         const ValuePulserSpec<ofVec3f>& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(minRate);
    R_JPROP(maxRate);
    R_JPROP(changeInterval);
    R_JPROP(startValue);
  }
}

template<>
Json::Value toJsonVal(const ValuePulserSpec<ofVec3f>& spec) {
  Json::Value obj(Json::objectValue);
  obj["minRate"] = toJsonVal(spec.minRate);
  obj["maxRate"] = toJsonVal(spec.maxRate);
  obj["changeInterval"] = toJsonVal(spec.changeInterval);
  obj["startValue"] = toJsonVal(spec.startValue);
  return obj;
}

template<>
void JsonLoader::readVal(const Json::Value &val,
                         RingSetSpec *result,
                         const RingSetSpec& defaultVal) const {
  if (!assertType(val, Json::objectValue)) {
    *result = defaultVal;
  } else {
    R_JPROP(spin);
    R_JPROP(spread);
    R_JPROP(spreadOffset);
    R_JPROP(count);
    R_JPROP(radiusScale);
    R_JPROP(lineWidth);
    R_JPROP(color);
  }
}

template<>
Json::Value toJsonVal(const RingSetSpec& spec) {
  Json::Value obj(Json::objectValue);
  obj["_type"] = toJsonVal(SPEC_RING_SET);
  obj["spin"] = toJsonVal(spec.spin);
  obj["spread"] = toJsonVal(spec.spread);
  obj["spreadOffset"] = toJsonVal(spec.spreadOffset);
  obj["count"] = spec.count;
  obj["radiusScale"] = spec.radiusScale;
  obj["lineWidth"] = spec.lineWidth;
  obj["color"] = toJsonVal(spec.color);
  return obj;
}


