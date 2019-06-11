// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "app/shared/resource_util.h"

#include "include/base/cef_logging.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "app/shared/constants.h"

namespace shared {

namespace {

// Retrieve the contents of a BINARY resource from the current executable.
bool LoadBinaryResource(int binaryId, DWORD& dwSize, LPBYTE& pBytes) {
  HINSTANCE hInst = GetModuleHandle(NULL);
  HRSRC hRes =
      FindResource(hInst, MAKEINTRESOURCE(binaryId), MAKEINTRESOURCE(256));
  if (hRes) {
    HGLOBAL hGlob = LoadResource(hInst, hRes);
    if (hGlob) {
      dwSize = SizeofResource(hInst, hRes);
      pBytes = (LPBYTE)LockResource(hGlob);
      if (dwSize > 0 && pBytes)
        return true;
    }
  }

  return false;
}

bool FileExists(const char* path) {
  FILE* f = fopen(path, "rb");
  if (f) {
    fclose(f);
    return true;
  }
  return false;
}

}  // namespace

bool GetResourceDir(std::string& dir) {
  char buff[MAX_PATH] = {0};
  GetModuleFileNameA(NULL, buff, MAX_PATH);
  char* p = strrchr(buff, '\\');
  if(p)
    *p = '\0';

  dir = std::string(buff);
  dir.append("\\");
  dir.append(easycef::kDomDomain);

  return true;
}

CefRefPtr<CefStreamReader> GetResourceReaderForFile(const std::string& resource_path) {
  std::string path;
  if (!GetResourceDir(path))
    return NULL;

  path.append("\\");
  path.append(resource_path);

  if (!FileExists(path.c_str())) {
    //DLOG(INFO) << "Resource not found in file system: " << path.c_str();
    return NULL;
  }

  return CefStreamReader::CreateForFile(path);
}

CefRefPtr<CefStreamReader> GetResourceReader(const std::string& resource_path) {
  // Try finding resource in file system first.
  CefRefPtr<CefStreamReader> reader = GetResourceReaderForFile(resource_path);
  if(reader)
    return reader;

  return NULL;
}

}  // namespace shared
