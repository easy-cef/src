// george @ 2019/4/21

#include "app/easy_cef_renderer.h"

EasyCefRenderer::EasyCefRenderer() {
}

// CefRenderProcessHandler methods:
void EasyCefRenderer::OnWebKitInitialized() {
  CefMessageRouterConfig config;
  message_router_ = CefMessageRouterRendererSide::Create(config);
}

void EasyCefRenderer::OnContextCreated(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context) {
  message_router_->OnContextCreated(browser, frame, context);
}

void EasyCefRenderer::OnContextReleased(
  CefRefPtr<CefBrowser> browser,
  CefRefPtr<CefFrame> frame,
  CefRefPtr<CefV8Context> context) {
  message_router_->OnContextReleased(browser, frame, context);
}

bool EasyCefRenderer::OnProcessMessageReceived(
  CefRefPtr<CefBrowser> browser,
  CefProcessId source_process,
  CefRefPtr<CefProcessMessage> message) {
  return message_router_->OnProcessMessageReceived(browser, source_process, message);
}

namespace shared {

CefRefPtr<CefApp> CreateRendererProcessApp() {
  return new EasyCefRenderer();
}

}
