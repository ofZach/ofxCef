#include "cef_client.h"

#include "render_handler.h"

#ifndef BROWSER_CLIENT_H
#define BROWSER_CLIENT_H

//--------------------------------------------------------------
class BrowserClient : public CefClient
{
public:
    BrowserClient(RenderHandler*);

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                              CefProcessId source_process,
                              CefRefPtr<CefProcessMessage> message);

private:
    CefRefPtr<CefRenderHandler> handler;

    IMPLEMENT_REFCOUNTING(BrowserClient);

};

#endif
