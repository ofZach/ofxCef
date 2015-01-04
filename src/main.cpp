#include "ofMain.h"
#include "ofApp.h"
#include "cefgui.h"

//========================================================================
int main( ){
    
    
   
   
	ofSetupOpenGL(1024*2,768*2,OF_WINDOW);			// <-------- setup the GL context
    ofApp * app = new ofApp();

    
   
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(app);

}
