//
//  PlayerManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//  
//

#include "PlayerManager.h"
#include "Logging.h"

// Engine.io client packet type prefixes
const std::string PACKET_OPEN = "0";
const std::string PACKET_CLOSE = "1";
const std::string PACKET_PING = "2";
const std::string PACKET_PONG = "3";
const std::string PACKET_MESSAGE = "4";
const std::string PACKET_UPGRADE = "5";
const std::string PACKET_NOOP = "6";

// Client-to-Server websocket message prefixes
const std::string MESSAGE_NEW = "new"; // New player
const std::string MESSAGE_START = "sta"; // Player start
const std::string MESSAGE_YPR = "ypr"; // Yaw / Pitch / Roll 
const std::string MESSAGE_ACT = "act"; // Action 

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

std::string messageDelimiter = "|";

PlayerManager::PlayerManager(BleepoutApp& bleepoutApp, PlayerController& playerController)
	: _bleepoutApp(bleepoutApp), 
	controller(playerController)
	{ }

ofPtr<Player> PlayerManager::addPlayer() {
  ofPtr<Player> player(new Player());
  player->setColor(ofColor::green);
  _players.push_back(player);
  return player;
}

void PlayerManager::setup() {

	ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
	options.port = 3500;

	bool connected = server.setup(options);

	// this adds your app as a listener for the server
	server.addListener(this);

	ofLogNotice() << "WebSocket server setup at " << ofToString(server.getPort()) << (server.usingSSL() ? " with SSL" : " without SSL");
}

void PlayerManager::update(){
	// [Jim] This is possibly not needed but not sure if something in oF will call it... ?
}

void PlayerManager::onConnect(ofxLibwebsockets::Event& args){
	cout << "on connected" << endl;
  args.conn.send(std::string(PACKET_MESSAGE) + "socket opened");
}

void PlayerManager::onOpen(ofxLibwebsockets::Event& args){
	cout << "new connection open from " << args.conn.getClientIP() << endl;

	// Engine.io handshake packet 
	// sid;
	// upgrades
	// pingInterval;
	// pingTimeout;

  // TODO - Create a proper SID
	// TODO - Build with a JSON builder
	std::string handshake = "{ \"sid\": 1, \"upgrades\": [\"websockets\"], \"pingInterval\": 100, \"pingTimeout\": 1000 }";

	args.conn.send(PACKET_OPEN + handshake);
}

void PlayerManager::onClose(ofxLibwebsockets::Event& args){
	cout << "on close" << endl;
}

void PlayerManager::onIdle(ofxLibwebsockets::Event& args){
  cout << "on idle " << args.message << endl;

}

