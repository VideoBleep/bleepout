//
//  Brick.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Brick.h"


void Brick::output(std::ostream &os) const {
  auto pos = getPosition();
  os << "Brick{id:" << id() << ", pos:" << pos << "}";
}
