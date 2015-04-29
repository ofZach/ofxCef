#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main(){
    
    //----------------------------------------------------- setup CEF at first (before anything)
    // note see http://www.magpcss.org/ceforum/viewtopic.php?f=6&t=11441&p=24037&hilit=AutoreleasePoolPage#p24037
    // or search for "AutoreleasePoolPage" in the CEF forum
    
    int argc = 0;
    char** argv = NULL; 
//#if defined(TARGET_OSX) 
//	ofxCEF * cefgui = initofxCEF(argc, argv);
//#elif defined(TARGET_WIN32)
	initofxCEF(argc, argv);
//#endif
    
    //----------------------------------------------------- setup opengl
    ofAppGLFWWindow window;
    ofSetupOpenGL(&window, 1400, 850, OF_WINDOW);
    
    //----------------------------------------------------- pass CEF to the ofApp
    ofApp * p = new ofApp();

//#if defined(TARGET_OSX) 
//	p->cefgui = cefgui;
//#elif defined(TARGET_WIN32)
//#endif

    ofRunApp( p );
    
}


