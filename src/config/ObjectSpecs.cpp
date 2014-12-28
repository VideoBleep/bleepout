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
  }
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

