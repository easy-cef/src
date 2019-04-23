// george @ 2019/4/21

#include "app/easy_cef_app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "app/easy_cef_handler.h"


EasyCefApp::EasyCefApp() {}

void EasyCefApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();

  const bool use_views = false;

  // EasyCefHandler implements browser-level callbacks.
  CefRefPtr<EasyCefHandler> handler(new EasyCefHandler(use_views));

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  std::string url;

  // Check if a "--url=" value was provided via the command-line. If so, use
  // that instead of the default URL.
  url = command_line->GetSwitchValue("url");
  if (url.empty())
    url = "https://www.bing.com";

  // Information used when creating the native window.
  CefWindowInfo window_info;

#if defined(OS_WIN)
  // On Windows we need to specify certain flags that will be passed to
  // CreateWindowEx().
  window_info.SetAsPopup(NULL, "easy_cef_app");
#endif

  // Create the first browser window.
  CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, NULL);
}
