// george @ 2019/4/21

#include "app/easy_cef_handler.h"

#include <windows.h>
#include <string>

#include "include/cef_browser.h"

void EasyCefHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                        const CefString& title) {
  CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
  SetWindowText(hwnd, std::wstring(title).c_str());
}
