//
//  PlayerManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//  
//

#include "PlayerManager.h"
#include "RoundManager.h"

PlayerManager::PlayerManager(ofPtr<RoundController> roundController) :
	_roundController(roundController),
	_state(roundController->state())
{
	setup();
}

ofPtr<Player> PlayerManager::addPlayer() {
  ofPtr<Player> player(new Player());
  _state.players().push_back(player);
  return player;
}

// -------------------WebSockets support


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

	// find player by comparing 
	// Parse control message

	// if the prefix is ypr then we have a yaw-pitch-roll message
	// if the prefix is but then we have a click message
}

void PlayerManager::onBroadcast(ofxLibwebsockets::Event& args){
	cout << "got broadcast " << args.message << endl;
}

void PlayerManager::gotMessage(ofMessage msg){
	// cout << "got message " << msg << endl;
}