#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    int argc = 0;
    char** argv;
    
    
    cefgui = initCefgui(argc, argv);
    cefgui->reshape(1000, 1000);
    cefgui->load("http://mrdoob.com/#/151/translucent_network");
    
    ofSetVerticalSync(false);
}

//--------------------------------------------------------------
void ofApp::update(){

    //CefRunMessageLoop();
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    //cout << ofGetFrameRate() << endl;
    cefgui->draw();
    cefgui->renderHandler->tex.draw(0,0);
    
    //cefgui->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    cefgui->keyPress(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    cefgui->mouseMove(x, y);
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
   
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 cefgui->mouseClick(x, y);
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
