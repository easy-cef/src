// george @ 2019/4/21

#include "app/easy_cef_client.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "app/shared/resource_util.h"

namespace {

EasyCefClient* g_instance = NULL;

const char kTestMessageName[] = "MessageRouterTest";

// Handle messages in the browser process.
class MessageHandler : public CefMessageRouterBrowserSide::Handler {
 public:
  explicit MessageHandler() {}

  // Called due to cefQuery execution in message_router.html.
  bool OnQuery(CefRefPtr<CefBrowser> browser,
               CefRefPtr<CefFrame> frame,
               int64 query_id,
               const CefString& request,
               bool persistent,
               CefRefPtr<Callback> callback) OVERRIDE {
    const std::string& message_name = request;
    if (message_name.find(kTestMessageName) == 0) {
      // Reverse the string and return.
      std::string result = message_name.substr(sizeof(kTestMessageName));
      std::reverse(result.begin(), result.end());
      callback->Success(result);
      return true;
    }

    return false;
  }

private:

  DISALLOW_COPY_AND_ASSIGN(MessageHandler);
};

}  // namespace

EasyCefClient::EasyCefClient(bool use_views)
    : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

EasyCefClient::~EasyCefClient() {
  g_instance = NULL;
}

// static
EasyCefClient* EasyCefClient::GetInstance() {
  return g_instance;
}

bool EasyCefClient::OnProcessMessageReceived(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {
  CEF_REQUIRE_UI_THREAD();

  return message_router_->OnProcessMessageReceived(browser, frame, source_process, message);
}

void EasyCefClient::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void EasyCefClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  if (!message_router_) {
    // Create the browser-side router for query handling.
    CefMessageRouterConfig config;
    message_router_ = CefMessageRouterBrowserSide::Create(config);

    // Register handlers with the router.
    message_handler_.reset(new MessageHandler());
    message_router_->AddHandler(message_handler_.get(), false);
  }

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool EasyCefClient::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void EasyCefClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // Free the router when the last browser is closed.
    message_router_->RemoveHandler(message_handler_.get());
    message_handler_.reset();
    message_router_ = NULL;

    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void EasyCefClient::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // // Display a load error message.
  // std::stringstream ss;
  // ss << "<html><body bgcolor=\"white\">"
  //       "<h2>Failed to load URL "
  //    << std::string(failedUrl) << " with error " << std::string(errorText)
  //    << " (" << errorCode << ").</h2></body></html>";
  // frame->LoadString(ss.str(), failedUrl);
}

void EasyCefClient::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&EasyCefClient::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

bool EasyCefClient::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefRequest> request,
                                   bool user_gesture,
                                   bool is_redirect) {
  CEF_REQUIRE_UI_THREAD();

  message_router_->OnBeforeBrowse(browser, frame);
  return false;
}

CefRefPtr<CefResourceHandler> EasyCefClient::GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request) {
  CEF_REQUIRE_IO_THREAD();

  const std::string& url = request->GetURL();
  //DLOG(INFO) << "EasyCefClient::GetResourceHandler: " << url.c_str();

  // This is a minimal implementation of resource loading. For more complex
  // usage (multiple files, zip archives, custom handlers, etc.) you might want
  // to use CefResourceManager. See the "resource_manager" target for an
  // example implementation.
  const std::string& resource_path = shared::GetResourcePath(url);
  if (!resource_path.empty())
    return shared::GetResourceHandler(resource_path);

  return NULL;
}

void EasyCefClient::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                              TerminationStatus status) {
  CEF_REQUIRE_UI_THREAD();
  DLOG(INFO) << "EasyCefClient::OnRenderProcessTerminated";

  message_router_->OnRenderProcessTerminated(browser);
}

// CefContextMenuHandler methods:
void EasyCefClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefContextMenuParams> params,
                                        CefRefPtr<CefMenuModel> model) {
  model->Clear();
}
