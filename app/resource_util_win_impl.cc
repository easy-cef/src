// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "app/shared/resource_util.h"

#include "app/resource.h"

namespace shared {

int GetResourceId(const std::string& resource_path) {
  if (resource_path == "index.html")
    return IDS_INTERNAL_DOM_INDEX_HTML;
  else if (resource_path == "index.css")
    return IDS_INTERNAL_DOM_INDEX_CSS;
  else if (resource_path == "index.js")
    return IDS_INTERNAL_DOM_INDEX_JS;
  return 0;
}

}  // namespace shared
