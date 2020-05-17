// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "app/shared/resource_util.h"

#include <stdio.h>

namespace shared {

namespace {

bool FileExists(const char* path) {
  FILE* f = fopen(path, "rb");
  if (f) {
    fclose(f);
    return true;
  }
  return false;
}

}  // namespace

CefRefPtr<CefStreamReader> GetResourceReader(const std::string& resource_path) {
  std::string path;
  if (!GetResourceDir(path))
    return NULL;

  path.append("/");
  path.append(resource_path);

  if (!FileExists(path.c_str()))
    return NULL;

  return CefStreamReader::CreateForFile(path);
}

}  // namespace shared
