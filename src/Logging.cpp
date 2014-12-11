//
//  Logging.cpp
//  bleepout
//
//  Created by tekt on 12/10/14.
//
//

#include "Logging.h"
#include <ofMain.h>
#include <ofxBullet.h>

//std::ostream& operator<<(std::ostream& os, const b2Vec2& vec) {
//  return os << vec.x << "," << vec.y;
//}

std::ostream& operator<<(std::ostream& os, const GameObject& obj) {
  obj.output(os);
  return os;
}

static void outputObjectId(std::ostream& os, const GameObject* obj) {
  if (obj)
    os << obj->id();
  else
    os << "(none)";
}

void Brick::output(std::ostream &os) const {
//  auto pos = body->GetPosition();
//  os << "Brick{id:" << id() << ", pos:(" << pos << ")}";
}

void Paddle::output(std::ostream &os) const {
  // this shouldn't be necessary. getPosition should really be marked as const
  auto pos = const_cast<Paddle*>(this)->getPosition();
  os << "Paddle{id:" << id()
     << ", pos:(" << pos << ")"
     << ", player:";
  outputObjectId(os, _player);
  os << "}";
}

void Ball::output(std::ostream &os) const {
//  auto pos = body->GetPosition();
  os << "Ball{id:" << id()
//     << ", pos:(" << pos << ")"
     << ", player:";
  outputObjectId(os, _player);
  os << "}";
}

void Player::output(std::ostream &os) const {
  os << "Player{id:" << id()
     << ", score:" << score()
     << ", lives:" << lives()
     << ", paddle:";
  outputObjectId(os, paddle());
  os << "}";
}

void Wall::output(std::ostream &os) const {
  os << "Wall{id:" << id();
  if (isExit())
    os << ", exit";
  os << "}";
}

template<typename T>
static void outputObjectCollection(std::ostream& os,
                                   const char* label,
                                   const GameObjectCollection<T>& collection) {
  os << label << "{size:" << collection.size();
  if (!collection.empty()) {
    os << "\n";
    for (auto& obj : collection) {
      os << "  ";
      obj->output(os);
      os << "\n";
    }
  }
  os << "}" << std::endl;
}

void RoundState::output(std::ostream &os) const {
  os << "RoundState{\n";
  outputObjectCollection(os, "paddles:", _paddles);
  outputObjectCollection(os, "balls:", _balls);
  outputObjectCollection(os, "bricks:", _bricks);
  outputObjectCollection(os, "players:", _players);
  outputObjectCollection(os, "walls:", _walls);
  os << "}";
}

std::ostream& operator<<(std::ostream& os, const RoundState& state) {
  state.output(os);
  return os;
}

void OrbitalTrajectory::output(std::ostream &os) const {
  os << "OrbitalTrajectory{radius:" << getRadius()
     << ", squish:" << _squishFactor
     << ", speed:" << getSpeed()
     << ", pos:" << getPosition()
     << ", u:" << _u
     << ", w:" << _w
     << ", t:" << _t
     << "}";
}

std::ostream& operator<<(std::ostream& os, const OrbitalTrajectory& trajectory) {
  trajectory.output(os);
  return os;
}
