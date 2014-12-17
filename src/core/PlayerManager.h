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

class RoundController;

struct PlayerYawPitchRollMessage {
	PlayerYawPitchRollMessage() {
	}

	PlayerYawPitchRollMessage(float y, float p, float r) {
		yaw = y;
		pitch = p;
		roll = r;
	}

	ofPtr<Player> player;
	float yaw;
	float pitch;
	float roll;
};

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
