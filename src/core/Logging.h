//
//  Logging.h
//  bleepout
//
//  Created by tekt on 12/10/14.
//
//

#ifndef __bleepout__Logging__
#define __bleepout__Logging__

#include <iostream>
#include "GameObject.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "Player.h"
#include "Wall.h"
#include "GameState.h"
#include "GameObjectCollection.h"
#include "PhysicsObject.h"
#include "OrbitalTrajectory.h"
#include "CircularTrajectory.h"
#include "RoundResults.h"

std::ostream& operator<<(std::ostream& os, const PhysicsObject& obj);
std::ostream& operator<<(std::ostream& os, const GameObject& obj);
std::ostream& operator<<(std::ostream& os, const RoundState& state);
std::ostream& operator<<(std::ostream& os, const Trajectory& trajectory);
std::ostream& operator<<(std::ostream& os, const BoundingBox& box);
std::ostream& operator<<(std::ostream& os, const CollisionShape& shape);
std::ostream& operator<<(std::ostream& os, const GameObjectType& type);
std::ostream& operator<<(std::ostream& os, const ModifierType& type);
std::ostream& operator<<(std::ostream& os, const ModifierSpec& spec);
std::ostream& operator<<(std::ostream& os, const PlayerRoundResult& result);
std::ostream& operator<<(std::ostream& os, const RoundResults& results);
std::ostream& operator<<(std::ostream& os, const RoundEndReason& reason);

template<typename T>
void outputField(std::ostream& os, const char* label, const T* obj) {
  os << label << ": ";
  if (obj)
    obj->output(os);
  else
    os << "(null)";
}

template<typename T>
void outputField(std::ostream& os, const T* obj) {
  outputField(os, GameObjectTypeTraits<T>::typeName, obj);
}

#endif /* defined(__bleepout__Logging__) */
