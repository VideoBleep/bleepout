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
