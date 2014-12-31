//
//  PlayerManager.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__PlayerManager__
#define __bleepout__PlayerManager__

//#include "BleepoutApp.h"
#include "Player.h"
#include "PlayerController.h"
#include <ofMain.h>
#include "GameObjectCollection.h"
#include "GameState.h"
#include "ofxLibwebsockets.h"
#include "GameEvents.h"
#include <list>

class BleepoutApp; 

class PlayerManager : public EventSource {
public:
	PlayerManager(BleepoutApp& bleepoutApp, PlayerController& playerController);
  virtual ~PlayerManager() {}

	std::list<ofPtr<Player> >& players() { return _players; }

  // Sockets Server
  ofxLibwebsockets::Server server;
  PlayerController controller;

  void setup();
  void update();
  void gotMessage(ofMessage msg);
  
  ofPtr<Player> addPlayer();
  void setIsInRound(bool r) { _inRoundMode = r; }

  // Message queue (temporary?)
  ofTrueTypeFont font;
  vector<string> messages;

  // Websocket event handlers
  void onConnect(ofxLibwebsockets::Event& args);
  void onOpen(ofxLibwebsockets::Event& args);
  void onClose(ofxLibwebsockets::Event& args);
  void onIdle(ofxLibwebsockets::Event& args);
  void onMessage(ofxLibwebsockets::Event& args);
  void onBroadcast(ofxLibwebsockets::Event& args);

	/*
	SEND STATE MESSAGES TO PLAYER
	*/
  // NO LONGER USED - Send 'Select Color' state message to player
  static void setPlayerColor(Player& player);
  // Send 'Queued' state message to player
	static void setPlayerQueued(Player& player);
  // Send 'Calibrate' state message to player
  static void setPlayerCalibrate(Player& player, ofColor color);
  // Send 'Ready' state message to player 
	static void setPlayerReady(Player& player);
  // Send 'Play' message to player (player should send back "start" message I think, to tell balls to drop)
	static void setPlayerPlay(Player& player);

  ofPtr<Player> findPlayer(ofxLibwebsockets::Connection& conn);

  /*
	Events
  */
  // Raised when player control message arrives
  ofEvent<PlayerYawPitchRollEventArgs> playerYawPitchRollEvent;
  
  const char* eventSourceName() const override { return "PlayerManager"; }
  
private:

  void notifyPlayerYawPitchRoll(Player* player, float yaw,
                                float pitch, float roll);
  
  bool _inRoundMode;

	BleepoutApp& _bleepoutApp;
  std::list<ofPtr<Player> > _players;
};

#endif /* defined(__bleepout__PlayerManager__) */
