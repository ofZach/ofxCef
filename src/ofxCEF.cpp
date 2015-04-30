#include "ofxCEF.h"
#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#if defined(TARGET_OSX)
#include <Cocoa/Cocoa.h>
#endif
#include "ofxCEFClientApp.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
#if defined(TARGET_OSX)
@interface NSNotificationManager: NSObject
{
    ofxCEF *observer;
    notificationHandler callback;
}

- (void)onChangeScreen:(id)sender;

@end

@implementation NSNotificationManager

- (id)initWithObserver:(ofxCEF *)obs method:(notificationHandler)m;
{
    self = [super init];
    if (self) {
        observer = obs;
        callback = m;
        
        NSWindow * cocoaWindow =  (NSWindow *) ((ofAppGLFWWindow *) ofGetWindowPtr())->getCocoaWindow();
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(onChangeScreen:) name:NSWindowDidChangeScreenNotification
                                                   object:cocoaWindow];
    }
    return self;
}

- (void)onChangeScreen:(id)sender
{
    (observer->*callback)();
}

@end
#endif

//--------------------------------------------------------------
//--------------------------------------------------------------
#if defined(TARGET_WIN32)
HINSTANCE hInst;   // current instance
#endif

ofxCEF* initofxCEF(int argc, char** argv){
#if defined(TARGET_OSX) 
	CefMainArgs main_args(argc, argv);
#elif defined(TARGET_WIN32)
	CefMainArgs main_args(::GetModuleHandle(NULL));
#endif

	CefRefPtr<ofxCEFClientApp> app(new ofxCEFClientApp);

	int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
	if (exit_code >= 0) {
		//return exit_code;
	}

	CefSettings settings;
	settings.background_color = 0xFFFF00FF;
	settings.single_process = false; 
	settings.windowless_rendering_enabled = true;
	settings.command_line_args_disabled = true;
	//settings.multi_threaded_message_loop = true;

	CefInitialize(main_args, settings, app.get(), NULL);

	return new ofxCEF();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofxCEF::setup(){
    
	CefWindowInfo windowInfo;
	renderHandler = new ofxCEFRenderHandler();

#if defined(TARGET_OSX) 
	NSWindow * cocoaWindow =  (NSWindow *) ((ofAppGLFWWindow *) ofGetWindowPtr())->getCocoaWindow();
    [cocoaWindow setReleasedWhenClosed:NO];
    
    NSView * view =  [ cocoaWindow contentView];
    windowInfo.SetAsWindowless(view, true);

	if (renderHandler->bIsRetinaDisplay) {
        ofSetWindowPosition(0, 0);
        ofSetWindowShape(ofGetWidth(), ofGetHeight());
    }

#elif defined(TARGET_WIN32)
	HWND hWnd = ofGetWin32Window();
	windowInfo.SetAsWindowless(hWnd, true);
#endif

	windowInfo.transparent_painting_enabled = STATE_ENABLED;

	CefBrowserSettings settings;
    settings.web_security = STATE_DISABLED;
	settings.webgl = STATE_ENABLED;
	settings.windowless_frame_rate = 60;

	client = new ofxCEFBrowserClient(this, renderHandler);
    browser = CefBrowserHost::CreateBrowserSync(windowInfo, client.get(), "", settings, NULL);
  
#if defined(TARGET_OSX) 
	if (renderHandler->bIsRetinaDisplay) {
        reshape(ofGetWidth()*2, ofGetHeight()*2);
    }
#endif
    
    enableEvents();    
}

//--------------------------------------------------------------
ofxCEF::ofxCEF(){
}

//--------------------------------------------------------------
ofxCEF::~ofxCEF(){
}

void ofxCEF::exit() {
	//TODO Check if we need to do some calls to OnBeforeClose 
	disableEvents();
	renderHandler->bIsShuttingDown = true;
    browser->GetHost()->CloseBrowser(false);
	CefShutdown();
}


//--------------------------------------------------------------
void ofxCEF::enableEvents(){
    ofAddListener(ofEvents().mousePressed, this, &ofxCEF::mousePressed);
    ofAddListener(ofEvents().mouseMoved, this, &ofxCEF::mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxCEF::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxCEF::mouseReleased);
    
    ofAddListener(ofEvents().keyPressed, this, &ofxCEF::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxCEF::keyReleased);
    
    ofAddListener(ofEvents().windowResized, this, &ofxCEF::windowResized);
}

//--------------------------------------------------------------
void ofxCEF::disableEvents(){
    ofRemoveListener(ofEvents().mousePressed, this, &ofxCEF::mousePressed);
    ofRemoveListener(ofEvents().mouseMoved, this, &ofxCEF::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxCEF::mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxCEF::mouseReleased);
    
    ofRemoveListener(ofEvents().keyPressed, this, &ofxCEF::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &ofxCEF::keyReleased);
    
    ofRemoveListener(ofEvents().windowResized, this, &ofxCEF::windowResized);
}

//--------------------------------------------------------------
void ofxCEF::load(const char* url){
    if (!renderHandler->initialized) {
        renderHandler->init();
    }

    browser->GetMainFrame()->LoadURL(url);
}

//--------------------------------------------------------------
void ofxCEF::update(){
    GLint swapbytes, lsbfirst, rowlength, skiprows, skippixels, alignment;
    /* Save current pixel store state. */
    
    glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
    glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    
    CefDoMessageLoopWork();
    //CefRunMessageLoop();
    
    /* Restore current pixel store state. */
    glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
    glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}

//--------------------------------------------------------------
void ofxCEF::draw(void){
    
//    cout << "ofxCEF::draw "<< endl;
//    CefDoMessageLoopWork();
    
    // Alpha blending style. Texture values have premultiplied alpha.
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // Enable alpha blending.
    glEnable(GL_BLEND);
    
    //cout << ofGetWidth() << " - " << ofGetHeight() << endl;
    float width = ofGetWidth();
    float height = ofGetHeight();
    
    ofMesh temp;
    temp.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    temp.addVertex( ofPoint(0,0) );
    temp.addTexCoord( ofPoint(0,0) );
    temp.addVertex( ofPoint(width,0) );
    temp.addTexCoord( ofPoint(1,0) );
    temp.addVertex( ofPoint(0,height) );
    temp.addTexCoord( ofPoint(0,1) );
    temp.addVertex( ofPoint(width,height) );
    temp.addTexCoord( ofPoint(1,1) );
    ofPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, renderHandler->texture_id_);
    temp.draw();
    ofPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofxCEF::onLoadStart(){
    ofxCEFEventArgs evt;
    evt.type = ofxCEFEventArgs::onLoadStart;
    evt.httpStatusCode = -1;
    
    ofNotifyEvent(eventFromCEF, evt, this);
}

//--------------------------------------------------------------
void ofxCEF::onLoadEnd(int httpStatusCode){
    ofxCEFEventArgs evt;
    evt.type = ofxCEFEventArgs::onLoadEnd;
    evt.httpStatusCode = httpStatusCode;
    
    ofNotifyEvent(eventFromCEF, evt, this);
}

//--------------------------------------------------------------
void ofxCEF::gotMessageFromJS(string name, string type, string value){
    ofxCEFMessageArgs msg;
    msg.type = type;
    msg.name = name;
    msg.value = value;
    
    ofNotifyEvent(messageFromJS, msg, this);
}

//--------------------------------------------------------------
void ofxCEF::notificationHandler(){

#if defined(TARGET_OSX) 
	float displayScale = [[NSScreen mainScreen] backingScaleFactor];
    
    cout << " ======= ofxCEF::notificationHandler =========" << endl;
    cout << "OF window size: " << ofGetWidth() << " - " << ofGetHeight() << endl;
    cout << "Changed Screen / displayScale :: " << displayScale << " ::/ frame orig : " << [NSScreen mainScreen].frame.origin.x << " - " << [NSScreen mainScreen].frame.origin.y << " ::/ size " << [NSScreen mainScreen].frame.size.width << " - " << [NSScreen mainScreen].frame.size.height << endl;
    
    bool scaleChanged = false;
    
    if (displayScale > 1.0){
        if (!renderHandler->bIsRetinaDisplay) {
            scaleChanged = true;
        }
        renderHandler->bIsRetinaDisplay = true;
        
    } else {
        if (renderHandler->bIsRetinaDisplay) {
            scaleChanged = true;
        }
        
        renderHandler->bIsRetinaDisplay = false;
    }
    
//    if (scaleChanged) {
//        if (renderHandler->bIsRetinaDisplay) {
//            reshape(ofGetWidth()*2, ofGetHeight()*2);
//        } else{
//            reshape(ofGetWidth(), ofGetHeight());
//        }
//    }
#elif defined(TARGET_WIN32)
	
#endif

    reshape(ofGetWidth(), ofGetHeight());
    renderHandler->init();
    browser->Reload();
}

//--------------------------------------------------------------
void ofxCEF::reshape(int w, int h){
    cout << "Reshape: " << w << " - " << h << endl;
    renderHandler->reshape(w, h);
    browser->GetHost()->WasResized();
}

//--------------------------------------------------------------
void ofxCEF::mousePressed(ofMouseEventArgs &e){
    int x = e.x;
    int y = e.y;
    
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

//--------------------------------------------------------------
void ofxCEF::mouseReleased(ofMouseEventArgs &e){
    int x = e.x;
    int y = e.y;
    
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

//--------------------------------------------------------------
void ofxCEF::mouseMoved(ofMouseEventArgs &e){
    int x = e.x;
    int y = e.y;
    
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

//--------------------------------------------------------------
void ofxCEF::mouseDragged(ofMouseEventArgs &e){
    mouseMoved(e);
}

//--------------------------------------------------------------
void ofxCEF::mouseWheel(int x, int y){
    CefMouseEvent mouse_event;
    mouse_event.x = mouse_event.y = 1;
    mouse_event.modifiers = 0;
    browser->GetHost()->SendMouseWheelEvent(mouse_event, x,y);
    
}

//--------------------------------------------------------------
void ofxCEF::keyPressed(ofKeyEventArgs &e){
    //cout << "KEY:: " << e.key << " - KEYCODE:: " <<  e.keycode << " - SCANCODE::" << e.scancode << endl;
    
    CefKeyEvent event;
    
    if (e.key == OF_KEY_LEFT || e.key == OF_KEY_UP
        || e.key == OF_KEY_RIGHT || e.key == OF_KEY_DOWN
        || e.key == OF_KEY_BACKSPACE || e.key == OF_KEY_DEL) {
        
        event.native_key_code = e.scancode;
        event.type = KEYEVENT_KEYDOWN;
        
    } else {
        event.native_key_code = e.scancode;
        event.character = (char)e.key;
        event.type = KEYEVENT_CHAR;
        
    }
    
    browser->GetHost()->SendKeyEvent(event);
}

//--------------------------------------------------------------
void ofxCEF::keyReleased(ofKeyEventArgs &e){
    CefKeyEvent event;
    
    if (e.key == OF_KEY_LEFT || e.key == OF_KEY_UP
        || e.key == OF_KEY_RIGHT || e.key == OF_KEY_DOWN
        || e.key == OF_KEY_BACKSPACE || e.key == OF_KEY_DEL) {
        
        // Hack - Need to do this otherwise we loose an event.
        event.native_key_code = e.scancode;
        event.character = (char)e.key;
        event.type = KEYEVENT_CHAR;
        browser->GetHost()->SendKeyEvent(event);
        
    } else {
        event.native_key_code = e.scancode;
        event.type = KEYEVENT_KEYUP;
        browser->GetHost()->SendKeyEvent(event);
    }
}

//--------------------------------------------------------------
void ofxCEF::windowResized(ofResizeEventArgs &e){
    reshape(e.width, e.height);
    renderHandler->init();
    //cefgui->browser->Reload();
}


//--------------------------------------------------------------
void ofxCEF::executeJS(const char* command){
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    frame->ExecuteJavaScript(command, frame->GetURL(), 0);

    // TODO limit frequency of texture updating
    CefRect rect;
    renderHandler->GetViewRect(browser, rect);
    browser->GetHost()->Invalidate(PET_VIEW);
}
