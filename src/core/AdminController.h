//
//  AdminController.h
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#ifndef __bleepout__AdminController__
#define __bleepout__AdminController__

#include <ofxUI.h>
#include "BleepoutConfig.h"
#include "GameEvents.h"
#include <ofEvents.h>
#include <ofTypes.h>
#include <list>

struct AdminUIControls;
class BleepoutApp;
class SetupController;
class RoundController;
class RoundState;

class AdminController : public EventSource {
public:
  AdminController();
  virtual ~AdminController();
  
  ofEvent<StartRoundEventArgs> tryStartRoundEvent;
  ofEvent<EmptyEventArgs> playRoundEvent;
  ofEvent<EndRoundEventArgs> tryEndRoundEvent;
  
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  void attachTo(BleepoutApp& app);
  void detachFrom(BleepoutApp& app);
  
  const char* eventSourceName() const override { return "AdminController"; }
  
  std::list<ofPtr<Player> >& lobby() { return _lobby; }
  
  // Event Handlers
  void handlePlayerConnected(PlayerEventArgs& e);
private:
  void onUIEvent(ofxUIEventArgs& e);
  void onRoundStarted(RoundStateEventArgs& e);
  void onRoundPlay(RoundStateEventArgs& e);
  void onRoundEnded(RoundEndedEventArgs& e);
  bool tryStartRound();
  bool canStartRound();
  void tryEndRound();
  bool notifyTryStartRound(std::list<ofPtr<RoundConfig> > configs,
                           std::list<ofPtr<Player> > players);
  bool notifyTryEndRound();
  void notifyPlayRound();
  void reloadConfig();
  void addTestPlayer();
  
  BleepoutConfig& _appConfig;
  // Lobby is the list of players queued for the game
  std::list<ofPtr<Player> > _lobby;
  ofxUICanvas* _gui;
  RoundState* _roundState;
  AdminUIControls* _controls;
};

#endif /* defined(__bleepout__AdminController__) */
