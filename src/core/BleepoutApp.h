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
#include <ofTypes.h>
#include "PlayerManager.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"
#include "RendererBase.h"
#include "Common.h"
#include "SetupController.h"
#include "GameEvents.h"
#include "AdminController.h"
#include "AudioManager.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphonClient.h>
#endif // ENABLE_SYPHON

class BleepoutApp : public ofBaseApp, public EventSource {
public:
  BleepoutApp();
  
  ofEvent<RoundStateEventArgs> roundStartedEvent;
  ofEvent<EmptyEventArgs> roundEndedEvent;
  
  // oF interface methods
  void setup() override;
  void update() override;
  void draw() override;
  
  void keyPressed(int key) override;
  void mousePressed(int x, int y, int button);
  void mouseMoved(int x, int y );
  void mouseReleased(int x, int y, int button);
  void mouseDragged(int x, int y, int button) override;
  
  const char* eventSourceName() const override { return "BleepoutApp"; }

private:
  void onTryStartRound(StartRoundEventArgs& e);
  void onTryEndRound(EndRoundEventArgs& e);

  void notifyRoundStarted(RoundState& state);
  void notifyRoundEnded();
  void endRound();

  ofPtr<BleepoutConfig> _config;
  ofPtr<BleepoutParameters> _appParams;
  ofPtr<PlayerManager> _playerManager;
  ofPtr<PlayerController> _playerController;
  ofPtr<SetupController> _setupController;
  ofPtr<RoundController> _roundController;
  ofPtr<AdminController> _adminController;
  ofPtr<AudioManager> _audioManager;
#ifdef ENABLE_SYPHON
  ofxSyphonClient _syphonClient;
#endif // ENABLE_SYPHON
  bool _syphonEnabled;
  bool _endingRound;
};

#endif /* defined(__bleepout__BleepoutApp__) */
