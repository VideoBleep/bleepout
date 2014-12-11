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
#include "GameEvents.h"

std::ostream& operator<<(std::ostream& os, const GameObject& obj);
std::ostream& operator<<(std::ostream& os, const RoundState& state);


#endif /* defined(__bleepout__Logging__) */