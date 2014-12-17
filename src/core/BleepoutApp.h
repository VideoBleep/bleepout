//
//  BleepoutApp.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__BleepoutApp__
#define __bleepout__BleepoutApp__

#include <ofMain.h>
#include "PlayerManager.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "RendererBase.h"
#include "Common.h"
#include "SetupController.h"
#include "GameEvents.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphonClient.h>
#endif // ENABLE_SYPHON

class BleepoutApp : public ofBaseApp {
public:
  BleepoutApp();
  
  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(int key) override;
  void mousePressed(int x, int y, int button);
  void mouseMoved(int x, int y );
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button) override;
  const BleepoutConfig& config() const { return _config; }
  
  void dumpConfig(ofLogLevel level) const;
private:
  void onStartRound(StartRoundEventArgs& e);
  
  BleepoutConfig _config;
	ofPtr<PlayerManager> _playerManager;
  ofPtr<SetupController> _setupController;
  ofPtr<RoundController> _roundController;
#ifdef ENABLE_SYPHON
  ofxSyphonClient _syphonClient;
#endif // ENABLE_SYPHON
};

#endif /* defined(__bleepout__BleepoutApp__) */
