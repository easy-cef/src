// george @ 2019/4/28

#include "app/easy_cef_client.h"

#import <Cocoa/Cocoa.h>

#include "include/cef_browser.h"

void EasyCefClient::PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                                        const CefString& title) {
  NSView* view = (NSView*)browser->GetHost()->GetWindowHandle();
  NSWindow* window = [view window];
  std::string titleStr(title);
  NSString* str = [NSString stringWithUTF8String:titleStr.c_str()];
  [window setTitle:str];
}
