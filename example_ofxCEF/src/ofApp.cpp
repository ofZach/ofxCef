
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//--------------------------------------------------------------
void mouseScroll(GLFWwindow* window, double x, double y){
    
//    ((ofApp *)ofGetAppPtr())->cefgui->mouseWheel(x*10, y*10);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::setup(){
    
    // add scrolling callback
   // glfwSetScrollCallback( ((ofAppGLFWWindow *) ofGetWindowPtr())->getGLFWWindow(), mouseScroll);
    
 #if defined(TARGET_OSX) 
	cefgui->setup();
#elif defined(TARGET_WIN32)
	cefgui = new ofxCEF();
	cefgui->setup();
#endif   
    // Register event listener
    ofAddListener(cefgui->messageFromJS, this, &ofApp::gotMessageFromJS);
    ofAddListener(cefgui->eventFromCEF, this, &ofApp::eventFromCEF);
    
    ofSetVerticalSync(false);
    ofDrawBitmapMode mode;
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL );
}

//--------------------------------------------------------------
void ofApp::exit(){
    cefgui->renderHandler->bIsShuttingDown = true;
    cefgui->disableEvents();
    cefgui->browser->GetHost()->CloseBrowser(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    cefgui->update();

    if (ofGetFrameNum() == 1){
        string path = "file://" + ofToDataPath("html/index.html", true);
//        path = "http://haikusinteractifs.com/";
        path = "http://mrdoob.com/#/137/voxels_liquid";
//        path = "http://threejs.org/examples/";
//        path = "http://www.thefamilyfarmer.com";

        cefgui->load(path.c_str());
    }
    else if (ofGetFrameNum() == 300) {
        string command = "sendMessageToJS('This is OF talking to you! (sent on frame number 100)')";
        cefgui->executeJS((const char*) command.c_str());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255,0,255);
    cefgui->draw();
    
//    ofPushMatrix();
//    ofTranslate(ofPoint(ofGetMouseX(), ofGetMouseY()));
//    ofScale(2,2);
//    ofDrawBitmapStringHighlight("openframeworks !! up arrow to load new url", 0,0);
//    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::eventFromCEF(ofxCEFEventArgs& evt){
    if (evt.type == ofxCEFEventArgs::onLoadStart) {
        cout << "Receive an event from CEF > Content loading..." << endl;
    } else if (evt.type == ofxCEFEventArgs::onLoadEnd) {
        cout << "Receive an event from CEF > Content loaded with HTTP Status Code: " << evt.httpStatusCode << endl;
    }
}

//--------------------------------------------------------------
void ofApp::gotMessageFromJS(ofxCEFMessageArgs& msg){
    cout << "Got a message of type string from JS - type: " << msg.type << " - name:: " << msg.name << " - value :: " << msg.value << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_UP){
        string message = ofSystemTextBoxDialog("url to load");
        if (message.length() > 0){
            cefgui->load(message.c_str());
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
