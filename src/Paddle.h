//
//  Paddle.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Paddle__
#define __bleepout__Paddle__

#include "Player.h"
#include "GameObject.h"
#include <ofTypes.h>
#include <ofxBullet.h>

class Paddle : public GameObject, public ofxBulletBox {
public:
  Paddle(ofPtr<Player> player);
  ~Paddle() override;
  
  ofPtr<Player> player() { return _player; }
  
  void output(std::ostream& os) const override;
private:
  static btBoxShape collisionShape;
  ofPtr<Player> _player;
};


#endif /* defined(__bleepout__Paddle__) */
