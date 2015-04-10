// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "include/cef_app.h"
#include "ofxCEFClientApp.h"

//--------------------------------------------------------------
// Entry point function for sub-processes.
int main(int argc, char* argv[]){
    // Provide CEF with command-line arguments.
    CefMainArgs main_args(argc, argv);

    CefRefPtr<ofxCEFClientApp> app(new ofxCEFClientApp);
    // Execute the sub-process.
    return CefExecuteProcess(main_args, app.get(), NULL);
}
