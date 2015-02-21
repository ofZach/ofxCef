
#include <fstream>
#include "ofxCEFRenderHandler.h"
#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

#import <Cocoa/Cocoa.h>


//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_REV
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#endif


#define NOGLDEBUG
// DCHECK on gl errors.
#ifndef NOGLDEBUG
#define VERIFY_NO_ERROR { \
int _gl_error = glGetError(); \
DCHECK(_gl_error == GL_NO_ERROR) << \
"glGetError returned " << _gl_error; \
}
#else
#define VERIFY_NO_ERROR
#endif


//--------------------------------------------------------------
//--------------------------------------------------------------

static CefRect convertRect(const NSRect& target, const NSRect& frame) {
    NSRect rect = target;
    rect.origin.y = NSMaxY(frame) - NSMaxY(target);
    return CefRect(rect.origin.x,
                   rect.origin.y,
                   rect.size.width,
                   rect.size.height);
}


//--------------------------------------------------------------
ofxCEFRenderHandler::ofxCEFRenderHandler(){
  transparent_ = true;
  initialized = false;
  
    /*
    //http://stackoverflow.com/questions/11067066/mac-os-x-best-way-to-do-runtime-check-for-retina-display
    float displayScale = 1;
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        NSArray *screens = [NSScreen screens];
        for (int i = 0; i < [screens count]; i++) {
            float s = [[screens objectAtIndex:i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
     */
    
    float displayScale = [[NSScreen mainScreen] backingScaleFactor];
    
    if (displayScale > 1.0){
        bIsRetinaDisplay = true;
    } else {
        bIsRetinaDisplay = false;
    }
    
    texture_id_ = 0;

    show_update_rect_ = true;
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::init(void){
    
    initialized = true;
    
    if (texture_id_ != 0){
        glDeleteTextures( 1, &texture_id_ );
    }
    
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); VERIFY_NO_ERROR;
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); VERIFY_NO_ERROR;
    
    // Necessary for non-power-of-2 textures to render correctly.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); VERIFY_NO_ERROR;
    
    // Create the texture.
    glGenTextures(1, &texture_id_); VERIFY_NO_ERROR;
    DCHECK_NE(texture_id_, 0U); VERIFY_NO_ERROR;
    
    glBindTexture(GL_TEXTURE_2D, texture_id_); VERIFY_NO_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST); VERIFY_NO_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST); VERIFY_NO_ERROR;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); VERIFY_NO_ERROR;
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::reshape(int w_, int h_){
  w = w_;
  h = h_;
 
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
                                   bool show) {
    if (!show) {
        // Clear the popup rectangle.
        ClearPopupRects();
    }
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                   const CefRect& rect) {
    if (rect.width <= 0 || rect.height <= 0)
        return;
    original_popup_rect_ = rect;
    popup_rect_ = GetPopupRectInWebView(original_popup_rect_);
}

//--------------------------------------------------------------
CefRect ofxCEFRenderHandler::GetPopupRectInWebView(const CefRect& original_rect) {
    CefRect rc(original_rect);
    // if x or y are negative, move them to 0.
    if (rc.x < 0)
        rc.x = 0;
    if (rc.y < 0)
        rc.y = 0;
    // if popup goes outside the view, try to reposition origin
    if (rc.x + rc.width > view_width_)
        rc.x = view_width_ - rc.width;
    if (rc.y + rc.height > view_height_)
        rc.y = view_height_ - rc.height;
    // if x or y became negative, move them to 0 again.
    if (rc.x < 0)
        rc.x = 0;
    if (rc.y < 0)
        rc.y = 0;
    return rc;
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::ClearPopupRects() {
    popup_rect_.Set(0, 0, 0, 0);
    original_popup_rect_.Set(0, 0, 0, 0);
}

//--------------------------------------------------------------
bool ofxCEFRenderHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
                   CefScreenInfo& screen_info) {

    NSWindow* mainWnd =  (NSWindow *) ((ofAppGLFWWindow *) ofGetWindowPtr())->getCocoaWindow();
    
    NSWindow* window = mainWnd;// [view_ window];
    if (!window)
        return false;
    
    if (bIsRetinaDisplay){
        screen_info.device_scale_factor = 2.0; //[[window contentView] getDeviceScaleFactor];
    } else {
        screen_info.device_scale_factor = 1.0; //[[window contentView] getDeviceScaleFactor];
    }
    
    NSScreen* screen = [window screen];
    if (!screen)
        screen = [NSScreen deepestScreen];
    
    screen_info.depth = NSBitsPerPixelFromDepth([screen depth]);
    screen_info.depth_per_component = NSBitsPerSampleFromDepth([screen depth]);
    screen_info.is_monochrome =
    [[screen colorSpace] colorSpaceModel] == NSGrayColorSpaceModel;
    // screen_info.is_monochrome = true;
    screen_info.rect = convertRect([screen frame], [screen frame]);
    screen_info.available_rect =
    convertRect([screen visibleFrame], [screen frame]);
    
    
    return true;
}

