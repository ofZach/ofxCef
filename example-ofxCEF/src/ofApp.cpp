
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//--------------------------------------------------------------
void mouseScroll(GLFWwindow* window, double x, double y)
{
    
    ((ofApp *)ofGetAppPtr())->cefgui->mouseWheel(x*10, y*10);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::setup()
{
    int argc = 0;
    char** argv;
    
    // add scrolling callback
    glfwSetScrollCallback( ((ofAppGLFWWindow *) ofGetWindowPtr())->getGLFWWindow(), mouseScroll);
    
    cefgui = initofxCEF(argc, argv);

    ofSetVerticalSync(false);
    ofDrawBitmapMode mode;
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL );
}

//--------------------------------------------------------------
void ofApp::exit()
{
    delete cefgui;
}

//--------------------------------------------------------------
void ofApp::update()
{
    CefDoMessageLoopWork();
    //CefRunMessageLoop();

    if (ofGetFrameNum() == 1){
        string path = "file://" + ofToDataPath("html/index.html", true);
//        path = "http://haikusinteractifs.com/";
//        path = "http://mrdoob.com/#/137/voxels_liquid";
//        path = "http://threejs.org/examples/";
        //path = "http://www.thefamilyfarmer.com";

        cefgui->load(path.c_str());
    }
    else if (ofGetFrameNum() == 300) {
        string command = "sendMessageToJS('This is OF talking to you! (sent on frame number 100)')";
        cefgui->executeJS((const char*) command.c_str());
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(255,0,255);
    
    cefgui->draw();
    
//    ofPushMatrix();
//    ofTranslate(ofPoint(ofGetMouseX(), ofGetMouseY()));
//    ofScale(2,2);
//    ofDrawBitmapStringHighlight("openframeworks !! up arrow to load new url", 0,0);
//    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::gotMessageFromJS(string strMessage)
{
    cout << "Got a message from JS: " << strMessage << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == OF_KEY_UP){
        string message = ofSystemTextBoxDialog("url to load");
        if (message.length() > 0){
            cefgui->load(message.c_str());
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    if (cefgui!= NULL){
        cefgui->mouseMove(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (cefgui!= NULL){
        cefgui->mouseMove(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    if (cefgui!= NULL){
        cefgui->mousePressed(x, y);
        //--------------------------------------------------------------
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if (cefgui!= NULL){
        cefgui->mouseReleased(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    if (cefgui!= NULL){
        cefgui->reshape(w,h);
        cefgui->renderHandler->init();
        cefgui->browser->Reload();
    }
    // this is failing
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
