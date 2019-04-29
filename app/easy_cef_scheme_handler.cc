// george @ 2019/4/27

#include "app/easy_cef_scheme_handler.h"

#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_request.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

#include "app/shared/constants.h"
#include "app/shared/resource_util.h"

namespace easycef {
namespace {

#define BUF_SIZE (4096)

// Implementation of the scheme handler for easycef:// requests.
class EasyCefSchemeHandler : public CefResourceHandler {
 public:
  EasyCefSchemeHandler() : offset_(0) {}

  bool ProcessRequest(CefRefPtr<CefRequest> request,
                      CefRefPtr<CefCallback> callback) OVERRIDE {
    CEF_REQUIRE_IO_THREAD();

    bool handled = false;
    std::string url = request->GetURL();
    //DLOG(INFO) << "ProcessRequest: " << url.c_str();
    std::string dom_path;
    std::string resource_path = shared::GetResourcePath(url);
    mime_type_ = shared::GetMimeType(resource_path);
    if (!resource_path.empty() && shared::GetResourceDir(dom_path)) {
      dom_path.append("/");
      dom_path.append(resource_path);
      CefRefPtr<CefStreamReader> reader = 
        CefStreamReader::CreateForFile(dom_path);
      data_.clear();
      if(reader.get()) {
        static char buffer[BUF_SIZE] = {0};
        size_t bytes_read = 0;
        handled = true;
        do {
          bytes_read = reader->Read(buffer, 1, sizeof(buffer));
          if(bytes_read > 0) {
            buffer[bytes_read] = '\0';
            data_.append(buffer);
          }
        } while (bytes_read > 0);
      }
    }

    if (handled) {
      // Indicate that the headers are available.
      callback->Continue();
      return true;
    }

    return false;
  }

  void GetResponseHeaders(CefRefPtr<CefResponse> response,
                          int64& response_length,
                          CefString& redirectUrl) OVERRIDE {
    CEF_REQUIRE_IO_THREAD();

    DCHECK(!data_.empty());

    response->SetMimeType(mime_type_);
    response->SetStatus(200);

    // Set the resulting response length.
    response_length = data_.length();
  }

  void Cancel() OVERRIDE { CEF_REQUIRE_IO_THREAD(); }

  bool ReadResponse(void* data_out,
                    int bytes_to_read,
                    int& bytes_read,
                    CefRefPtr<CefCallback> callback) OVERRIDE {
    CEF_REQUIRE_IO_THREAD();

    bool has_data = false;
    bytes_read = 0;

    if (offset_ < data_.length()) {
      // Copy the next block of data into the buffer.
      int transfer_size =
          std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
      memcpy(data_out, data_.c_str() + offset_, transfer_size);
      offset_ += transfer_size;

      bytes_read = transfer_size;
      has_data = true;
    }

    return has_data;
  }

 private:
  std::string data_;
  std::string mime_type_;
  size_t offset_;

  IMPLEMENT_REFCOUNTING(EasyCefSchemeHandler);
  DISALLOW_COPY_AND_ASSIGN(EasyCefSchemeHandler);
};

// Implementation of the factory for creating scheme handlers.
class EasyCefSchemeHandlerFactory : public CefSchemeHandlerFactory {
 public:
  EasyCefSchemeHandlerFactory() {}

  // Return a new scheme handler instance to handle the request.
  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString& scheme_name,
                                       CefRefPtr<CefRequest> request) OVERRIDE {
    CEF_REQUIRE_IO_THREAD();
    return new EasyCefSchemeHandler();
  }

 private:
  IMPLEMENT_REFCOUNTING(EasyCefSchemeHandlerFactory);
  DISALLOW_COPY_AND_ASSIGN(EasyCefSchemeHandlerFactory);
};

}  // namespace


void RegisterEasyCefSchemeHandler() {
  CefRegisterSchemeHandlerFactory(kEasyCefScheme, kDomDomain,
                                  new EasyCefSchemeHandlerFactory());
}

}  // namespace easycef
