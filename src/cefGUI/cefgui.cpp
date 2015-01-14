#include "cefgui.h"
#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include <Cocoa/Cocoa.h>
#include "ClientApp.h"



Cefgui* initCefgui(int argc, char** argv)
{
  CefMainArgs args(argc, argv);
  //CefExecuteProcess(args, 0, NULL);

  CefSettings settings;
  settings.background_color = 0xFFFF00FF;
  settings.single_process = true;
  
  CefRefPtr<ClientApp> app(new ClientApp);

  CefInitialize(args, settings, app.get(), NULL);
  
  return new Cefgui();
}

Cefgui::Cefgui()
{
  CefWindowInfo windowInfo;
  CefBrowserSettings settings;
 
    
    NSWindow * cocoaWindow =  (NSWindow *) ((ofAppGLFWWindow *) ofGetWindowPtr())->getCocoaWindow();
    NSView * view =  [ cocoaWindow contentView];
    
    
    windowInfo.SetAsWindowless(view, true);
    windowInfo.transparent_painting_enabled = 1;
    //windowInfo.SetAsChild(view, 0, 0, 1000, 1000);
    
    
    
  renderHandler = new RenderHandler();

  client = new BrowserClient(renderHandler);
  browser = CefBrowserHost::CreateBrowserSync(windowInfo, client.get(), "", settings, 0);
}

void Cefgui::load(const char* url)
{
  if (!renderHandler->initialized)
    renderHandler->init();

  browser->GetMainFrame()->LoadURL(url);
}

void Cefgui::draw(void)
{
  CefDoMessageLoopWork();
  //renderHandler->draw();
}

void Cefgui::reshape(int w, int h)
{
  renderHandler->reshape(w, h);
  browser->GetHost()->WasResized();
}

void Cefgui::mouseMove(int x, int y)
{
    if (renderHandler->bIsRetinaDisplay){
        x/=2;
        y/=2;
    }
    
  mouseX = x;
  mouseY = y;

  CefMouseEvent event;
  event.x = x;
  event.y = y;

  browser->GetHost()->SendMouseMoveEvent(event, false);
    
}

void Cefgui::mouseWheel(int x, int y){
    
    CefMouseEvent mouse_event;
    mouse_event.x = mouse_event.y = 1;
    mouse_event.modifiers = 0;
    browser->GetHost()->SendMouseWheelEvent(mouse_event, x,y);
    
}

void Cefgui::mousePressed(int x, int y)
{
    
    browser->GetHost()->SendFocusEvent(true);
    
    if (renderHandler->bIsRetinaDisplay){
        x/=2;
        y/=2;
    }
    
  CefMouseEvent event;
  event.x = x;
  event.y = y;
  event.modifiers = 0;
    event.modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  CefBrowserHost::MouseButtonType btnType = MBT_LEFT;
  browser->GetHost()->SendMouseClickEvent(event, btnType, false, 1);
}

void Cefgui::mouseReleased(int x, int y)
{
    if (renderHandler->bIsRetinaDisplay){
        x/=2;
        y/=2;
    }
    
    CefMouseEvent event;
    event.x = x;
    event.y = y;
    event.modifiers = 0;
    event.modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
    CefBrowserHost::MouseButtonType btnType = MBT_LEFT;
    browser->GetHost()->SendMouseClickEvent(event, btnType, true, 1);

}

void Cefgui::keyPressed(int key)
{
  CefKeyEvent event;
    event.native_key_code = key;
    event.character = (char)key;
    event.unmodified_character  =  event.character=  event.native_key_code;
    event.type = KEYEVENT_CHAR;
  browser->GetHost()->SendKeyEvent(event);
}

void Cefgui::keyReleased(int key)
{
    CefKeyEvent event;
    event.native_key_code = key; 
    event.type = KEYEVENT_KEYUP;
    event.unmodified_character  =  event.character=  event.native_key_code;
    
    browser->GetHost()->SendKeyEvent(event);
}


void Cefgui::executeJS(const char* command)
{
  CefRefPtr<CefFrame> frame = browser->GetMainFrame();
  frame->ExecuteJavaScript(command, frame->GetURL(), 0);

  // TODO limit frequency of texture updating
  CefRect rect;
  renderHandler->GetViewRect(browser, rect);
  browser->GetHost()->Invalidate(PET_VIEW);
}
