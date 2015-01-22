#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main()
{
    ofAppGLFWWindow window;
    ofSetupOpenGL(&window, 1400, 850, OF_WINDOW);
    ofRunApp( new ofApp());
}
