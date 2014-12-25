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

// Engine.io client packet type prefixes
const std::string PACKET_OPEN = "0";
const std::string PACKET_CLOSE = "1";
const std::string PACKET_PING = "2";
const std::string PACKET_PONG = "3";
const std::string PACKET_MESSAGE = "4";
const std::string PACKET_UPGRADE = "5";
const std::string PACKET_NOOP = "6";

// New player
const std::string MESSAGE_NEW = "new";
// Player start
const std::string MESSAGE_START = "sta";
// Yaw / Pitch / Roll 
const std::string MESSAGE_YPR = "ypr";
// Action 
const std::string MESSAGE_ACT = "act";

// Action message prefixes
const std::string ACTION_CONFIGURE = "cfg"; // set color
const std::string ACTION_CALIBRATE = "cal"; // set position offsets
const std::string ACTION_START = "start"; // player ready
const std::string ACTION_QUIT = "quit"; // quit game

// State message prefixes
const std::string STATE_COLOR = "col"; // player should select color
const std::string STATE_QUEUED = "que"; // player is queued, holding for game ready
const std::string STATE_CALIBRATION = "cal"; // player needs to calibrate
const std::string STATE_READY = "rdy"; // game is ready, awaiting player ready
const std::string STATE_PLAY = "play"; // game is playing, free to send control

class BleepoutApp; 

class PlayerManager : public EventSource {
public:
	//PlayerManager();
	PlayerManager(BleepoutApp& bleepoutApp, PlayerController& playerController);
	
  std::list<ofPtr<Player> >& players() { return _players; }

  // Sockets Server
  ofxLibwebsockets::Server server;
  PlayerController controller;

  void setup();
  void update();
  void draw();
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

  // Send messages
  // Send 'Select Color' state message to player
  void setPlayerColor(Player& player);
  // Send 'Queued' state message to player
  void setPlayerQueued(Player& player);
  // Send 'Calibrate' state message to player
  void setPlayerCalibrate(Player& player);
  // Send 'Ready' state message to player 
  void setPlayerReady(Player& player);
  // Send 'Play' message to player (player should send back "start" message I think, to tell balls to drop)
  void setPlayerPlay(Player& player);

  ofPtr<Player> findPlayer(ofxLibwebsockets::Connection& conn);

  /*
	Events
  */
  // Raised when player control message arrives
  ofEvent<PlayerYawPitchRollEventArgs> playerYawPitchRollEvent;
  
private:

  void notifyPlayerYawPitchRoll(Player* player, float yaw,
                                float pitch, float roll);
  
  bool _inRoundMode;

  BleepoutApp& _bleepoutApp;
  std::list<ofPtr<Player> > _players;
};

#endif /* defined(__bleepout__PlayerManager__) */
