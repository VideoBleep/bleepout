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
#include <map>
#include <vector>
#include "GameObjectCollection.h"
#include "GameState.h"
#include "ofxLibwebsockets.h"

class RoundController;

class PlayerManager {
public:
	explicit PlayerManager(ofPtr<RoundController> roundController);
	
  RoundState& state() { return _state; }

  ofPtr<Player> addPlayer();

  // Sockets Server
  ofxLibwebsockets::Server server;

  void setup();
  void update();
  void draw();
  void gotMessage(ofMessage msg);

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

private:
  RoundState _state;
  ofPtr<RoundController> _roundController;
};

#endif /* defined(__bleepout__PlayerManager__) */