//--------------------------------------------------------------
bool ofxCEFRenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
    if (bIsRetinaDisplay){
        rect = CefRect(0,0, ofGetWidth()*0.5, ofGetHeight()*0.5);
    } else {
        rect = CefRect(0,0, ofGetWidth(), ofGetHeight());
    }

  return true;
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::render() {
    
    //return;
    
    if (w == 0 || h == 0)
        return;
    
    DCHECK(initialized);
    
    struct {
        float tu, tv;
        float x, y, z;
    } static vertices[] = {
        {0.0f, 1.0f, -1.0f, -1.0f, 0.0f},
        {1.0f, 1.0f,  1.0f, -1.0f, 0.0f},
        {1.0f, 0.0f,  1.0f,  1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f,  1.0f, 0.0f}
    };
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); VERIFY_NO_ERROR;
    
    glMatrixMode(GL_MODELVIEW); VERIFY_NO_ERROR;
    glLoadIdentity(); VERIFY_NO_ERROR;
    
    // Match GL units to screen coordinates.
    glViewport(0, 0, w, h); VERIFY_NO_ERROR;
    glMatrixMode(GL_PROJECTION); VERIFY_NO_ERROR;
    glLoadIdentity(); VERIFY_NO_ERROR;
    
    // Draw the background gradient.
    glPushAttrib(GL_ALL_ATTRIB_BITS); VERIFY_NO_ERROR;
    // Don't check for errors until glEnd().
    glBegin(GL_QUADS);
    glColor4f(1.0, 0.0, 0.0, 1.0);  // red
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glColor4f(0.0, 0.0, 1.0, 1.0);  // blue
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd(); VERIFY_NO_ERROR;
    glPopAttrib(); VERIFY_NO_ERROR;
    
//    // Rotate the view based on the mouse spin.
//    if (spin_x_ != 0) {
//        glRotatef(-spin_x_, 1.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
//    }
//    if (spin_y_ != 0) {
//        glRotatef(-spin_y_, 0.0f, 1.0f, 0.0f); VERIFY_NO_ERROR;
//    }
    
    if (transparent_) {
        // Alpha blending style. Texture values have premultiplied alpha.
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); VERIFY_NO_ERROR;
        
        // Enable alpha blending.
        glEnable(GL_BLEND); VERIFY_NO_ERROR;
    }
    
    // Enable 2D textures.
    glEnable(GL_TEXTURE_2D); VERIFY_NO_ERROR;
    
    // Draw the facets with the texture.
    DCHECK_NE(texture_id_, 0U); VERIFY_NO_ERROR;
    glBindTexture(GL_TEXTURE_2D, texture_id_); VERIFY_NO_ERROR;
    glInterleavedArrays(GL_T2F_V3F, 0, vertices); VERIFY_NO_ERROR;
    glDrawArrays(GL_QUADS, 0, 4); VERIFY_NO_ERROR;
    
    // Disable 2D textures.
    glDisable(GL_TEXTURE_2D); VERIFY_NO_ERROR;
    
    if (transparent_) {
        // Disable alpha blending.
        glDisable(GL_BLEND); VERIFY_NO_ERROR;
    }
    
    // Draw a rectangle around the update region.
    if (show_update_rect_ && !update_rect_.IsEmpty()) {
        int left = update_rect_.x;
        int right = update_rect_.x + update_rect_.width;
        int top = update_rect_.y;
        int bottom = update_rect_.y + update_rect_.height;
        
        
#if defined(OS_LINUX)
        // Shrink the box so that top & right sides are drawn.
        top += 1;
        right -= 1;
#else
        // Shrink the box so that left & bottom sides are drawn.
        left += 1;
        bottom -= 1;
#endif
        
        glPushAttrib(GL_ALL_ATTRIB_BITS); VERIFY_NO_ERROR
        glMatrixMode(GL_PROJECTION); VERIFY_NO_ERROR;
        glPushMatrix(); VERIFY_NO_ERROR;
        glLoadIdentity(); VERIFY_NO_ERROR;
        glOrtho(0, view_width_, view_height_, 0, 0, 1); VERIFY_NO_ERROR;
        
        glLineWidth(1); VERIFY_NO_ERROR;
        glColor3f(1.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
        // Don't check for errors until glEnd().
        glBegin(GL_LINE_STRIP);
        glVertex2i(left, top);
        glVertex2i(right, top);
        glVertex2i(right, bottom);
        glVertex2i(left, bottom);
        glVertex2i(left, top);
        glEnd(); VERIFY_NO_ERROR;
        
        glPopMatrix(); VERIFY_NO_ERROR;
        glPopAttrib(); VERIFY_NO_ERROR;
    }
}

