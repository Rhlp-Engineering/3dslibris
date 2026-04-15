set -eu

ROOT="$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)"
OUTDIR="${TMPDIR:-/tmp}/3dslibris-tests"
WORKDIR="$OUTDIR/cbz-archive"
mkdir -p "$OUTDIR" "$WORKDIR"

ARCHIVE_PATH="$WORKDIR/sample.cbz"

python3 - "$ARCHIVE_PATH" <<'PY'
import sys
import zipfile

archive_path = sys.argv[1]
entries = [
    ("./002-page.PNG", b"png-page"),
    ("001-cover.jpg", b"jpg-cover"),
    ("folder/../010-last.JPEG", b"jpeg-last"),
    ("nested\\\\003-middle.jpg", b"jpg-middle"),
    ("notes.txt", b"skip-me"),
    ("__MACOSX/", b""),
]

with zipfile.ZipFile(archive_path, "w", compression=zipfile.ZIP_STORED) as zf:
    for name, data in entries:
        zf.writestr(name, data)
PY

. "$ROOT/tests/test_build.sh"

MINIZIP_INCLUDE=""
for candidate in \
  /opt/homebrew/include \
  /usr/local/include \
  /usr/include \
  "${DEVKITPRO:-}/portlibs/3ds/include" \
  "${DEVKITPRO:-}/portlibs/switch/include"
do
  if [ -n "${candidate:-}" ] && [ -f "$candidate/minizip/unzip.h" ]; then
    MINIZIP_INCLUDE="-I$candidate"
    break
  fi
done

if [ -z "$MINIZIP_INCLUDE" ]; then
  echo "SKIP test_cbz_archive: minizip headers not available in host environment"
  exit 0
fi

build_test test_cbz_archive \
  "$ROOT/tests/test_cbz_archive.cpp" \
  "$ROOT/source/formats/cbz/cbz_archive.cpp" \
  "$MINIZIP_INCLUDE" \
  -lminizip
