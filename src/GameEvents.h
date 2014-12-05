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
  BallHitObjectEventArgs(ofPtr<Ball> ball, ofPtr<T> object)
  : _ball(ball), _object(object) { }

  ofPtr<Ball> ball() { return _ball; }
  ofPtr<T> object() { return _object; }
private:
  ofPtr<Ball> _ball;
  ofPtr<T> _object;
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
  void notifyBallHitPaddle(ofPtr<Ball> ball, ofPtr<Paddle> paddle) {
    BallHitPaddleEventArgs e(ball, paddle);
    ofNotifyEvent(ballHitPaddleEvent, e);
  }
  void notifyBallHitBrick(ofPtr<Ball> ball, ofPtr<Brick> brick) {
    BallHitBrickEventArgs e(ball, brick);
    ofNotifyEvent(ballHitBrickEvent, e);
  }
};

#endif /* defined(__bleepout__GameEvents__) */
