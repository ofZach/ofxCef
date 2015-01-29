#include "cef_app.h"

#include "ofxCEFBrowserClient.h"
#include "ofxCEFRenderHandler.h"

#ifndef CEFGUI_H
#define CEFGUI_H

//--------------------------------------------------------------
class ofxCEF;

ofxCEF* initofxCEF(int argc, char** argv);

//--------------------------------------------------------------
class ofxCEF
{
public:
    ofxCEF();
    ~ofxCEF();
    
    void load(const char*);
    void update();
    void draw(void);
    void reshape(int, int);

    void mouseWheel(int, int);
    
    void mousePressed(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void mouseMoved(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    
    void windowResized(ofResizeEventArgs &e);
    
    void enableEvents();
    void disableEvents();
    
    void executeJS(const char*);

    void notificationHandler();

 // private:
    int mouseX, mouseY;

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<ofxCEFBrowserClient> client;

    ofxCEFRenderHandler* renderHandler;

};

typedef void (ofxCEF::*notificationHandler)();

#endif
