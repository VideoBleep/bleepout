//
//  Modifier.cpp
//  bleepout
//
//  Created by tekt on 12/15/14.
//
//

#include "Modifier.h"
#include "Player.h"
#include "Paddle.h"

const char GameObjectTypeTraits<Modifier>::typeName[] = "modifier";

Modifier* Modifier::createModifier(const ModifierSpec &spec) {
  if (spec.type == MODIFIER_NONE)
    return NULL;
  switch (spec.type) {
    case MODIFIER_EXTRA_LIFE:
      return new ExtraLifeModifier();
    case MODIFIER_PADDLE_WIDTH:
      return new PaddleWidthModifier(&spec);
    default:
      return NULL;
  }
}
