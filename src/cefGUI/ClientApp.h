#ifndef __CEF3SimpleSample__ClientHandler__
#define __CEF3SimpleSample__ClientHandler__

#include "include/cef_app.h"
#include "include/cef_client.h"

//--------------------------------------------------------------
class ClientApp : public CefApp, public CefRenderProcessHandler
{
public:
    ClientApp();

    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
    {
        return this;
    }

    void OnWebKitInitialized() OVERRIDE;

    IMPLEMENT_REFCOUNTING(ClientApp);
};

#endif
