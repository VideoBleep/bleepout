//
//  Logging.cpp
//  bleepout
//
//  Created by tekt on 12/10/14.
//
//

#include "Logging.h"
#include "GameEvents.h"
#include <ofMain.h>

std::ostream& operator<<(std::ostream& os, const BallSpec& spec) {
  os << "BallSpec{";
  os << "elevation: " << spec.elevation;
  os << ", heading: " << spec.heading;
  os << "}";
  return os;
}

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
  if (!obj.visible())
    os << ", invisible";
  if (!obj.physical())
    os << ", non-physical";
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
  os << ", modifier:" << (_modifierName.empty()? "()": ("'" + _modifierName + "'"));
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

void Paddle::output(std::ostream &os) const {
  os << "Paddle{";
  outputGameObjectFields(os, *this);
  os << ", player:" << player().id();
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
  outputObjectCollection(os, _modifiers);
  outputObjectCollection(os, _animations);
  os << "liveBricks: " << _liveBricks << std::endl;
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

std::ostream& operator<<(std::ostream& os, const GameObjectType& type) {
  return os << enumToString(type);
}

std::ostream& operator<<(std::ostream& os, const ModifierType& type) {
  return os << enumToString(type);
}

std::ostream& operator<<(std::ostream& os, const ModifierSpec& spec) {
  os << "(type:" << spec.type
    << ", duration:" << spec.duration
    << ", amount:" << spec.amount
    << ")";
  return os;
}

void PaddleWidthModifier::output(std::ostream &os) const {
  os << "PaddleWidthModifier{amount:" << amount();
  os << ", ";
  outputGameObjectFields(os, *this);
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

void ExtraLifeModifier::output(std::ostream &os) const {
  os << "ExtraLifeModifier{";
  outputGameObjectFields(os, *this);
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

void BallModifier::output(std::ostream &os) const {
  os << "BallModifier{";
  outputGameObjectFields(os, *this);
  os << ", type: " << modifierType();
  os << ", ";
  outputPhysicsObjectFields(os, *this);
  os << "}";
}

std::ostream& operator<<(std::ostream& os, const RoundEndReason& reason) {
  switch (reason) {
    case END_TIME_LIMIT:
      os << "TimeLimit";
      break;
    case END_NO_BALLS:
      os << "NoBalls";
      break;
    case END_NO_BRICKS:
      os << "NoBricks";
      break;
    case END_NO_PLAYERS:
      os << "NoPlayers";
      break;
    case END_ADMIN_OVERRIDE:
      os << "AdminOverride";
      break;
    default:
      os << "Unknown{" << (int)reason << "}";
      break;
  }
  return os;
}

void PlayerRoundResult::output(std::ostream &os) const {
  os << "PlayerResult{";
  os << "id: " << playerId << ", ";
  os << "score: " << score << ", ";
  os << "modifiers: (";
  bool first = true;
  for (const auto& entry : modifierCounts) {
    if (!first)
      os << ", ";
    else
      first = false;
    os << entry.first << ": " << entry.second;
  }
  os << ")}";
}

std::ostream& operator<<(std::ostream& os, const PlayerRoundResult& result) {
  result.output(os);
  return os;
}

void RoundResults::output(std::ostream &os) const {
  os << "RoundResults{";
  os << "reason: " << reason << ", ";
  os << "duration: " << std::setprecision(4) << std::fixed
                     << duration << "sec, ";
  os << "remainingBricks: " << liveBricks << "/" << totalBricks << ", ";
  os << "remainingBalls: " << liveBalls << "/" << totalBalls << ", ";
  os << "players: (";
  bool first = true;
  for (const auto& player : _playerResults) {
    if (!first)
      os << ", ";
    else
      first = false;
    os << player;
  }
  os << ")";
  os << "}";
}

std::ostream& operator<<(std::ostream& os, const RoundResults& results) {
  results.output(os);
  return os;
}
