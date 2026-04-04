# Architecture

High-level architecture of 3dslibris. This document describes the current structure, module responsibilities, and known design trade-offs.

## Module map

```
source/
├── main.cpp                    # Entry point: 3DS bootstrap, service init, App lifecycle
├── app/                        # Application orchestration + lifecycle controllers
│   ├── app.cpp                 # App wiring, shared state accessors, navigation helpers
│   ├── startup_controller.cpp  # Boot sequence and runtime asset checks
│   ├── main_loop_controller.cpp# Main loop dispatch (mode routing + frame present)
│   ├── status_controller.cpp   # Status bar rendering/update cadence
│   └── status_layout_utils.cpp # Status bar layout
├── library/                    # Library/browser screen
│   ├── app_browser.cpp         # Book grid, cover cache, metadata jobs
│   ├── library_controller.cpp  # Library scan + browser flow delegation target
│   └── browser_*.h             # Cover cache, job queue, warmup utilities
├── reader/                     # Book reader screen
│   ├── app_book.cpp            # Open/reopen flow, input routing, deferred relayout
│   ├── reader_controller.cpp   # Reader flow delegation target
│   └── deferred_relayout_utils.h
├── book/                       # Book domain model + page management
│   ├── book.cpp                # Metadata, chapters, bookmarks, page vector
│   ├── book_fixed_layout.cpp   # PDF/CBZ viewport interaction
│   ├── book_inline_image.cpp   # Inline image layout for reflowable formats
│   ├── heading_layout.cpp      # Heading/chapter title layout
│   ├── inline_image_layout.cpp # Image placement algorithm
│   ├── layout_reflow.cpp       # Layout recalculation on settings change
│   ├── page.cpp                # Page buffer (text + layout tokens)
│   ├── reflow_worker.cpp       # Background reflow for New 3DS
│   ├── page_buffer_utils.h     # Page buffer serialization
│   └── reflow_cache_save_utils.h
├── formats/                    # Format-specific parsers
│   ├── common/                 # Shared format utilities
│   │   ├── book_io.cpp         # Thin format dispatcher
│   │   ├── buffered_status_log.cpp
│   │   ├── epub_image_utils.cpp
│   │   ├── file_read_utils.cpp
│   │   ├── page_cache_utils.cpp
│   │   ├── plain_parser.cpp   # TXT/RTF parse flow + plain heading heuristics/callbacks
│   │   ├── plain_text_stream.cpp # Incremental plain text pagination state machine
│   │   ├── text_helpers.cpp    # Text normalization (UTF repair, RTF decode, etc.)
│   │   ├── xml_book_parser.cpp # XML/Fb2 parse route extracted from Book::Parse
│   │   ├── xml_parse_utils.cpp
│   │   ├── pdf_view_utils.cpp  # Shared MuPDF viewport/navigation
│   │   ├── fixed_layout_viewport_utils.h
│   │   └── rtf_control_word_utils.h
│   ├── txt/                    # Plain text loader
│   │   └── txt_loader.cpp     # ReadAndNormalize() with CP1252 repair
│   ├── rtf/                    # Rich Text Format loader
│   │   └── rtf_loader.cpp     # ReadAndDecode() with RTF→UTF-8
 │   ├── epub/                   # EPUB2/EPUB3 parser
 │   │   ├── epub.cpp            # EPUB parse orchestration (384 lines, down from 1328)
 │   │   ├── epub_toc.cpp        # TOC parsing (NAV/NCX fallback, structured extraction)
 │   │   ├── epub_cache.cpp      # Page/resource cache management
 │   │   ├── epub_manifest.cpp   # Manifest parsing + item resolution
 │   │   ├── epub_cover.cpp      # Cover extraction
 │   │   ├── epub_ncx_parser.cpp # NCX TOC parser
 │   │   ├── epub_zip_utils.cpp  # ZIP access helpers
 │   │   ├── epub_page_cache.cpp # Persistent page cache
 │   │   └── epub_*_utils.cpp    # TOC title match, package loader, package TOC, diagnostics
│   ├── fb2/                    # FictionBook 2 parser
│   │   └── fb2.cpp
│   ├── mobi/                   # MOBI/KF8 parser
│   │   ├── mobi.cpp            # Cover extraction
│   │   ├── mobi_cover_meta_cache.cpp
│   │   ├── mobi_heading_markers.cpp
│   │   ├── mobi_page_cache.cpp  # Persistent page cache serialization
│   │   ├── mobi_parser_core.cpp # Source load/header parse/text record merge
│   │   ├── mobi_deferred_runtime.cpp # Deferred parse state machine runtime
│   │   ├── mobi_markup_extract.cpp # MOBI markup -> text extraction pipeline
│   │   ├── mobi_book_hooks.cpp  # Book-facing MOBI hook/callback wiring
│   │   ├── mobi_parser.cpp      # MOBI parse orchestration (open + deferred finalize)
│   │   ├── mobi_markup_tag.cpp
│   │   ├── mobi_position_map.cpp
│   │   ├── mobi_record_decode.cpp
│   │   ├── mobi_record_scan.cpp
│   │   ├── mobi_structured_toc_parser.cpp # INDX/TAGX/CNCX structured TOC parser
│   │   ├── mobi_toc_finalize.cpp # Structured/heuristic TOC finalization
│   │   ├── mobi_toc_apply.cpp    # Structured TOC mapping/apply (html_pos -> page)
│   │   ├── mobi_toc_prepare.cpp  # Structured TOC prepare/deferred-load wrapper
│   │   ├── mobi_toc_resolver.cpp # TOC resolver + inline filepos/deferred fallback
│   │   ├── mobi_text_decode.cpp  # Encoding decode + embedded title extraction
│   │   ├── mobi_text_cleanup.cpp
│   │   └── mobi_*.h            # Cover utils, deferred finalize
│   ├── pdf/                    # PDF entry point
│   │   └── pdf.cpp
│   ├── cbz/                    # CBZ entry point + rendering
│   │   ├── cbz.cpp
│   │   ├── cbz_archive.cpp
│   │   ├── cbz_decode.cpp
│   │   ├── cbz_document.cpp
│   │   ├── cbz_view.cpp
│   │   └── cbz_worker.cpp
│   └── mupdf/                  # MuPDF integration (PDF/CBZ/XPS)
│       ├── mupdf_common.cpp/h
│       ├── mupdf_document.cpp
│       ├── mupdf_render.cpp/h
│       ├── mupdf_view.cpp/h
│       ├── mupdf_worker.cpp/h
│       └── mupdf_state.h
 ├── ui/                         # UI primitives
 │   ├── text.cpp                # FreeType text renderer (289 lines, down from 1349)
 │   ├── text_renderer.cpp/h     # Extracted FreeType rendering engine
 │   ├── font_manager.cpp/h      # Font config, paths, metrics
 │   ├── button.cpp              # Button rendering + hit-testing
│   ├── ui_button_skin.cpp      # Procedural button skins + icon loading
│   ├── touch_utils.cpp         # Touch gesture interpretation
│   ├── browser_nav.cpp         # Browser navigation helpers
│   ├── framebuffer_blit_utils.h
│   ├── glyph_cache_lru.h
│   └── text_buffer_utils.h
├── menus/                      # Overlay menus
│   ├── menu.cpp
│   ├── bookmark_menu.cpp
│   ├── chapter_menu.cpp
│   └── paged_list_menu.cpp
├── settings/                   # Settings/prefs management
│   ├── prefs.cpp               # Persistent preferences (XML)
│   ├── font.cpp                # Font loading + metrics
│   └── app_prefs.cpp           # Settings screen logic
├── shared/                     # Cross-cutting utilities (genuinely shared)
│   ├── app_flow_utils.cpp/h    # Format detection, path conversion
│   ├── text_layout_utils.cpp/h # Text layout helpers
│   ├── text_unicode_utils.cpp/h# Unicode/UTF-8 utilities
│   └── utf8_utils.cpp/h        # UTF-8 encoding/decoding
└── core/
    └── parse.cpp               # Parser dispatch entry point

include/                        # Public headers (mirrors source/ structure)
├── book/book_context.h        # BookContext struct (replaces App* back-pointer)
├── shared/status_reporter.h   # IStatusReporter interface
├── ui/text_renderer.h         # TextRenderer class
├── ui/font_manager.h          # FontManager class
└── string_utils.h             # Inline string utilities (StartsWithNoCase, etc.)

third_party/                    # External dependencies
├── expat/                      # XML parser (moved from source/expat)
├── stb/                        # stb_image header
├── utf8proc/                   # UTF-8 normalization
├── libunibreak/                # Line breaking
└── mupdf/                      # MuPDF (PDF/CBZ/XPS rendering)
```

