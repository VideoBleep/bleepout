//
//  Brick.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Brick.h"


Brick::Brick()
: GameObject(GAME_OBJECT_BRICK)
, PhysicsObject(CollisionBox)
, _value(1)
{
    thisGameObject = this;
}

void Brick::output(std::ostream &os) const {
  auto pos = getPosition();
  os << "Brick{id:" << id() << ", pos:" << pos << "}";
}
