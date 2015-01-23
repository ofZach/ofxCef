#pragma once

#include "ofMain.h"
#include "ofxCEF.h"


class GLFWwindow;


class ofApp : public ofBaseApp
{

public:
    void setup();
    void exit();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void gotMessageFromJS(string strMessage);
    
    ofxCEF* cefgui;
    
    
    ofVideoGrabber 		vidGrabber;
    unsigned char * 	videoInverted;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
};
