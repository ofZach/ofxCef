#include "ofxCEFBrowserClient.h"
#include "ofApp.h"

//--------------------------------------------------------------
ofxCEFBrowserClient::ofxCEFBrowserClient(ofxCEFRenderHandler* renderHandler)
{
    handler = renderHandler;
}

//--------------------------------------------------------------
CefRefPtr<CefRenderHandler> ofxCEFBrowserClient::GetRenderHandler()
{
    return handler;
}

//--------------------------------------------------------------
bool ofxCEFBrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message)
{
    // Retrieve the argument list object.
    CefRefPtr<CefListValue> args = message->GetArgumentList();

    // Retrieve the argument values.
    CefString valStr = args->GetString(0);

    // Forward the message argument value to OF.
    ofApp* ofHostApp = (ofApp*)ofGetAppPtr();
    ofHostApp->gotMessageFromJS(valStr.ToString());
    return true;
}
