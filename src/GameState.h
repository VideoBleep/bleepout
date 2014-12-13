//
//  GameState.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__GameState__
#define __bleepout__GameState__

#include <iostream>

#include "GameObject.h"
#include "Paddle.H"
#include "Brick.h"
#include "Ball.h"
#include "Player.h"
#include "Wall.h"
#include "GameObjectCollection.h"

struct RoundMessage {
    RoundMessage() {}
    RoundMessage(string t, const ofColor& c, float sz) : text(t), color(c), size(sz) {}
    string text;
    ofColor color;
    float size;
};

class RoundState {
public:

  const GameObjectCollection<Paddle>& paddles() const { return _paddles; }
  const GameObjectCollection<Ball>& balls() const { return _balls; }
  const GameObjectCollection<Brick>& bricks() const { return _bricks; }
  const GameObjectCollection<Wall>& walls() const { return _walls; }
  const GameObjectCollection<Player>& players() const { return _players; }

  GameObjectCollection<Paddle>& paddles() { return _paddles; }
  GameObjectCollection<Ball>& balls() { return _balls; }
  GameObjectCollection<Brick>& bricks() { return _bricks; }
  GameObjectCollection<Wall>& walls() { return _walls; }
  GameObjectCollection<Player>& players() { return _players; }

  void output(std::ostream& os) const;
    
  RoundMessage message;
  float time;

private:
  GameObjectCollection<Paddle> _paddles;
  GameObjectCollection<Ball>   _balls;
  GameObjectCollection<Brick>  _bricks;
  GameObjectCollection<Wall>   _walls;
  GameObjectCollection<Player> _players;
};

#endif /* defined(__bleepout__GameState__) */
