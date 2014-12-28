//
//  ObjectSpecs.cpp
//  bleepout
//
//  Created by tekt on 12/17/14.
//
//

#include "ObjectSpecs.h"
#include "BleepoutConfig.h"

void BrickQuadClusterRingsSpec::addBricksTo(RoundConfig *config) const {
  config->addBrickRing()
    .setElevation(elevation - elevationSpacing)
    .setCount(count)
    .setColor(color1)
    .setPhase(-headingSpacing)
    .setSize(size)
    .setSpeed(speed)
    .setModifier(modifierName, modifierChance)
    .setValue(2).setLives(2);
  config->addBrickRing()
    .setElevation(elevation - elevationSpacing)
    .setCount(count)
    .setColor(color2)
    .setSize(size)
    .setSpeed(speed)
    .setModifier(modifierName, modifierChance)
    .setPhase(headingSpacing);
  config->addBrickRing()
    .setElevation(elevation + elevationSpacing)
    .setCount(count)
    .setColor(color2)
    .setSize(size)
    .setSpeed(speed)
    .setModifier(modifierName, modifierChance)
    .setPhase(-headingSpacing);
  config->addBrickRing()
    .setElevation(elevation + elevationSpacing)
    .setCount(count)
    .setColor(color1)
    .setPhase(headingSpacing)
    .setSize(size)
    .setSpeed(speed)
    .setModifier(modifierName, modifierChance)
    .setValue(2).setLives(2);
}
