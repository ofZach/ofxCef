#include "cef_client.h"

#include "ofxCEFRenderHandler.h"

#ifndef BROWSER_CLIENT_H
#define BROWSER_CLIENT_H

//--------------------------------------------------------------
class ofxCEFBrowserClient : public CefClient, public CefLoadHandler
{
public:
    ofxCEFBrowserClient(ofxCEFRenderHandler*);

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler();
    
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                              CefProcessId source_process,
                              CefRefPtr<CefProcessMessage> message);
    
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame);
    

    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode);

private:
    CefRefPtr<CefRenderHandler> handler;

    IMPLEMENT_REFCOUNTING(ofxCEFBrowserClient);

};

#endif
