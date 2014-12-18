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

	// TEMPORARY rendering stuff -------------------------------------
	// setup message queue
	font.loadFont("myriad.ttf", 20);
	messages.push_back("WebSocket server setup at " + ofToString(server.getPort()) + (server.usingSSL() ? " with SSL" : " without SSL"));

	//ofBackground(0);
	//ofSetFrameRate(60);	
	// TEMPORARY rendering stuff end ---------------------------------
}

void PlayerManager::update(){
	messages.push_back("Reply would execute here");
}

void PlayerManager::draw(){
	int x = font.getSize();
	int y = font.getSize() * 2;
	ofSetColor(255);
	for (int i = messages.size() - 1; i >= 0; i--){
		//font.drawString( messages[i], x, y );
		y += font.stringHeight(messages[i]) + font.getSize();
	}
	//if (currentImage.bAllocated()) currentImage.draw(0, 0);
	//ofDrawBitmapString("Drag an image onto the window to send!\nOpen your browser to localhost:9093 to receive", 20, 20);
}

void PlayerManager::onConnect(ofxLibwebsockets::Event& args){
	cout << "on connected" << endl;
}

void PlayerManager::onOpen(ofxLibwebsockets::Event& args){
	cout << "new connection open from " << args.conn.getClientIP() << endl;
	messages.push_back("New connection from " + args.conn.getClientIP());
}

void PlayerManager::onClose(ofxLibwebsockets::Event& args){
	cout << "on close" << endl;
	messages.push_back("Connection closed");
}

void PlayerManager::onIdle(ofxLibwebsockets::Event& args){
	cout << "on idle" << endl;
}

void PlayerManager::onMessage(ofxLibwebsockets::Event& args){
	ofLogVerbose() << "got message " << args.message << endl;

	// Parse message
	int pos = args.message.find(messageDelimiter);
	//std::string msgPrefix = args.message.substr(0, pos);
	std::string msgData = args.message.substr(pos, args.message.length());

	vector<string> parts = ofSplitString(args.message, messageDelimiter);
	std::string msgPrefix = parts[0];

	ofPtr<Player> player = findPlayer(args.conn);

	if (msgPrefix == "4new") {
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
	if (msgPrefix == "ypr") {
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