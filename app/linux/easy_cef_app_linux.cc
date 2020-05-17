// george @ 2019/4/29

#include <X11/Xlib.h>

#include "include/base/cef_logging.h"

#include "app/shared/app_factory.h"
#include "app/shared/client_manager.h"
#include "app/shared/main_util.h"

namespace {

int XErrorHandlerImpl(Display* display, XErrorEvent* event) {
  LOG(WARNING) << "X error received: "
               << "type " << event->type << ", "
               << "serial " << event->serial << ", "
               << "error_code " << static_cast<int>(event->error_code) << ", "
               << "request_code " << static_cast<int>(event->request_code)
               << ", "
               << "minor_code " << static_cast<int>(event->minor_code);
  return 0;
}

int XIOErrorHandlerImpl(Display* display) {
  return 0;
}

}  // namespace

// Entry point function for all processes.
int main(int argc, char* argv[]) {
  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Create a temporary CommandLine object.
  CefRefPtr<CefCommandLine> command_line = shared::CreateCommandLine(main_args);

  // Create a CefApp of the correct process type.
  CefRefPtr<CefApp> app;
  switch (shared::GetProcessType(command_line)) {
    case shared::PROCESS_TYPE_BROWSER:
      app = shared::CreateBrowserProcessApp();
      break;
    case shared::PROCESS_TYPE_RENDERER:
      app = shared::CreateRendererProcessApp();
      break;
    case shared::PROCESS_TYPE_OTHER:
      app = shared::CreateOtherProcessApp();
      break;
  }

  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  int exit_code = CefExecuteProcess(main_args, app, NULL);
  if (exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Install xlib error handlers so that the application won't be terminated
  // on non-fatal errors.
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);

  // Create the singleton manager instance.
  shared::ClientManager manager;

  // Specify CEF global settings here.
  CefSettings settings;

  // Initialize CEF for the browser process. The first browser instance will be
  // created in CefBrowserProcessHandler::OnContextInitialized() after CEF has
  // been initialized.
  CefInitialize(main_args, settings, app, NULL);

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  CefRunMessageLoop();

  // Shut down CEF.
  CefShutdown();

  return 0;
}
