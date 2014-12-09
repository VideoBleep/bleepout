/*
	PlayerSocket class for Bleepout
	By Jim Ankrom

*/
#ifndef __bleepout__PlayerSocket__
#define __bleepout__PlayerSocket__

#include "ofMain.h"

#include "ofxLibwebsockets.h"

class PlayerSocket {
	public:		
		//PlayerSocket();
		
		// Sockets Server
		ofxLibwebsockets::Server server;

		// Message queue (temporary?)
		ofTrueTypeFont font;
		vector<string> messages;

		void setup();
		void update();
		void draw();
		void gotMessage(ofMessage msg);

		// Websocket event handlers
		void onConnect(ofxLibwebsockets::Event& args);
		void onOpen(ofxLibwebsockets::Event& args);
		void onClose(ofxLibwebsockets::Event& args);
		void onIdle(ofxLibwebsockets::Event& args);
		void onMessage(ofxLibwebsockets::Event& args);
		void onBroadcast(ofxLibwebsockets::Event& args);

};
#endif /* defined(__bleepout__PlayerSocket__) */