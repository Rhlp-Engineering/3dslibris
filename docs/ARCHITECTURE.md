# Architecture

High-level architecture of 3dslibris as of v2.0.3. This document describes the current structure, module responsibilities, and known design trade-offs.

## Module map

```
source/
├── main.cpp                    # Entry point: 3DS bootstrap, service init, App lifecycle
├── app/                        # Application state machine + orchestration
│   ├── app.cpp                 # Main loop, mode routing, job queue, warmup
│   └── status_layout_utils.cpp # Status bar layout
├── library/                    # Library/browser screen
│   ├── app_browser.cpp         # Book grid, cover cache, metadata jobs
│   └── browser_*.h             # Cover cache, job queue, warmup utilities
├── reader/                     # Book reader screen
│   ├── app_book.cpp            # Open/reopen flow, input routing, deferred relayout
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
│   │   ├── book_io.cpp         # MOBI dispatch + ODT parsing (TXT/RTF extracted)
│   │   ├── buffered_status_log.cpp
│   │   ├── epub_image_utils.cpp
│   │   ├── file_read_utils.cpp
│   │   ├── page_cache_utils.cpp
│   │   ├── text_helpers.cpp    # Text normalization (UTF repair, RTF decode, etc.)
│   │   ├── xml_parse_utils.cpp
│   │   ├── pdf_view_utils.cpp  # Shared MuPDF viewport/navigation
│   │   ├── fixed_layout_viewport_utils.h
│   │   └── rtf_control_word_utils.h
│   ├── txt/                    # Plain text loader
│   │   └── txt_loader.cpp     # ReadAndNormalize() with CP1252 repair
│   ├── rtf/                    # Rich Text Format loader
│   │   └── rtf_loader.cpp     # ReadAndDecode() with RTF→UTF-8
│   ├── epub/                   # EPUB2/EPUB3 parser
│   │   └── epub.cpp            # Full EPUB parsing + page cache
│   ├── fb2/                    # FictionBook 2 parser
│   │   └── fb2.cpp
│   ├── mobi/                   # MOBI/KF8 parser
│   │   ├── mobi.cpp            # Cover extraction
│   │   ├── mobi_cover_meta_cache.cpp
│   │   ├── mobi_heading_markers.cpp
│   │   ├── mobi_page_cache.cpp  # Persistent page cache serialization
│   │   ├── mobi_markup_tag.cpp
│   │   ├── mobi_position_map.cpp
│   │   ├── mobi_record_decode.cpp
│   │   ├── mobi_record_scan.cpp
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
│   ├── text.cpp                # FreeType text renderer, framebuffers
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
  └─ main loop (input → mode dispatch → draw → frame swap)
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
       ├─ MOBI  → book_io.cpp + mobi/*.cpp (PDB records → pages)
       ├─ TXT   → txt_loader.cpp (raw text → pages)
       ├─ RTF   → rtf_loader.cpp (RTF control words → pages)
       ├─ ODT   → book_io.cpp (minizip + expat → pages)
       ├─ PDF   → mupdf/*.cpp (MuPDF → display lists → bitmaps)
       ├─ CBZ   → cbz/*.cpp + mupdf/*.cpp (zip → images → bitmaps)
       └─ XPS   → mupdf/*.cpp (MuPDF → display lists → bitmaps)
```

Reflowable formats (EPUB, FB2, MOBI, TXT, RTF, ODT) produce `Page` objects with text buffers and layout tokens. Fixed-layout formats (PDF, CBZ, XPS) produce bitmaps via MuPDF.

## Known design trade-offs

### 1. App is a God Object
`App` handles state machine, input routing, job queue, cover warmup, screen drawing, settings, and menu management. This is a legacy from dslibris that has grown organically.

**Impact:** Every new screen or feature requires modifying App. Hard to test in isolation.

**Future direction:** Extract controllers (LibraryController, ReaderController, SettingsController) and reduce App to a state machine that delegates.

### 2. Book mixes model, parsing, and rendering
`Book` contains metadata (good), but also parsing methods (`Open()`, `Parse()`, `Index()`) and rendering methods (`DrawCurrentView()`, `DrawCurrentMuPdfView()`). It also holds an `App*` pointer creating a circular dependency.

**Impact:** Cannot reuse Book outside this project. Format-specific state (MuPdfState, CbzState) lives in the domain model.

**Future direction:** Separate Book (pure model) from BookParser (format-specific) and BookRenderer (format-specific).

### 3. book_io.cpp reduced (4844 lines, down from 5369)
TXT and RTF loaders, text normalization helpers, and MOBI page cache were extracted to separate modules. Remaining content: MOBI parsing dispatch and ODT parsing.

**Impact:** TXT and RTF changes no longer touch book_io.cpp. MOBI and ODT still require navigating a large file.

**Future direction:** Extract ODT to its own module (requires decoupling from `FinalizePlainPage`). MOBI parsing (~3000 lines) is the next extraction target.

### 4. Formats extracted from book_io.cpp
The following modules were extracted to improve testability and reduce monolith size:

| Module | From | Public API |
|--------|------|------------|
| `txt_loader` | book_io.cpp | `ReadAndNormalize()` — reads TXT, repairs CP1252 mojibake, normalizes newlines |
| `rtf_loader` | book_io.cpp | `ReadAndDecode()` — reads RTF, decodes to UTF-8 via `text_helpers` |
| `text_helpers` | book_io.cpp | `NormalizeNewlines`, `NormalizeTextUtf8`, `DecodeRtfToUtf8`, `LooksLikeValidUtf8Bytes` |
| `mobi_page_cache` | book_io.cpp | `TryLoad()`, `Save()` — persistent page cache for MOBI |
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
