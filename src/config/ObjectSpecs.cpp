//
//  ObjectSpecs.cpp
//  bleepout
//
//  Created by tekt on 12/17/14.
//
//

#include "ObjectSpecs.h"
#include "BleepoutConfig.h"

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
                    .setStopHeading(stopHeading < 0 ? -1 : (heading + stopHeading)));
  }

}
