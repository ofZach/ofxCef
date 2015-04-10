#include "ofxCEFBrowserClient.h"
#include "ofxCEF.h"

//--------------------------------------------------------------
ofxCEFBrowserClient::ofxCEFBrowserClient(ofxCEF* parent, ofxCEFRenderHandler* renderHandler){
    _parent = parent;
    handler = renderHandler;
}

//--------------------------------------------------------------
CefRefPtr<CefRenderHandler> ofxCEFBrowserClient::GetRenderHandler(){
    return handler;
}

//--------------------------------------------------------------
CefRefPtr<CefLoadHandler> ofxCEFBrowserClient::GetLoadHandler(){
    return this;
}

//--------------------------------------------------------------
void ofxCEFBrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame){
    _parent->onLoadStart();
}


//--------------------------------------------------------------
void ofxCEFBrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode){
    _parent->onLoadEnd(httpStatusCode);
}

//--------------------------------------------------------------
bool ofxCEFBrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message){
    // Retrieve the argument list object.
    CefRefPtr<CefListValue> args = message->GetArgumentList();

    // Retrieve the argument values.
    CefString type = message->GetName();
    string name = args->GetString(0).ToString();
    
    
    // Forward the message argument value to the parent (instance of ofxCEF).    
    if (type == "string") {
        _parent->gotMessageFromJS(name, type, args->GetString(1).ToString());
    }
    else if (type == "double") {
        _parent->gotMessageFromJS(name, type, ofToString(args->GetDouble(1)));
    }
    else if (type == "int") {
        _parent->gotMessageFromJS(name, type, ofToString(args->GetInt(1)));
    }
    else if (type == "bool") {
        _parent->gotMessageFromJS(name, type, ofToString(args->GetBool(1)));
    }
    else {
        std::cout << "ofxCEFBrowserClient received a message of unknown type." << std::endl;
        return false;
    }


    
    return true;
}