## Data flow

```
App::Run()
  ├─ StartupController::RunBootSequence()
  └─ MainLoopController::RunMainLoop() (input → mode dispatch → draw → frame swap)
       │
       ├─ Browser mode
       │    ├─ FindBooks() → scan bookdir → create Book objects
       │    ├─ Job queue → extract covers, index metadata, resolve TOC
       │    └─ Cover cache → load/evict RGB565 thumbnails
       │
       ├─ Opening mode
       │    └─ OpenBook() → parse → paginate → switch to Book mode
       │
       ├─ Book mode
       │    ├─ Page navigation (A/B/L/R, touch, D-pad)
       │    ├─ Deferred relayout (MOBI async reflow on New 3DS)
       │    └─ Viewport interaction (PDF/CBZ zoom + pan)
       │
       └─ Settings mode
            └─ Font size, spacing, orientation, color mode
```

## Format pipeline

```
Book::Open()
  └─ parse.cpp dispatches by extension
       │
       ├─ EPUB  → epub.cpp (minizip + expat XML → pages)
       ├─ FB2   → fb2.cpp (expat XML → pages)
       ├─ MOBI  → mobi_parser.cpp + mobi/*.cpp (PDB records → pages)
       ├─ TXT   → txt_loader.cpp (raw text → pages)
       ├─ RTF   → rtf_loader.cpp (RTF control words → pages)
       ├─ ODT   → book_io.cpp (minizip + expat → pages)
       ├─ PDF   → mupdf/*.cpp (MuPDF → display lists → bitmaps)
       ├─ CBZ   → cbz/*.cpp + mupdf/*.cpp (zip → images → bitmaps)
       └─ XPS   → mupdf/*.cpp (MuPDF → display lists → bitmaps)
```

