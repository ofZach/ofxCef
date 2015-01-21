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
        "    app.sendDataToOF = function(text) {"
        "        native function sendDataToOF(text);"
        "        return sendDataToOF(text);"
        "    };"
        "})();;";

    CefRegisterExtension( "v8/app", app_code, new ofxCEFV8ExtensionHandler(this) );
}
