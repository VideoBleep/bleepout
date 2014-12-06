//
//  Brick.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Brick.h"


void Brick::output(std::ostream &os) const {
  auto pos = body->GetPosition();
  os << "Brick{id:" << id() << ", pos:(" << pos.x << "," << pos.y << ")}";
}