Reflowable formats (EPUB, FB2, MOBI, TXT, RTF, ODT) produce `Page` objects with text buffers and layout tokens. Fixed-layout formats (PDF, CBZ, XPS) produce bitmaps via MuPDF.

## Known design trade-offs

### 1. App monolith reduction in progress
`App` no longer owns the full runtime loop implementation directly. `StartupController`, `MainLoopController`, `LibraryController`, `ReaderController`, `SettingsController`, and `StatusController` now host major flows, while `App` delegates via explicit methods.

**Impact:** Coupling was reduced (notably by removing controller `friend` access and introducing explicit `App` APIs), but `App` is still a broad facade and remains the central mutable state hub.

**Current direction:** Continue migrating mode-specific mutable state (`job_queue`, browser runtime details, reader deferred/opening state) behind controller-owned interfaces so `App` can converge toward orchestration-only responsibilities.

### 2. Book mixes model, parsing, and rendering
`Book` contains metadata (good), but also parsing methods (`Open()`, `Parse()`, `Index()`) and rendering methods (`DrawCurrentView()`, `DrawCurrentMuPdfView()`). It also holds an `App*` pointer creating a circular dependency.

**Impact:** Cannot reuse Book outside this project. Format-specific state (MuPdfState, CbzState) lives in the domain model.

**Future direction:** Separate Book (pure model) from BookParser (format-specific) and BookRenderer (format-specific).

### 3. book_io.cpp reduced (66 lines, down from 5369)
TXT/RTF parse flow, plain heading heuristics/callbacks, text normalization helpers, MOBI page cache, MOBI parser core helpers (source/header/merge), MOBI deferred runtime state machine, MOBI markup extraction pipeline, MOBI text decode/title extraction, MOBI structured TOC INDX/TAGX/CNCX parsing, MOBI TOC finalization, MOBI TOC prepare/deferred-load wrappers, MOBI TOC resolver (inline/deferred), MOBI parse orchestration, MOBI hook/callback wiring, and XML parse flow were extracted to separate modules. Remaining content: format dispatch only.

**Impact:** Format/parser changes no longer touch `book_io.cpp` in practice, except when adding/removing dispatch routes.

**Future direction:** Keep `book_io.cpp` as a thin dispatch unit. Optional next step: a small dispatch table helper to simplify extension registration.

### 4. Formats extracted from book_io.cpp
The following modules were extracted to improve testability and reduce monolith size:

