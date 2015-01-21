#include "ofxCEF.h"
#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include <Cocoa/Cocoa.h>
#include "ofxCEFClientApp.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
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

//--------------------------------------------------------------
//--------------------------------------------------------------
ofxCEF* initofxCEF(int argc, char** argv)
{
    CefMainArgs args(argc, argv);
    //CefExecuteProcess(args, 0, NULL);

    CefSettings settings;
    settings.background_color = 0xFFFF00FF;
    //settings.single_process = true;

    CefRefPtr<ofxCEFClientApp> app(new ofxCEFClientApp);

    CefInitialize(args, settings, app.get(), NULL);

    return new ofxCEF();
}

//--------------------------------------------------------------
ofxCEF::ofxCEF()
{
    
    CefWindowInfo windowInfo;
    CefBrowserSettings settings;

    NSWindow * cocoaWindow =  (NSWindow *) ((ofAppGLFWWindow *) ofGetWindowPtr())->getCocoaWindow();
    NSView * view =  [ cocoaWindow contentView];

    windowInfo.SetAsWindowless(view, true);
    windowInfo.transparent_painting_enabled = 1;
    //windowInfo.SetAsChild(view, 0, 0, 1000, 1000);

    renderHandler = new ofxCEFRenderHandler();
    
    if (renderHandler->bIsRetinaDisplay) {
        ofSetWindowPosition(0, 0);
        ofSetWindowShape(ofGetWidth(), ofGetHeight());
    }

    client = new ofxCEFBrowserClient(renderHandler);
    browser = CefBrowserHost::CreateBrowserSync(windowInfo, client.get(), "", settings, 0);
    
    if (renderHandler->bIsRetinaDisplay) {
        reshape(ofGetWidth()*2, ofGetHeight()*2);
    }
    
    enableEvents();
    
    // Listener to get a notification when the app window switches screen
    //NSNotificationManager *nsNotificationManager = [[NSNotificationManager alloc] initWithObserver:this method:&ofxCEF::notificationHandler];
}

//--------------------------------------------------------------
ofxCEF::~ofxCEF()
{
}

//--------------------------------------------------------------
void ofxCEF::enableEvents()
{
    ofAddListener(ofEvents().keyPressed, this, &ofxCEF::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxCEF::keyReleased);
}

//--------------------------------------------------------------
void ofxCEF::load(const char* url)
{
    if (!renderHandler->initialized) {
        renderHandler->init();
    }

    browser->GetMainFrame()->LoadURL(url);
}

//--------------------------------------------------------------
void ofxCEF::draw(void)
{
    
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
    
    
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofxCEF::notificationHandler()
{
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

    reshape(ofGetWidth(), ofGetHeight());
    renderHandler->init();
    browser->Reload();
}

//--------------------------------------------------------------
void ofxCEF::reshape(int w, int h)
{
    cout << "Reshape: " << w << " - " << h << endl;
    renderHandler->reshape(w, h);
    browser->GetHost()->WasResized();
}

//--------------------------------------------------------------
void ofxCEF::mouseMove(int x, int y)
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

//--------------------------------------------------------------
void ofxCEF::mouseWheel(int x, int y)
{
    CefMouseEvent mouse_event;
    mouse_event.x = mouse_event.y = 1;
    mouse_event.modifiers = 0;
    browser->GetHost()->SendMouseWheelEvent(mouse_event, x,y);
    
}

//--------------------------------------------------------------
void ofxCEF::mousePressed(int x, int y)
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

//--------------------------------------------------------------
void ofxCEF::mouseReleased(int x, int y)
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

//--------------------------------------------------------------
void ofxCEF::keyPressed(ofKeyEventArgs &e)
{
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
void ofxCEF::keyReleased(ofKeyEventArgs &e)
{
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
void ofxCEF::executeJS(const char* command)
{
    CefRefPtr<CefFrame> frame = browser->GetMainFrame();
    frame->ExecuteJavaScript(command, frame->GetURL(), 0);

    // TODO limit frequency of texture updating
    CefRect rect;
    renderHandler->GetViewRect(browser, rect);
    browser->GetHost()->Invalidate(PET_VIEW);
}
