# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# Sources shared by all executables.
set(SHARED_COMMON_SRCS
  shared/app_factory.h
  shared/main_util.cc
  shared/main_util.h
  shared/constants.cc
  shared/constants.h
  )
# easy_cef_renderer sources.
set(EASY_CEF_RENDERER_SRCS
  easy_cef_renderer.cc
  easy_cef_renderer.h
  )
# easy_cef_app sources.
set(EASY_CEF_APP_SRCS
  ${SHARED_COMMON_SRCS}
  ${EASY_CEF_RENDERER_SRCS}
  easy_cef_app.cc
  easy_cef_app.h
  easy_cef_client.cc
  easy_cef_client.h
  easy_cef_scheme_handler.cc
  easy_cef_scheme_handler.h
  shared/client_manager.cc
  shared/client_manager.h
  shared/resource_util.cc
  shared/resource_util.h
  )
set(EASY_CEF_APP_SRCS_LINUX
  easy_cef_app_linux.cc
  easy_cef_client_linux.cc
  shared/resource_util_linux.cc
  shared/resource_util_posix.cc
  )
set(EASY_CEF_APP_SRCS_MACOSX
  easy_cef_app_mac.mm
  easy_cef_client_mac.mm
  shared/resource_util_mac.mm
  shared/resource_util_posix.cc
  )
set(EASY_CEF_APP_SRCS_WINDOWS
  win/easy_cef_app.exe.manifest
  win/easy_cef_app.rc
  win/easy_cef_app_win.cc
  win/easy_cef_client_win.cc
  win/resource.h
  win/resource_util_win_impl.cc
  shared/resource_util_win.cc
  )
APPEND_PLATFORM_SOURCES(EASY_CEF_APP_SRCS)
source_group(easy_cef_app FILES ${EASY_CEF_APP_SRCS})

set(EASY_CEF_APP_RES_SRCS_WINDOWS
  res/win/easy_cef.ico
  res/win/small.ico
  )
APPEND_PLATFORM_SOURCES(EASY_CEF_APP_RES_SRCS)
source_group(easy_cef_app\\\\res FILES ${EASY_CEF_APP_RES_SRCS})

set(EASY_CEF_APP_SRCS
  ${EASY_CEF_APP_SRCS}
  ${EASY_CEF_APP_RES_SRCS}
  )

# easy_cef_app helper sources.
set(EASY_CEF_APP_HELPER_SRCS_MACOSX
  ${SHARED_COMMON_SRCS}
  ${EASY_CEF_RENDERER_SRCS}
  shared/process_helper_mac.cc
  )
APPEND_PLATFORM_SOURCES(EASY_CEF_APP_HELPER_SRCS)
source_group(easy_cef_app FILES ${EASY_CEF_APP_HELPER_SRCS})

# easy_cef_app resources.
set(EASY_CEF_APP_RESOURCES_MAC_SRCS_MACOSX
  res/mac/Info.plist
  res/mac/easy_cef.icns
  )
APPEND_PLATFORM_SOURCES(EASY_CEF_APP_RESOURCES_MAC_SRCS)
source_group(easy_cef_app\\\\res\\\\mac FILES ${EASY_CEF_APP_RESOURCES_MAC_SRCS})

set(EASY_CEF_APP_RESOURCES_MAC_ENGLISH_LPROJ_SRCS_MACOSX
  res/mac/English.lproj/InfoPlist.strings
  res/mac/English.lproj/MainMenu.xib
  )
APPEND_PLATFORM_SOURCES(EASY_CEF_APP_RESOURCES_MAC_ENGLISH_LPROJ_SRCS)
source_group(easy_cef_app\\\\res\\\\mac\\\\English.lproj FILES ${EASY_CEF_APP_RESOURCES_MAC_ENGLISH_LPROJ_SRCS})

set(EASY_CEF_APP_RESOURCES_SRCS
  ${EASY_CEF_APP_RESOURCES_MAC_SRCS}
  ${EASY_CEF_APP_RESOURCES_MAC_ENGLISH_LPROJ_SRCS}
  )


#
# Shared configuration.
#

# Target executable names.
set(CEF_TARGET "easy_cef_app")
if(OS_MACOSX)
  set(CEF_HELPER_TARGET "easy_cef_app_Helper")
  set(CEF_HELPER_OUTPUT_NAME "easy_cef_app Helper")
else()
  # Logical target used to link the libcef library.
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()


#
# Linux configuration.
#

if(OS_LINUX)
  # Executable target.
  add_executable(${CEF_TARGET} ${EASY_CEF_APP_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  # Set rpath so that libraries can be placed next to the executable.
  set_target_properties(${CEF_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
  set_target_properties(${CEF_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  set_target_properties(${CEF_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

  # Set SUID permissions on the chrome-sandbox target.
  SET_LINUX_SUID_PERMISSIONS("${CEF_TARGET}" "${CEF_TARGET_OUT_DIR}/chrome-sandbox")
endif()


#
# Mac OS X configuration.
#

if(OS_MACOSX)
  # Output paths for the app bundles.
  set(CEF_APP "${CEF_TARGET_OUT_DIR}/${CEF_TARGET}.app")
  set(CEF_HELPER_APP "${CEF_TARGET_OUT_DIR}/${CEF_HELPER_OUTPUT_NAME}.app")

  # Variable referenced from Info.plist files.
  set(PRODUCT_NAME "${CEF_TARGET}")

  # Helper executable target.
  add_executable(${CEF_HELPER_TARGET} MACOSX_BUNDLE ${EASY_CEF_APP_HELPER_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_HELPER_TARGET})
  add_dependencies(${CEF_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${CEF_HELPER_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
  set_target_properties(${CEF_HELPER_TARGET} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/mac/helper-Info.plist
    OUTPUT_NAME ${CEF_HELPER_OUTPUT_NAME}
    )

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${CEF_HELPER_TARGET} cef_sandbox_lib)
  endif()

  # Main executable target.
  add_executable(${CEF_TARGET} MACOSX_BUNDLE ${EASY_CEF_APP_RESOURCES_SRCS} ${EASY_CEF_APP_SRCS})
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper "${CEF_HELPER_TARGET}")
  target_link_libraries(${CEF_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
  set_target_properties(${CEF_TARGET} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/mac/Info.plist
    )

  # Copy files into the main app bundle.
  add_custom_command(
    TARGET ${CEF_TARGET}
    POST_BUILD
    # Copy the helper app bundle into the Frameworks directory.
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_HELPER_APP}"
            "${CEF_APP}/Contents/Frameworks/${CEF_HELPER_OUTPUT_NAME}.app"
    # Copy the CEF framework into the Frameworks directory.
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
            "${CEF_APP}/Contents/Frameworks/Chromium Embedded Framework.framework"
    VERBATIM
    )

  # Manually process and copy over resource files.
  # The Xcode generator can support this via the set_target_properties RESOURCE
  # directive but that doesn't properly handle nested resource directories.
  # Remove these prefixes from input file paths.
  set(PREFIXES "mac/")
  COPY_MACOSX_RESOURCES("${EASY_CEF_APP_RESOURCES_SRCS}" "${PREFIXES}" "${CEF_TARGET}" "${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_APP}")
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
  # Executable target.
  add_executable(${CEF_TARGET} WIN32 ${EASY_CEF_APP_SRCS})
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${CEF_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
  endif()

  # Add the custom manifest files to the executable.
  ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}/win" "${CEF_TARGET}" "exe")

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
endif()
