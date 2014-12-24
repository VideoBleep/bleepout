//
//  PlayerManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//  
//


#include "PlayerManager.h"
#include "RoundManager.h"
#include "Logging.h"

std::string messageDelimiter = "|";

PlayerManager::PlayerManager()
{

}

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
	//messages.push_back("Reply would execute here");
}

void PlayerManager::draw(){
	// [Jim] This is not needed but not sure if something in oF will call it... ?
}

void PlayerManager::onConnect(ofxLibwebsockets::Event& args){
	cout << "on connected" << endl;



	// Engine.io handshake packet 
	// sid;
	// upgrades
	// pingInterval;
	// pingTimeout;

	// TODO - Build with a JSON builder
	std::string handshake = "{ \"sid\": 1, \"upgrades\": [\"websockets\"], \"pingInterval\": 100, \"pingTimeout\": 1000 }";

	args.conn.send(PACKET_OPEN + handshake);
}

void PlayerManager::onOpen(ofxLibwebsockets::Event& args){
	cout << "new connection open from " << args.conn.getClientIP() << endl;

	args.conn.send(PACKET_MESSAGE + "socket opened");
}

void PlayerManager::onClose(ofxLibwebsockets::Event& args){
	cout << "on close" << endl;
}

void PlayerManager::onIdle(ofxLibwebsockets::Event& args){
	cout << "on idle" << endl;
}

void PlayerManager::onMessage(ofxLibwebsockets::Event& args){
	ofLogVerbose() << "got message " << args.message << endl;

	// Parse message
	// TODO: Create an engine.io packet parser
	int pos = args.message.find(messageDelimiter);
	//std::string msgPrefix = args.message.substr(0, pos);
	std::string msgData = args.message.substr(pos, args.message.length());

	vector<string> parts = ofSplitString(args.message, messageDelimiter);
	std::string msgPrefix = parts[0];
	char msgType = msgPrefix.at(0);
	msgPrefix.erase(0, 1);

	ofPtr<Player> player = findPlayer(args.conn);

	if (msgPrefix == MESSAGE_NEW) {
		if (_inRoundMode) {
			ofLogWarning() << "Ignoring create player message in setup mode";
			return;
		}
		if (player) {
			ofLogWarning() << "Got create player message for existing player: " << *player;
			return;
		}
		int id___UNUSED = ofHexToInt(parts[1]);
		ofColor color(ofHexToInt(parts[2]),
									ofHexToInt(parts[3]),
									ofHexToInt(parts[4]));
		player.reset(new Player(&args.conn));
		player->setColor(color);
		_players.push_back(player);
		
		ofLogNotice() << "Player Created - id#" << id___UNUSED;
		// Pong
		args.conn.send("hello");
		return;
	}

	// if the prefix is ypr then we have a yaw-pitch-roll message, parse it
	if (msgPrefix == MESSAGE_YPR) {
		if (!_inRoundMode) {
			ofLogWarning() << "Ignoring YPR message in setup mode";
			return;
		}

		if (!player) {
			ofLogWarning() << "YPR message received for nonexistant player";
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

void PlayerManager::notifyPlayerAdded(ofPtr<Player> player) {
  PlayerEventArgs e(player);
  ofNotifyEvent(playerAddedEvent, e);
  logEvent("PlayerAdded", e);
}
void PlayerManager::notifyPlayerRemoved(ofPtr<Player> player) {
  PlayerEventArgs e(player);
  ofNotifyEvent(playerRemovedEvent, e);
  logEvent("PlayerRemoved", e);
}

void PlayerManager::notifyPlayerYawPitchRoll(Player* player,
                                             float yaw,
                                             float pitch,
                                             float roll) {
  PlayerYawPitchRollEventArgs e(player, yaw, pitch, roll);
  ofNotifyEvent(playerYawPitchRollEvent, e);
}