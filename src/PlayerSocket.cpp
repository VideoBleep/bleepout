/*
	PlayerSocket class for Bleepout
	By Jim Ankrom
	(currently pattened after Libwebsockets example code)

*/
#include "PlayerSocket.h"

void PlayerSocket::setup(){

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

void PlayerSocket::update(){
	messages.push_back("Reply would execute here");
}

void PlayerSocket::draw(){
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

void PlayerSocket::onConnect(ofxLibwebsockets::Event& args){
	cout << "on connected" << endl;
}

void PlayerSocket::onOpen(ofxLibwebsockets::Event& args){
	cout << "new connection open" << endl;
	messages.push_back("New connection from " + args.conn.getClientIP());
}

void PlayerSocket::onClose(ofxLibwebsockets::Event& args){
	cout << "on close" << endl;
	messages.push_back("Connection closed");
}

void PlayerSocket::onIdle(ofxLibwebsockets::Event& args){
	cout << "on idle" << endl;
}

void PlayerSocket::onMessage(ofxLibwebsockets::Event& args){
	ofLogVerbose() << "got message " << args.message << endl;

	// trace out string messages or JSON messages!
	// args.json is null if badly formed or just not JOSN
	if (!args.json.isNull()){
		messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName());
	}
	else {
		messages.push_back("New message: " + args.message + " from " + args.conn.getClientName());
	}

	//if (messages.size() > NUM_MESSAGES) messages.erase( messages.begin() );

	// echo server = send message right back!
	args.conn.send(args.message);
}

void PlayerSocket::onBroadcast(ofxLibwebsockets::Event& args){
	cout << "got broadcast " << args.message << endl;
}

void PlayerSocket::gotMessage(ofMessage msg){
	// cout << "got message " << msg << endl;
}