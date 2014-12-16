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
  RoundMessage(string t, const ofColor& c, float sz = 50, int trail = 0)
  : text(t), color(c), size(sz), trails(trail) {}
  string text;
  ofColor color;
  float size;
  int trails;
};

class RoundState {
public:
  
  RoundState(const RoundConfig& config)
  : _config(config)
  , _liveBricks(0) {}
  
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

  void addPlayer(ofPtr<Player> player);
  Paddle& addPaddle(Player* player);
  Brick& addBrick(const BrickSpec& brickSpec);
  Wall& addWall(const WallSpec& wallSpec);
  Ball& addBall(const BallSpec& ballSpec);
  
  int decrementLiveBricks() {
    return ++_liveBricks;
  }
  
  int liveBricks() const { return _liveBricks; }
  
  void output(std::ostream& os) const;
  
  RoundMessage message;
  float time;
  
private:
  RoundState(const RoundState& other) : _config(other._config) {
    ofLogWarning() << "DUPLICATING ROUND STATE!!!";
  }
  const RoundConfig& _config;
  GameObjectCollection<Paddle> _paddles;
  GameObjectCollection<Ball>   _balls;
  GameObjectCollection<Brick>  _bricks;
  GameObjectCollection<Wall>   _walls;
  GameObjectCollection<Player> _players;
  int _liveBricks;
};

#endif /* defined(__bleepout__GameState__) */
