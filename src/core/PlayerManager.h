//
//  PlayerManager.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__PlayerManager__
#define __bleepout__PlayerManager__

#include "Player.h"
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
const std::string ACTION_CONFIGURE = "cfg";
const std::string ACTION_START = "start";
const std::string ACTION_CALIBRATE = "cal";
const std::string ACTION_QUIT = "quit";

class PlayerManager
: public PlayerEventSource
, public ControlEventSource {
public:
	PlayerManager();
	
  std::list<ofPtr<Player> >& players() { return _players; }

  ofPtr<Player> addPlayer();

  // Sockets Server
  ofxLibwebsockets::Server server;

  void setup();
  void update();
  void draw();
  void gotMessage(ofMessage msg);
  
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

  ofPtr<Player> findPlayer(ofxLibwebsockets::Connection& conn);

private:
  bool _inRoundMode;
  std::list<ofPtr<Player> > _players;
};

#endif /* defined(__bleepout__PlayerManager__) */
