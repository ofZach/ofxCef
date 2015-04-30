#ifndef __CEF3SimpleSample__ClientHandler__
#define __CEF3SimpleSample__ClientHandler__

#include "include/cef_app.h"
#include "include/cef_client.h"

//--------------------------------------------------------------
class ofxCEFClientApp : public CefApp, public CefRenderProcessHandler
{
public:
    ofxCEFClientApp();

    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE
    {
        return this;
    }

    void OnWebKitInitialized() OVERRIDE;

	virtual void OnBeforeCommandLineProcessing( const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {

		CefString osr(L"-off-screen-rendering-enabled");
		command_line->AppendSwitch(osr);

		CefString d3d11(L"-disable-d3d11");
		command_line->AppendSwitch(d3d11);

		CefString frameScheduling(L"-enable-begin-frame-scheduling");
		command_line->AppendSwitch(frameScheduling);

		//CefString singleProcess(L"-single-process");
		//command_line->AppendSwitch(singleProcess);

		//CefString igProxy(L"-no-proxy-server");
		//command_line->AppendSwitch(igProxy);

		//CefString addFile(L"-allow-file-access-from-files");
		//command_line->AppendSwitch(addFile);

		//CefString touchEventsDisabled(L"-touch-events=disabled");
		//command_line->AppendSwitch(touchEventsDisabled);

		//CefString screenCap(L"-allow-http-screen-capture");
		//command_line->AppendSwitch(screenCap);

		//CefString optimizedUI(L"-touch-optimized-ui=disabled");
		//command_line->AppendSwitch(optimizedUI);

		// CefString touchSimulated(L"-simulate-touch-screen-with-mouse");
		// command_line->AppendSwitch(touchSimulated);

	}

    IMPLEMENT_REFCOUNTING(ofxCEFClientApp);
};

#endif
