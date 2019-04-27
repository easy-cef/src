// george @ 2019/4/21

#ifndef EASY_CEF_APP_EASY_CEF_APP_H_
#define EASY_CEF_APP_EASY_CEF_APP_H_

#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class EasyCefApp : public CefApp, public CefBrowserProcessHandler {
public:
  EasyCefApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE {
    return this;
  }

  virtual void OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar) OVERRIDE;

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(EasyCefApp);
  DISALLOW_COPY_AND_ASSIGN(EasyCefApp);
};

#endif  // EASY_CEF_APP_EASY_CEF_APP_H_