| Module | From | Public API |
|--------|------|------------|
| `txt_loader` | book_io.cpp | `ReadAndNormalize()` — reads TXT, repairs CP1252 mojibake, normalizes newlines |
| `rtf_loader` | book_io.cpp | `ReadAndDecode()` — reads RTF, decodes to UTF-8 via `text_helpers` |
| `text_helpers` | book_io.cpp | `NormalizeNewlines`, `NormalizeTextUtf8`, `DecodeRtfToUtf8`, `LooksLikeValidUtf8Bytes` |
| `plain_text_stream` | book_io.cpp | `InitState()`, `ContinueState()` — incremental pagination state machine |
| `plain_parser` | book_io.cpp | `ParseBuffer()`, `ParseTxtFile()`, `ParseRtfFile()`, heading heuristics/callback wiring |
| `xml_book_parser` | book_io.cpp | `ParseXmlBookFile()` — XML/Fb2 parse path and perf/error reporting |
| `mobi_book_hooks` | book_io.cpp | `Make()` — MOBI callbacks/hook wiring for parser runtime |
| `mobi_page_cache` | book_io.cpp | `TryLoad()`, `Save()` — persistent page cache for MOBI |
| `mobi_parser_core` | book_io.cpp | `LoadMobiSource()`, `ParseMobiHeader()`, `BuildMobiMergedText()` |
| `mobi_deferred_runtime` | book_io.cpp | `Continue()`, `Finalize()`, deferred state map lifecycle |
| `mobi_markup_extract` | book_io.cpp | `ExtractToText()` — markup parsing, heading hints, inline image contexts |
| `mobi_text_decode` | book_io.cpp | `DecodeBytesToUtf8()`, `ApplyEmbeddedTitle()` |
| `mobi_parser` | book_io.cpp | `ParseFile()`, `ContinueDeferredParse()` — MOBI open/deferred orchestration |
| `mobi_structured_toc_parser` | book_io.cpp | `ParseStructuredToc()` — INDX/TAGX/CNCX parser with callback-based decoding/filtering |
| `mobi_toc_finalize` | book_io.cpp | `BuildChaptersFromHints()`, `FinalizePreparedToc()` — TOC finalization + confidence |
| `mobi_toc_apply` | book_io.cpp | `HtmlPosToPage()`, `BuildChaptersFromStructuredToc()` |
| `mobi_toc_prepare` | book_io.cpp | `Prepare()`, `LoadDeferred()` — structured TOC prepare/deferred load wrappers |
| `mobi_toc_resolver` | book_io.cpp | `ParseInlineFileposToc()`, `PrepareStructuredToc()`, `LoadDeferredStructuredToc()` |
| `StartsWithNoCase` | epub_image_utils.cpp | Generic string utility → `string_utils.h` |

All extracted modules have corresponding test suites using the project's `test_build.sh` helper.

## Build system

- **Target:** `3dslibris.3dsx` (homebrew) and `3dslibris.cia` (installable)
- **Toolchain:** devkitARM (ARM11 + ARM9 for New 3DS syscore)
- **Docker:** `docker/Dockerfile.cia` for reproducible builds
- **CI:** GitHub Actions (`.github/workflows/ci.yml`)
- **Tests:** Host-compiled C++ tests in `tests/` run via shell scripts

## Path constants

All SD card paths are centralized in `include/path_utils.h` under the `paths::` namespace. If the directory layout changes, update constants there only.

## Architectural review

Critical design issues identified during architectural audit, ordered by severity.

### ~~Critical: App ↔ Book circular dependency~~ (resolved)

`Book` no longer holds an `App*` pointer. The dependency was broken via `BookContext` (`include/book/book_context.h`), a struct that injects only the interfaces Book needs: `Text*`, `Prefs*`, `IStatusReporter*`, and callbacks. The global `App *app` variable was removed from file scope — `App` is now heap-allocated locally in `main()`.

**Impact:** Book can be tested and reused independently. The circular dependency is broken.

**Remaining:** `App *app` is still a singleton passed around by pointer (not a file-scope global, but still widely accessible). 20 files include `app/app.h` directly.

### ~~Critical: Format layer coupled to App/UI~~ (resolved for MOBI)

`mobi.cpp` no longer includes `app/app.h`. MOBI parsers communicate via `IStatusReporter` interface and `BookContext`. EPUB parsers may still have residual coupling — needs verification.

### ~~Critical: book_io.cpp remains a monolith~~ (resolved)

`book_io.cpp` is now a thin format dispatcher. All parser logic lives in dedicated modules.

### ~~High: epub.cpp remains a large critical parser~~ (resolved)

