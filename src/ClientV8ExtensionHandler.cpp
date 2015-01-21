#include "ClientV8ExtensionHandler.h"

//--------------------------------------------------------------
ClientV8ExtensionHandler::ClientV8ExtensionHandler(CefRefPtr<CefApp> app)
{
    this->app = app;
}

//--------------------------------------------------------------
bool ClientV8ExtensionHandler::Execute(const CefString &name,
                                       CefRefPtr<CefV8Value> object,
                                       const CefV8ValueList &arguments,
                                       CefRefPtr<CefV8Value> &retval,
                                       CefString &exception)
{
    if (name == "sendDataToOF") {
        if (arguments.size() == 1 && arguments[0]->IsString()) {
            // Create the message object.
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
            
            // Retrieve the argument list object.
            CefRefPtr<CefListValue> args = message->GetArgumentList();
            
            // Populate the argument values.
            CefString valStr = arguments[0]->GetStringValue();
            args->SetString(0, valStr);
            
            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            browser->SendProcessMessage(PID_BROWSER, message);

         }
    }

    return false;
}
