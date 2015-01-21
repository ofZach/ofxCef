#include "ClientApp.h"

#include "ClientV8ExtensionHandler.h"

//--------------------------------------------------------------
ClientApp::ClientApp()
{
}

//--------------------------------------------------------------
void ClientApp::OnWebKitInitialized()
{    
    std::string app_code =
        "var app;"
        "if (!app)"
        "    app = {};"
        "(function() {"
        "    app.sendDataToOF = function(text) {"
        "        native function sendDataToOF(text);"
        "        return sendDataToOF(text);"
        "    };"
        "})();;";

    CefRegisterExtension( "v8/app", app_code, new ClientV8ExtensionHandler(this) );
}
