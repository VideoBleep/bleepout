//
//  Logging.cpp
//  bleepout
//
//  Created by tekt on 12/10/14.
//
//

#include "Logging.h"
#include <ofMain.h>

void outputPhysicsObjectFields(std::ostream& os, const PhysicsObject& obj) {
  os << "position:(" << obj.getPosition() << ")"
  << ", size:(" << obj.getSize() << ")"
  << ", boundingBox:" << obj.getBoundingBox()
  << ", velocity:" << obj.getVelocity()
  << ", collisionShape:" << obj.collisionShape
  << ", trajectory:";
  if (obj.trajectory)
    os << (obj.trajectory.get());
  else
    os << "(none)";
}

std::ostream& operator<<(std::ostream& os, const PhysicsObject& obj) {
    obj.output(os);
    return os;
}

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

static void outputGameObjectFields(std::ostream& os, const GameObject& obj) {
  os << "id:" << obj.id();
  if (!obj.alive())
    os << ", dead";
}

void PhysicsObject::output(std::ostream& os) const {
    outputPhysicsObjectFields(os, *this);
}

void Brick::output(std::ostream &os) const {
  os << "Brick{";
  outputGameObjectFields(os, *this);
  os << ", value:" << value();
  os << ", lives:" << lives();
  if (maxLives() > 1)
    os << "/" << maxLives();
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

void Paddle::output(std::ostream &os) const {
  os << "Paddle{";
  outputGameObjectFields(os, *this);
  os << ", player:";
  outputObjectId(os, player());
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

void Ball::output(std::ostream &os) const {
  os << "Ball{";
  outputGameObjectFields(os, *this);
  os << ", player:";
  outputObjectId(os, player());
  os << ", ";
  outputPhysicsObjectFields(os, *this);
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
  os << "Wall{";
  outputGameObjectFields(os, *this);
  if (isExit())
    os << ", exit";
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

template<typename T>
static void outputObjectCollection(std::ostream& os,
                                   const GameObjectCollection<T>& collection) {
  os << GameObjectTypeTraits<T>::typeName << "s: {size:" << collection.size();
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
  outputObjectCollection(os, _paddles);
  outputObjectCollection(os, _balls);
  outputObjectCollection(os, _bricks);
  outputObjectCollection(os, _players);
  outputObjectCollection(os, _walls);
  os << "}";
}

std::ostream& operator<<(std::ostream& os, const RoundState& state) {
  state.output(os);
  return os;
}

void OrbitalTrajectory::output(std::ostream &os) const {
  os << "OrbitalTrajectory{radius:" << getRadius()
     << ", speed:" << getSpeed()
     << ", pos:" << getPosition()
     << ", u:" << _u
     << ", w:" << _w
     << ", t:" << _t
     << "}";
}

void CircularTrajectory::output(std::ostream &os) const {
    os << "OrbitalTrajectory{radius:" << getRadius()
    << ", speed:" << getSpeed()
    << ", pos:" << getPosition()
    << ", t:" << _t
    << "}";
}

std::ostream& operator<<(std::ostream& os, const Trajectory& trajectory) {
  trajectory.output(os);
  return os;
}

std::ostream& operator<<(std::ostream& os, const BoundingBox& box) {
  return os << "BoundingBox{center:" << box.center
            << ", hw:" << box.halfwidths << "}";
}

std::ostream& operator<<(std::ostream& os, const CollisionShape& shape) {
  switch (shape) {
    case CollisionBox:
      os << "CollisionBox";
      break;
    case CollisionSphere:
      os << "CollisionSphere";
      break;
    default:
      os << "unknown(" << (int)shape << ")";
      break;
  }
  return os;
}

void PaddleWidthModifier::output(std::ostream &os) const {
  os << "PaddleWidthModifier{amount:" << _amount;
  os << ", ";
  outputGameObjectFields(os, *this);
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

void ExtraLifeModifier::output(std::ostream &os) const {
  os << "ExtraLifeModifier{";
  os << ", ";
  outputGameObjectFields(os, *this);
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}