//--------------------------------------------------------------
void ofxCEFRenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList &dirtyRects,
                            const void* buffer,
                            int width,
                            int height){

    if (!initialized)
        init();
    
    if (transparent_) {
        // Enable alpha blending.
        glEnable(GL_BLEND); VERIFY_NO_ERROR;
    }
    
    // Necessary for non-power-of-2 textures to render correctly.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); VERIFY_NO_ERROR;
    
    // Enable 2D textures.
    glEnable(GL_TEXTURE_2D); VERIFY_NO_ERROR;
    
    DCHECK_NE(texture_id_, 0U);
    glBindTexture(GL_TEXTURE_2D, texture_id_); VERIFY_NO_ERROR;
    
    if (type == PET_VIEW) {
        int old_width = view_width_;
        int old_height = view_height_;
        
        view_width_ = width;
        view_height_ = height;
        
        
        if (show_update_rect_)
            update_rect_ = dirtyRects[0];
        
        glPixelStorei(GL_UNPACK_ROW_LENGTH, view_width_); VERIFY_NO_ERROR;
        
        if (old_width != view_width_ || old_height != view_height_ ||
            (dirtyRects.size() == 1 &&
             dirtyRects[0] == CefRect(0, 0, view_width_, view_height_))) {
                // Update/resize the whole texture.
                glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); VERIFY_NO_ERROR;
                glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); VERIFY_NO_ERROR;
                glTexImage2D(
                             GL_TEXTURE_2D, 0, GL_RGBA, view_width_, view_height_, 0,
                             GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
            } else {
                // Update just the dirty rectangles.
                CefRenderHandler::RectList::const_iterator i = dirtyRects.begin();
                for (; i != dirtyRects.end(); ++i) {
                    
                    const CefRect& rect = *i;
                    ///printf("%i %i %i %i \n", rect.x, rect.y, rect.width,
                    //    rect.height);
                    
                    glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect.x); VERIFY_NO_ERROR;
                    glPixelStorei(GL_UNPACK_SKIP_ROWS, rect.y); VERIFY_NO_ERROR;
                    glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width,
                                    rect.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                                    buffer); VERIFY_NO_ERROR;
                }
            }
    } else if (type == PET_POPUP && popup_rect_.width > 0 &&
               popup_rect_.height > 0) {
        int skip_pixels = 0, x = popup_rect_.x;
        int skip_rows = 0, y = popup_rect_.y;
        int w = width;
        int h = height;
        
        // Adjust the popup to fit inside the view.
        if (x < 0) {
            skip_pixels = -x;
            x = 0;
        }
        if (y < 0) {
            skip_rows = -y;
            y = 0;
        }
        if (x + w > view_width_)
            w -= x + w - view_width_;
        if (y + h > view_height_)
            h -= y + h - view_height_;
        
        // Update the popup rectangle.
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width); VERIFY_NO_ERROR;
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, skip_pixels); VERIFY_NO_ERROR;
        glPixelStorei(GL_UNPACK_SKIP_ROWS, skip_rows); VERIFY_NO_ERROR;
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA,
                        GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
    }
    
    // Disable 2D textures.
    glDisable(GL_TEXTURE_2D); VERIFY_NO_ERROR;
    
    if (transparent_) {
        // Disable alpha blending.
        glDisable(GL_BLEND); VERIFY_NO_ERROR;
    }
}
