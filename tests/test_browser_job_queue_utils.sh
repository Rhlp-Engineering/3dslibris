set -eu

ROOT="$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)"
OUTDIR="${TMPDIR:-/tmp}/3dslibris-tests"
mkdir -p "$OUTDIR"

c++ -std=c++11 \
  "$ROOT/tests/test_browser_job_queue_utils.cpp" \
  -I"$ROOT/include" \
  -o "$OUTDIR/test_browser_job_queue_utils"

"$OUTDIR/test_browser_job_queue_utils"
