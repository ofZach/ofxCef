#include "cef_app.h"

#include "ofxCEFBrowserClient.h"
#include "ofxCEFRenderHandler.h"

#ifndef CEFGUI_H
#define CEFGUI_H

//--------------------------------------------------------------
class ofxCEFMessageArgs : public ofEventArgs{
public:
    string name;
    string type;
    string value;
};

//--------------------------------------------------------------
class ofxCEFEventArgs : public ofEventArgs{
public:
    enum Type{
        onLoadStart,
        onLoadEnd
    } type;
    int httpStatusCode;
};

//--------------------------------------------------------------
class ofxCEF;

#if defined(TARGET_OSX) 
ofxCEF* initofxCEF(int argc, char** argv);
#elif defined(TARGET_WIN32)
int initofxCEF(int argc, char** argv);
#endif


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
    
    void setup();
    
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
    
    void onLoadStart();
    void onLoadEnd(int httpStatusCode);
    
    void gotMessageFromJS(string name, string type, string value);

    ofEvent<ofxCEFMessageArgs> messageFromJS;
    ofEvent<ofxCEFEventArgs> eventFromCEF;

 // private:
    int mouseX, mouseY;

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<ofxCEFBrowserClient> client;

    ofxCEFRenderHandler* renderHandler;

};

typedef void (ofxCEF::*notificationHandler)();

#endif
