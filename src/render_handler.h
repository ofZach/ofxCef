#include "ofMain.h"

#include "cef_render_handler.h"

#ifndef RENDER_HANDLER_H
#define RENDER_HANDLER_H

//--------------------------------------------------------------
class RenderHandler : public CefRenderHandler
{
  
public:
    
    RenderHandler();

    bool initialized;
    bool bIsRetinaDisplay;

    void init(void);
    void draw(void);
    void reshape(int, int);
    void render();
    

    bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info);
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);
    void OnPaint(CefRefPtr<CefBrowser> browser,
                 PaintElementType type,
                 const RectList &dirtyRects,
                 const void* buffer,
                 int width,
                 int height);
    const CefRect& popup_rect() const { return popup_rect_; }
    const CefRect& original_popup_rect() const { return original_popup_rect_; }
    CefRect GetPopupRectInWebView(const CefRect& original_rect);
    void ClearPopupRects();
    void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show);
    void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect);
    
  //private:
    
    CefRect popup_rect_;
    CefRect original_popup_rect_;
    
    int w, h;
    bool transparent_;
    bool show_update_rect_;
    bool initialized_;
    unsigned int texture_id_;
    int view_width_;
    int view_height_;
    CefRect update_rect_;
    

    IMPLEMENT_REFCOUNTING(RenderHandler);

};

#endif