// THIS BADLY NEEDS REFACTORING, it only grows from here... but maybe not today.
void PlayerManager::onMessage(ofxLibwebsockets::Event& args) {
	ofLogVerbose() << "got message " << args.message << endl;

	// Parse message
	// TODO: Create an engine.io packet parser
	int pos = args.message.find(messageDelimiter);

  // HAHAHAH, rookie mistake.
  if (pos >= 0) {
	//std::string msgPrefix = args.message.substr(0, pos);
	std::string msgData = args.message.substr(pos, args.message.length());

	vector<string> parts = ofSplitString(args.message, messageDelimiter);
	std::string msgPrefix = parts[0];
    // If someone has a better way to convert this stupid char to a string, I'm all ears -jim
    string msgType(1, msgPrefix.at(0)); 
	msgPrefix.erase(0, 1);

	ofPtr<Player> player = findPlayer(args.conn);

    // PING (heartbeat)
    if (msgType == PACKET_PING) {
      // PONG (no, bleepout)
      args.conn.send(PACKET_PONG);
      return;
    }

	// LEAVE YPR AT TOP OF MESSAGE SWITCHING - ypr is by far the priority message
	// if the prefix is ypr then we have a yaw-pitch-roll message, parse it
	if (msgPrefix == MESSAGE_YPR) {
		if (!_inRoundMode) {
        ofLogWarning() << "Ignoring YPR message in setup mode" << endl;
			return;
		}

		if (!player) {
        ofLogWarning() << "YPR message received for nonexistant player" << endl;
			return;
		}

		//pos = msgData.find(messageDelimiter);
		float yaw = ofToFloat(parts[1]); //ofToFloat(msgData.substr(0, pos));
		// msgData.erase(0, pos + 1);
		// pos = msgData.find(messageDelimiter);
		float pitch = ofToFloat(parts[2]); //ofToFloat(msgData.substr(0, pos));
		// msgData.erase(0, pos + 1);
		float roll = ofToFloat(parts[3]); //ofToFloat(msgData);
		notifyPlayerYawPitchRoll(player.get(), yaw, pitch, roll);
	}
	// click messages? Other?

	if (msgPrefix == MESSAGE_NEW) {
      /*if (_inRoundMode) {
			ofLogWarning() << "Ignoring create player message in setup mode";
			return;
        }*/
		if (player) {
        ofLogWarning() << "Got create player message for existing player: " << *player << endl;
			return;
		}

		player.reset(new Player(&args.conn));
      // TODO: set player id right here ... should be in parts[0]
      //int id___UNUSED = ofHexToInt(parts[1])

      controller.connect(*player);

      ofLogNotice() << "Player Created - id#" << parts[1];
		return;
	}

	// Set color
	if (msgPrefix == ACTION_CONFIGURE) {
      ofColor color(
        ofHexToInt(parts[2]),
        ofHexToInt(parts[3]),
        ofHexToInt(parts[4]));
      controller.configure(*player, color);
	}
    // Indicates that player has calibrated
	if (msgPrefix == ACTION_CALIBRATE) {
      controller.calibrate(*player);
	}
    // Player has started their game, we are free to drop their ball
	if (msgPrefix == ACTION_START) {
      controller.start(*player);
	}

	if (msgPrefix == ACTION_QUIT) {
      controller.quit(*player);
    }
	}
}

void PlayerManager::onBroadcast(ofxLibwebsockets::Event& args){
	cout << "got broadcast " << args.message << endl;
}

void PlayerManager::gotMessage(ofMessage msg){
	// cout << "got message " << msg << endl;
}

// Find player in collection based on connection (say that 10 times fast, sucka)
ofPtr<Player> PlayerManager::findPlayer(ofxLibwebsockets::Connection& conn) {
	// find player by comparing connection
	for (const ofPtr<Player>& p : _players) {
		if (*(p->connection()) == conn) {
			return p;
		}
	}

	return ofPtr<Player>();
}

// Consider renaming! 'notifyPlayer' sounds like we should notify the player
void PlayerManager::notifyPlayerYawPitchRoll(Player* player,
                                             float yaw,
                                             float pitch,
                                             float roll) {
  PlayerYawPitchRollEventArgs e(player, yaw, pitch, roll);
  ofNotifyEvent(playerYawPitchRollEvent, e);
}

/*
	SEND STATE MESSAGES TO PLAYER
*/
// Send 'Select Color' state message to player
void PlayerManager::setPlayerColor(Player& player) {
  player.connection()->send(std::string(PACKET_MESSAGE) + STATE_COLOR);
}
// Send 'Queued' state message to player
void PlayerManager::setPlayerQueued(Player& player) {
	player.connection()->send(std::string(PACKET_MESSAGE) + STATE_QUEUED);
}
// Send 'Calibrate' state message to player
void PlayerManager::setPlayerCalibrate(Player& player) {
	player.connection()->send(std::string(PACKET_MESSAGE) + STATE_CALIBRATION);
}
// Send 'Ready' state message to player 
void PlayerManager::setPlayerReady(Player& player) {
	player.connection()->send(std::string(PACKET_MESSAGE) + STATE_READY);
}
// Send 'Play' message to player (player should send back "start" message I think, to tell balls to drop)
void PlayerManager::setPlayerPlay(Player& player) {
	player.connection()->send(std::string(PACKET_MESSAGE) + STATE_PLAY);
}
