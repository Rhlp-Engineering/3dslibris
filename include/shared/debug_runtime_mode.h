#pragma once

namespace debug_runtime {

inline bool BackgroundWorkersDisabled() {
#if defined(DEBUG_DISABLE_BACKGROUND_WORKERS) && DEBUG_DISABLE_BACKGROUND_WORKERS
  return true;
#else
  return false;
#endif
}

inline bool BrowserWarmupDisabled() { return BackgroundWorkersDisabled(); }

inline bool ForceSynchronousBookOpen() { return BackgroundWorkersDisabled(); }

inline bool ForceSynchronousCbzDecode() { return BackgroundWorkersDisabled(); }

inline bool ForceSynchronousMuPdfRender() {
  return BackgroundWorkersDisabled();
}

inline bool ForceSynchronousMobiFinalize() {
  return BackgroundWorkersDisabled();
}

} // namespace debug_runtime
