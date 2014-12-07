//
//  GameEvents.h
//  bleepout
//
//  Created by tekt on 12/4/14.
//
//

#ifndef __bleepout__GameEvents__
#define __bleepout__GameEvents__

#include <ofTypes.h>
#include "GameObject.h"
#include "Player.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"

template<typename T>
class BallHitObjectEventArgs {
public:
  BallHitObjectEventArgs(Ball& ball, T& object)
  : _ball(ball), _object(object) { }

  Ball& ball() { return _ball; }
  T& object() { return _object; }
private:
  Ball& _ball;
  T& _object;
};

class PlayerEventArgs {
public:
  PlayerEventArgs(ofPtr<Player> player) : _player(player) { }
private:
  ofPtr<Player> _player;
};

typedef BallHitObjectEventArgs<Paddle> BallHitPaddleEventArgs;
typedef BallHitObjectEventArgs<Brick> BallHitBrickEventArgs;

class RoundEventSender {
public:
  ofEvent<BallHitPaddleEventArgs> ballHitPaddleEvent;
  ofEvent<BallHitBrickEventArgs> ballHitBrickEvent;
protected:
  void notifyBallHitPaddle(Ball& ball, Paddle& paddle);
  void notifyBallHitBrick(Ball& ball, Brick& brick);
};

#endif /* defined(__bleepout__GameEvents__) */
