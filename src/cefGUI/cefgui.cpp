#include "cefgui.h"
#include "ofMain.h"
#include "ofAppGLFWWindow.h"
	#include <Cocoa/Cocoa.h>



int GetMacKeyCodeFromChar(int key_char) {
    switch (key_char) {
        case ' ': return kVK_Space;
            
        case '0': case ')': return kVK_ANSI_0;
        case '1': case '!': return kVK_ANSI_1;
        case '2': case '@': return kVK_ANSI_2;
        case '3': case '#': return kVK_ANSI_3;
        case '4': case '$': return kVK_ANSI_4;
        case '5': case '%': return kVK_ANSI_5;
        case '6': case '^': return kVK_ANSI_6;
        case '7': case '&': return kVK_ANSI_7;
        case '8': case '*': return kVK_ANSI_8;
        case '9': case '(': return kVK_ANSI_9;
            
        case 'a': case 'A': return kVK_ANSI_A;
        case 'b': case 'B': return kVK_ANSI_B;
        case 'c': case 'C': return kVK_ANSI_C;
        case 'd': case 'D': return kVK_ANSI_D;
        case 'e': case 'E': return kVK_ANSI_E;
        case 'f': case 'F': return kVK_ANSI_F;
        case 'g': case 'G': return kVK_ANSI_G;
        case 'h': case 'H': return kVK_ANSI_H;
        case 'i': case 'I': return kVK_ANSI_I;
        case 'j': case 'J': return kVK_ANSI_J;
        case 'k': case 'K': return kVK_ANSI_K;
        case 'l': case 'L': return kVK_ANSI_L;
        case 'm': case 'M': return kVK_ANSI_M;
        case 'n': case 'N': return kVK_ANSI_N;
        case 'o': case 'O': return kVK_ANSI_O;
        case 'p': case 'P': return kVK_ANSI_P;
        case 'q': case 'Q': return kVK_ANSI_Q;
        case 'r': case 'R': return kVK_ANSI_R;
        case 's': case 'S': return kVK_ANSI_S;
        case 't': case 'T': return kVK_ANSI_T;
        case 'u': case 'U': return kVK_ANSI_U;
        case 'v': case 'V': return kVK_ANSI_V;
        case 'w': case 'W': return kVK_ANSI_W;
        case 'x': case 'X': return kVK_ANSI_X;
        case 'y': case 'Y': return kVK_ANSI_Y;
        case 'z': case 'Z': return kVK_ANSI_Z;
            
            // U.S. Specific mappings.  Mileage may vary.
        case ';': case ':': return kVK_ANSI_Semicolon;
        case '=': case '+': return kVK_ANSI_Equal;
        case ',': case '<': return kVK_ANSI_Comma;
        case '-': case '_': return kVK_ANSI_Minus;
        case '.': case '>': return kVK_ANSI_Period;
        case '/': case '?': return kVK_ANSI_Slash;
        case '`': case '~': return kVK_ANSI_Grave;
        case '[': case '{': return kVK_ANSI_LeftBracket;
        case '\\': case '|': return kVK_ANSI_Backslash;
        case ']': case '}': return kVK_ANSI_RightBracket;
        case '\'': case '"': return kVK_ANSI_Quote;
    }
    
    return -1;
}

Cefgui* initCefgui(int argc, char** argv)
{
  CefMainArgs args(argc, argv);
  //CefExecuteProcess(args, 0, NULL);

  CefSettings settings;
  settings.background_color = 0xFFFF00FF;
  
  CefInitialize(args, settings, 0, NULL);

  return new Cefgui();
}

Cefgui::Cefgui()
{
  CefWindowInfo windowInfo;
  CefBrowserSettings settings;
 
    
    NSWindow * cocoaWindow =  (NSWindow *) ((ofAppGLFWWindow *) ofGetWindowPtr())->getCocoaWindow();
    NSView * view =  [ cocoaWindow contentView];
    
    
    windowInfo.SetAsWindowless(view, true);
    
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
  event.native_key_code = GetMacKeyCodeFromChar(key);
    event.character = (char)key;
    event.unmodified_character  =  event.character=  event.native_key_code;
    event.type = KEYEVENT_KEYDOWN;
  browser->GetHost()->SendKeyEvent(event);
}

void Cefgui::keyReleased(int key)
{
    CefKeyEvent event;
    event.native_key_code = GetMacKeyCodeFromChar(key);
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
