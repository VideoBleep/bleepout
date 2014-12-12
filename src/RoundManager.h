//
//  RoundManager.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__RoundController__
#define __bleepout__RoundController__

#include <ofMain.h>
#include <vector>
#include "PlayerManager.h"
#include "BleepoutConfig.h"
#include "GameState.h"
#include "SpaceController.h"
#include "LogicController.h"
#include "GameEvents.h"

class RendererBase;

class RoundController
{
public:
  explicit RoundController(RoundConfig config);
  
  ~RoundController();
  
  void setup();
  void draw();
  void update();
  
  RoundState& state() { return _state; }
  const RoundState& state() const { return _state; }
  
  void dumpToLog(ofLogLevel level);
  
  void keyPressed(int key);
  void mousePressed(int x, int y, int button);
  void mouseMoved(int x, int y );
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button);
  
  void setPaddlePosition(GameObjectId playerId, float xPercent);
  void setPaddlePosition(PlayerYawPitchRollMessage ypr);

private:
  RoundConfig _config;
  RoundState _state;
  ofPtr<RendererBase> _renderer;
  ofPtr<SpaceController> _spaceController;
  ofPtr<LogicController> _logicController;
};

#endif /* defined(__bleepout__RoundController__) */
