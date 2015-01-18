#include "browser_client.h"
#include "ofApp.h"

BrowserClient::BrowserClient(RenderHandler* renderHandler)
{
  handler = renderHandler;
}

CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler() {
  return handler;
};

bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                              CefProcessId source_process,
                              CefRefPtr<CefProcessMessage> message) {
    
    // Retrieve the argument list object.
    CefRefPtr<CefListValue> args = message->GetArgumentList();
    
    // Retrieve the argument values.
    CefString valStr = args->GetString(0);

    // Forward the message argument value to OF.
    ofApp* ofHostApp = (ofApp*)ofGetAppPtr();
    ofHostApp->gotMessageFromJS(valStr.ToString());
    return true;
}
