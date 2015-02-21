#include "ofxCEFClientApp.h"

#include "ofxCEFV8ExtensionHandler.h"

//--------------------------------------------------------------
ofxCEFClientApp::ofxCEFClientApp()
{
}

//--------------------------------------------------------------
void ofxCEFClientApp::OnWebKitInitialized()
{    
    std::string app_code =
        "var app;"
        "if (!app)"
        "    app = {};"
        "(function() {"
        "    app.sendMessageWithTypeToOF = function(type, name, value) {"
        "        native function sendMessageWithTypeToOF();"
        "        return sendMessageWithTypeToOF(type, name, value);"
        "    };"
        "    app.sendMessageToOF = function(name, value) {"
        "        native function sendMessageToOF();"
        "        return sendMessageToOF(name, value);"
        "    };"
        "})();;";

    CefRegisterExtension( "v8/app", app_code, new ofxCEFV8ExtensionHandler(this) );
}
