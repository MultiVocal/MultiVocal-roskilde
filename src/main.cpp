#include "ofMain.h"
#include "ofApp.h"

#define DISPLAY_WIDTH 1024
#define DISPLAY_HEIGHT 768

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
