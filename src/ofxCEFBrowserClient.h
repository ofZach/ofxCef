#include "cef_client.h"

#include "ofxCEFRenderHandler.h"

#ifndef BROWSER_CLIENT_H
#define BROWSER_CLIENT_H

//--------------------------------------------------------------
class ofxCEFBrowserClient : public CefClient
{
public:
    ofxCEFBrowserClient(ofxCEFRenderHandler*);

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                              CefProcessId source_process,
                              CefRefPtr<CefProcessMessage> message);

private:
    CefRefPtr<CefRenderHandler> handler;

    IMPLEMENT_REFCOUNTING(ofxCEFBrowserClient);

};

#endif
