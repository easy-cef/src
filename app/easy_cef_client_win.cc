// george @ 2019/4/21

#include "app/easy_cef_client.h"

#include <windows.h>
#include <string>

#include "include/cef_browser.h"

void EasyCefClient::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                        const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  SetWindowText(hwnd, std::wstring(title).c_str());
}
