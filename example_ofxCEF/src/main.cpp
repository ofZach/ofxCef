#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main(){
    
    //----------------------------------------------------- setup CEF at first (before anything)
    // note see http://www.magpcss.org/ceforum/viewtopic.php?f=6&t=11441&p=24037&hilit=AutoreleasePoolPage#p24037
    // or search for "AutoreleasePoolPage" in the CEF forum
	// see alos this thread http://www.magpcss.org/ceforum/viewtopic.php?f=10&t=12790#p25119
    
    int argc = 0;
    char** argv = NULL; 
	ofxCEF * cefgui = initofxCEF(argc, argv);
    
    //----------------------------------------------------- setup opengl
    ofAppGLFWWindow window;
    ofSetupOpenGL(&window, 1400, 850, OF_WINDOW);
    
    //----------------------------------------------------- pass CEF to the ofApp
    ofApp * p = new ofApp();
	p->cefgui = cefgui;
    ofRunApp( p );
    
}


