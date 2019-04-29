// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef EASY_CEF_APP_SHARED_RESOURCE_UTIL_H_
#define EASY_CEF_APP_SHARED_RESOURCE_UTIL_H_

#include <string>

#include "include/cef_resource_handler.h"
#include "include/cef_stream.h"

#if defined(OS_WIN)
#include "include/wrapper/cef_resource_manager.h"
#endif

namespace shared {

// This file provides functionality for resource loading. On Linux amd macOS
// resource files are loaded from a directory on the filesystem
// ("<target>_files" sub-directory on Linux, app bundle Resources directory on
// macOS).

// Origin for loading local test resources.
extern const char kTestOrigin[];

// Retrieve the directory containing resource files.
bool GetResourceDir(std::string& dir);

// Returns the resource path for |url|. Removes fragment and/or query component
// if it exists. The URL must start with kTestOrigin.
std::string GetResourcePath(const std::string& url);

// Determine the mime type based on |resource_path|'s file extension.
std::string GetMimeType(const std::string& resource_path);

// Retrieve |resource_path| contents as a CefStreamReader. Returns NULL if the
// resource is not found.
CefRefPtr<CefStreamReader> GetResourceReader(const std::string& resource_path);

// Retrieve |resource_path| contents as a CefResourceHandler. Returns NULL if
// the resource is not found.
CefRefPtr<CefResourceHandler> GetResourceHandler(
    const std::string& resource_path);

}  // namespace shared

#endif  // EASY_CEF_APP_SHARED_RESOURCE_UTIL_H_
