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
#include <iostream>

class Player : public GameObject {
public:
  Player();
  ~Player() override;
private:
  void output(std::ostream& os) const override;
};

#endif /* defined(__bleepout__Player__) */
