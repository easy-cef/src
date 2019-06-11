// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "app/shared/resource_util.h"

#include "include/cef_parser.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "app/shared/constants.h"

namespace shared {

namespace {

#define BUF_SIZE (4096)

// Returns |url| without the query or fragment components, if any.
std::string GetUrlWithoutQueryOrFragment(const std::string& url) {
  // Find the first instance of '?' or '#'.
  const size_t pos = std::min(url.find('?'), url.find('#'));
  if (pos != std::string::npos)
    return url.substr(0, pos);

  return url;
}

}  // namespace

std::string GetResourcePath(const std::string& url) {
  static std::string easycef_dom_origin;
  if(easycef_dom_origin.empty()) {
    easycef_dom_origin.append(easycef::kEasyCefScheme);
    easycef_dom_origin.append("://");
    easycef_dom_origin.append(easycef::kDomDomain);
    easycef_dom_origin.append("/");
  }
  std::string origin;
  if (url.find(easycef_dom_origin.c_str()) == 0)
    origin = easycef_dom_origin;
  else
    return std::string();

  const std::string& url_no_query = GetUrlWithoutQueryOrFragment(url);
  return url_no_query.substr(origin.size());
}

// Determine the mime type based on the |file_path| file extension.
std::string GetMimeType(const std::string& resource_path) {
  std::string mime_type;
  size_t sep = resource_path.find_last_of(".");
  if (sep != std::string::npos) {
    mime_type = CefGetMimeType(resource_path.substr(sep + 1));
    if (!mime_type.empty())
      return mime_type;
  }
  return "text/html";
}

CefRefPtr<CefResourceHandler> GetResourceHandler(
    const std::string& resource_path) {
  CefRefPtr<CefStreamReader> reader = GetResourceReader(resource_path);
  if (!reader)
    return NULL;

  return new CefStreamResourceHandler(GetMimeType(resource_path), reader);
}

bool LoadResourceData(const std::string& resource_path,
                      std::string& out_data) {
  if(resource_path.empty())
    return false;

  std::string dom_path;
  if(!shared::GetResourceDir(dom_path))
    return false;

  dom_path.append("/");
  dom_path.append(resource_path);
  CefRefPtr<CefStreamReader> reader = 
    CefStreamReader::CreateForFile(dom_path);
  if(!reader.get())
    return false;

  out_data.clear();
  static char buffer[BUF_SIZE] = {0};
  size_t bytes_read = 0;
  do {
    bytes_read = reader->Read(buffer, 1, sizeof(buffer));
    if(bytes_read > 0) {
      buffer[bytes_read] = '\0';
      out_data.append(buffer);
    }
  } while (bytes_read > 0);

  return true;
}

}  // namespace shared
