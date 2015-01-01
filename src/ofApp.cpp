#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    int argc = 0;
    char** argv;
    cefgui = initCefgui(argc, argv);
    cefgui->reshape(1000, 1000);
    cefgui->load("http://www.google.com");
    
    ofSetVerticalSync(false);
}

//--------------------------------------------------------------
void ofApp::update(){

    
}

//--------------------------------------------------------------
void ofApp::draw(){

    cout << ofGetFrameRate() << endl;
    cefgui->draw();

    cefgui->renderHandler->tex.draw(0,0);
    
    //cefgui->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
