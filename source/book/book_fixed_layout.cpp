#include "book/book.h"

#include "book/page.h"
#include "formats/mupdf/mupdf_worker.h"
#include "formats/cbz/cbz_worker.h"
#include "shared/debug_log.h"

void Book::DrawCurrentView(Text *ts) {
  if (!ts)
    return;
  if (IsPdf()) {
    DrawCurrentMuPdfView(ts);
    return;
  }
  if (IsCbz()) {
    DrawCurrentCbzView(ts);
    return;
  }
  if (GetPageCount() == 0)
    return;
  GetPage()->Draw(ts);
}

void Book::SetFixedLayoutViewportInteraction(bool active) {
  if (IsPdf()) {
    SetMuPdfViewportInteraction(active);
    return;
  }
  if (IsCbz())
    SetCbzViewportInteraction(active);
}

void Book::ResetFixedLayoutViewportForNavigation() {
  if (IsPdf()) {
    ResetMuPdfViewport();
    return;
  }
  if (IsCbz())
    ResetCbzViewport();
}

bool Book::ChangeFixedLayoutZoom(int delta) {
  if (IsPdf())
    return ChangeMuPdfZoom(delta);
  if (IsCbz())
    return ChangeCbzZoom(delta);
  return false;
}

bool Book::MoveFixedLayoutViewportToPreview(int touch_x, int touch_y) {
  if (IsPdf())
    return MoveMuPdfViewportToPreview(touch_x, touch_y);
  if (IsCbz())
    return MoveCbzViewportToPreview(touch_x, touch_y);
  return false;
}

bool Book::TranslateFixedLayoutViewport(float dx, float dy) {
  if (IsPdf())
    return TranslateMuPdfViewport(dx, dy);
  if (IsCbz())
    return TranslateCbzViewport(dx, dy);
  return false;
}

bool Book::JumpFixedLayoutChapter(int delta) {
  if (IsPdf())
    return JumpMuPdfChapter(delta);
  if (IsCbz())
    return JumpCbzChapter(delta);
  return false;
}

bool Book::HasPendingFixedLayoutDeferredWork() const {
  if (IsPdf())
    return HasPendingMuPdfDeferredWork();
  if (IsCbz())
    return HasPendingCbzDeferredWork();
  return false;
}

u32 Book::GetFixedLayoutDeferredDelayMs() const {
  if (IsPdf())
    return GetMuPdfDeferredDelayMs();
  if (IsCbz())
    return GetCbzDeferredDelayMs();
  return 0;
}

bool Book::PumpDeferredFixedLayoutWork(u32 budget_ms) {
  if (IsPdf())
    return PumpDeferredMuPdfWork(budget_ms);
  if (IsCbz())
    return PumpDeferredCbzWork(budget_ms);
  return false;
}

void Book::CancelFixedLayoutDeferredWork() {
  if (IsPdf()) {
    CancelMuPdfIncrementalRender();
    return;
  }
  if (IsCbz())
    CancelCbzDeferredWork();
}

void Book::SuspendFixedLayoutWorkers() {
  // Stop background worker threads before the HOME menu takes over.
  // Workers are New3DS-only threadCreate threads on core 1. Leaving them alive
  // across an APT suspend can leave the system in a bad state and cause the
  // HOME menu to panic (svcBreak). ResumeFixedLayoutWorkers() restarts them.
  //
  // For MuPDF: ShutdownMuPdfWorker joins the thread and NULLs worker, but
  // leaves the fz_context/fz_document intact. We also reset worker_init_attempted
  // so that ResumeFixedLayoutWorkers() can call InitMuPdfWorker() again.
  //
  // For CBZ: ResetCbzTransientViewState(false) shuts the worker, frees cached
  // bitmaps, and leaves cbz_state and the archive entries intact.
  if (IsPdf() && mupdf_state && mupdf_state->worker) {
    DBG_LOGF(GetStatusReporter(), "[APT][SUSPEND] MuPDF worker shutdown begin book=%s",
             GetFileName() ? GetFileName() : "");
    ShutdownMuPdfWorker(mupdf_state);
    mupdf_state->worker_init_attempted = false;
    DBG_LOGF(GetStatusReporter(), "[APT][SUSPEND] MuPDF worker shutdown done book=%s",
             GetFileName() ? GetFileName() : "");
  }
  if (IsCbz()) {
    DBG_LOGF(GetStatusReporter(), "[APT][SUSPEND] CBZ worker shutdown begin book=%s",
             GetFileName() ? GetFileName() : "");
    ResetCbzTransientViewState(false);
    DBG_LOGF(GetStatusReporter(), "[APT][SUSPEND] CBZ worker shutdown done book=%s",
             GetFileName() ? GetFileName() : "");
  }
}

void Book::ResumeFixedLayoutWorkers() {
  // Restart background worker threads after HOME menu returns control.
  // Called from ReaderController::OnAppletResumed() on the main thread.
  if (IsPdf() && mupdf_state) {
    DBG_LOGF(GetStatusReporter(), "[APT][RESUME] MuPDF worker init begin book=%s",
             GetFileName() ? GetFileName() : "");
    InitMuPdfWorker(mupdf_state);
    DBG_LOGF(GetStatusReporter(), "[APT][RESUME] MuPDF worker init done worker=%s book=%s",
             mupdf_state->worker ? "ok" : "null (n/a)", GetFileName() ? GetFileName() : "");
  }
  if (IsCbz()) {
    DBG_LOGF(GetStatusReporter(), "[APT][RESUME] CBZ worker restart book=%s",
             GetFileName() ? GetFileName() : "");
    ResetCbzTransientViewState(true);
    DBG_LOGF(GetStatusReporter(), "[APT][RESUME] CBZ worker restart done book=%s",
             GetFileName() ? GetFileName() : "");
  }
}
