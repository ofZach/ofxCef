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

    void mouseMove(int, int);
    void mousePressed(int, int);
    void mouseReleased(int, int);
    void mouseWheel(int, int);
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    
    void enableEvents();
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