epub.cpp was reduced from 1328 → 384 lines. Three major extractions completed:
- `epub_toc.cpp` (624 lines) — TOC parsing with NAV/NCX fallback
- `epub_cache.cpp` (65 lines) — page/resource cache management  
- `epub_manifest.cpp` (430 lines) — manifest parsing and item resolution

Additional files extracted: `epub_cover.cpp`, `epub_ncx_parser.cpp`, `epub_zip_utils.cpp`, `epub_page_cache.cpp`, and various TOC utility modules.

**Remaining:** epub.cpp itself still has zero dedicated tests.

### High: Zero tests on critical components

49 host-compiled unit tests now exist covering utilities (MOBI parsing, text layout, UTF-8, XML, path utils, page cache, glyph cache, etc.) with ~769 assertions. Test infrastructure includes `test_assert.h`, `run_all_tests.sh`, `make test-host`, and a CI job.

**Still untested:** `app.cpp` (686 lines), `app_book.cpp` (903 lines), `epub.cpp` (384 lines). No integration tests load real book files (.epub, .mobi, etc.) — all tests use synthetic in-memory data.

**Future direction:** Adopt Catch2/doctest for richer test DSL. Add integration tests with minimal real books. Centralize test macros (some .cpp files still define their own).

### ~~High: UI and business logic mixed~~ (partially resolved)

`TextRenderer` (FreeType pure) and `FontManager` (config, paths, metrics) have been extracted. `text.cpp` reduced from 1349 → 289 lines (-79%).

**Remaining:** `UIManager` does not exist — UI orchestration still lives in `App` (686 lines). `App` mixes orchestration with shared runtime state and compatibility wrappers.

**Future direction:** Extract `UIManager` from App to separate UI lifecycle from business orchestration.

### ~~Medium: Inconsistent third-party dependency management~~ (resolved)

`DEPENDENCIES.md` exists with version table for all vendored deps. Cleanup completed:
- `third_party/minizip/` removed — dead vendored source; 3DS build uses devkitPro portlib `-lminizip`
- `lib/libbz2.a` removed — duplicated devkitPro bzip2 portlib
- Remaining inconsistencies:
  - MuPDF is a full upstream tree with no update strategy
  - No git submodules — all deps are full copies with no provenance tracking

**Future direction:** Consider git submodules or commit-hash pinning in DEPENDENCIES.md.

### Medium: Global `App *app` singleton

`App *app` is heap-allocated in `main()` (not a file-scope global), but still functions as a de facto singleton. 20 files include `app/app.h` and access App state directly.

**Impact:** Implicit dependencies the compiler cannot verify. Impossible to reason about data flow.

**Future direction:** Eliminate singleton pattern. Pass App as explicit parameter or use context structs for frequently-needed data.

### Medium: No clear extension points for new formats

Adding a format requires modifying `book_io.cpp` (dispatch), `app_flow_utils.cpp` (detection), `format_t` enum in `app_flow_utils.h`, and `Makefile`. No `FormatParser` interface or registry exists.

**Future direction:** Define abstract `FormatParser` interface. Register parsers by extension in a map. `book_io.cpp` becomes a generic dispatcher.

### Fragile zones

| Zone | Why fragile |
|------|-------------|
| `plain_parser.cpp` | Plain text parse heuristics + callback wiring still complex and regression-prone without integration tests |
| `book.h` public API | Mixes metadata, rendering, parsing, fixed-layout, async reflow, MOBI deferred |
| `include/app/app.h` public section | Public fields (`ts`, `prefs`, `buttons`, `books`) still allow broad external mutation of App state |
| `epub.cpp` | 384 lines, zero dedicated tests, most-used format |
| `App *app` singleton | Heap-allocated in main() but still widely accessible — UB if accessed before init or after destruction |
| `app.cpp` / `app_book.cpp` | 686 + 903 lines, zero test coverage |

### Resolved items (for reference)

| Item | Resolution |
|------|-----------|
| `Book` → `App*` back-pointer | Replaced by `BookContext` DI struct with `IStatusReporter*` |
| `book_io.cpp` monolith | Reduced to thin dispatcher; 18+ modules extracted |
| `epub.cpp` 1328 lines | Reduced to 384 lines; 12+ modules extracted |
| `text.cpp` 1349 lines | Reduced to 289 lines; `TextRenderer` + `FontManager` extracted |
| `DEPENDENCIES.md` missing | Created with version table for all vendored deps |
| Test infrastructure | 49 host tests, ~769 assertions, `make test-host`, CI job |
