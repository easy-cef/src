// Copyright (c) 2017 The Chromium Embedded Framework Authors.
// Portions copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "app/shared/resource_util.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "app/shared/constants.h"

namespace shared {

bool GetResourceDir(std::string& dir) {
  char buff[1024];

  // Retrieve the executable path.
  ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
  if (len == -1)
    return false;

  buff[len] = 0;
  // trim exe name
  char* p = strrchr(buff, '/');
  if(p)
    *p = '\0';

  dir = std::string(buff);
  dir.append("/");
  dir.append(easycef::kDomDomain);

  return true;
}

}  // namespace shared
