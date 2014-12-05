//
//  Player.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Player__
#define __bleepout__Player__

#include "GameObject.h"


class Player : public GameObject {
public:
  Player() : GameObject(GAME_OBJECT_PLAYER) {}
private:
};

#endif /* defined(__bleepout__Player__) */
