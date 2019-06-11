// george @ 2019/4/21

#include "app/easy_cef_app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "app/shared/constants.h"
#include "app/shared/resource_util.h"
#include "app/easy_cef_client.h"
#include "app/easy_cef_scheme_handler.h"


EasyCefApp::EasyCefApp() {}

void EasyCefApp::OnRegisterCustomSchemes(
  CefRawPtr<CefSchemeRegistrar> registrar) {
  registrar->AddCustomScheme(
    easycef::kEasyCefScheme, CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
}

void EasyCefApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();

  const bool use_views = false;

  // EasyCefClient implements browser-level callbacks.
  CefRefPtr<EasyCefClient> client(new EasyCefClient(use_views));

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  std::string url;

  // Check if a "--url=" value was provided via the command-line. If so, use
  // that instead of the default URL.
  url = command_line->GetSwitchValue("url");
  
  // Determining startup point
  if (url.empty()) {
    std::string app_json_data;
    if(shared::LoadResourceData(easycef::kAppConfigFile, app_json_data)) {
      cef_json_parser_error_t error_code;
      CefString error_msg;
      CefRefPtr<CefValue> value =
          CefParseJSONAndReturnError(
            app_json_data, 
            JSON_PARSER_RFC, 
            error_code, 
            error_msg);
      if(value.get()) {
        CefRefPtr<CefDictionaryValue> app_json =
          value->GetDictionary();
        std::string startup_url = 
          app_json->GetString(easycef::kKeyStartupUrl);
        if(!startup_url.empty())
          url = startup_url;
      }
    }
  }

  // Fall back to default entry
  if (url.empty()) {
    url = easycef::kEasyCefScheme;
    url.append("://");
    url.append(easycef::kDomDomain);
    url.append("/");
    url.append(easycef::kDefaultEntryPage);
  }

  // Information used when creating the native window.
  CefWindowInfo window_info;

#if defined(OS_WIN)
  // On Windows we need to specify certain flags that will be passed to
  // CreateWindowEx().
  window_info.SetAsPopup(NULL, "easy_cef_app");
#endif

  // Register scheme handler.
  easycef::RegisterEasyCefSchemeHandler();

  // Create the first browser window.
  CefBrowserHost::CreateBrowser(window_info, client, url, browser_settings, NULL, NULL);
}


namespace shared {

CefRefPtr<CefApp> CreateBrowserProcessApp() {
  return new EasyCefApp();
}

}  // namespace shared
