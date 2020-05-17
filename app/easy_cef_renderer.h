// george @ 2019/4/21

#ifndef EASY_CEF_APP_EASY_CEF_RENDERER_H_
#define EASY_CEF_APP_EASY_CEF_RENDERER_H_

#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"

// Implement of CefApp for the renderer process.
class EasyCefRenderer : public CefApp, public CefRenderProcessHandler {
public:
  EasyCefRenderer();

  // CefApp methods:
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
      OVERRIDE {
    return this;
  }
  virtual void OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar) OVERRIDE;

  // CefRenderProcessHandler methods:
  virtual void OnWebKitInitialized() OVERRIDE;
  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) OVERRIDE;
  virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) OVERRIDE;
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
  // Handles the renderer side of query routing.
  CefRefPtr<CefMessageRouterRendererSide> message_router_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(EasyCefRenderer);
  DISALLOW_COPY_AND_ASSIGN(EasyCefRenderer);
};

#endif  // EASY_CEF_APP_EASY_CEF_RENDERER_H_
