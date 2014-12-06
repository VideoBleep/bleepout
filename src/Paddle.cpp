//
//  Paddle.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Paddle.h"


void Paddle::output(std::ostream &os) const {
  auto pos = _rect.body->GetPosition();
  os << "Paddle{id:" << id() << ", pos:(" << pos.x << "," << pos.y << ")";
  os << ", player:" << _player.id() << "}";
}
