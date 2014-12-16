//
//  PlayerManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//  
//


#include "PlayerManager.h"
#include "RoundManager.h"

std::string messageDelimiter = "|";

PlayerManager::PlayerManager(ofPtr<RoundController> roundController) :
	_roundController(roundController),
	_state(roundController->state())
{

}

ofPtr<Player> PlayerManager::addPlayer() {
  ofPtr<Player> player(new Player());
  player->setColor(ofColor::green);
  _state.players().push_back(player);
  return player;
}

void PlayerManager::setup(){

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
	cout << "new connection open" << endl;
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

	if (msgPrefix == "4new") {
		// This implies that the PlayerCreateMessage will be responsible for instantiating the player
		// Is that correct? If the .player instance is referenced but not 
		// the rest of the PlayerCreateMessage ... is that a problem?
		PlayerCreateMessage newPlayer;
		newPlayer.id = ofHexToInt(parts[1]);
		newPlayer.color.set(ofHexToInt(parts[2]),
			ofHexToInt(parts[3]),
			ofHexToInt(parts[4]));
		newPlayer.player.reset(new Player(&args.conn));
		newPlayer.player->setColor(newPlayer.color);
		_roundController->state().players().push_back(newPlayer.player);

		// Pong
		args.conn.send("hello");
		return;
	}

	// if the prefix is ypr then we have a yaw-pitch-roll message, parse it
	if (msgPrefix == "ypr") {
		ofPtr<Player> player = findPlayer(args.conn);

		PlayerYawPitchRollMessage ypr;
		ypr.player = player;
		//pos = msgData.find(messageDelimiter);
		ypr.yaw = ofToFloat(parts[1]); //ofToFloat(msgData.substr(0, pos));
		// msgData.erase(0, pos + 1);
		// pos = msgData.find(messageDelimiter);
		ypr.pitch = ofToFloat(parts[2]); //ofToFloat(msgData.substr(0, pos));
		// msgData.erase(0, pos + 1);
		ypr.roll = ofToFloat(parts[3]); //ofToFloat(msgData);
		_roundController->setPaddlePosition(ypr);
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
	for (const ofPtr<Player>& p : state().players()) {
		if (*(p->connection()) != NULL) {
			if (*(p->connection()) == conn) {
				return p;
			}
		}
	}
	// TODO: Is this correct way to return null?
	return ofPtr<Player>();
}