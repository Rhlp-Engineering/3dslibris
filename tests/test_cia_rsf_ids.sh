set -eu

ROOT="$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)"
RELEASE_RSF="$ROOT/assets/cia/build-cia.rsf"
DEBUG_RSF="$ROOT/assets/cia/build-cia-debug.rsf"

test -f "$RELEASE_RSF"
test -f "$DEBUG_RSF"

release_unique_id="$(awk -F: '/UniqueId/ {gsub(/[[:space:]]/, "", $2); print $2}' "$RELEASE_RSF")"
debug_unique_id="$(awk -F: '/UniqueId/ {gsub(/[[:space:]]/, "", $2); print $2}' "$DEBUG_RSF")"
release_product_code="$(awk -F: '/ProductCode/ {gsub(/[[:space:]\"]/, "", $2); print $2}' "$RELEASE_RSF")"
debug_product_code="$(awk -F: '/ProductCode/ {gsub(/[[:space:]\"]/, "", $2); print $2}' "$DEBUG_RSF")"

[ -n "$release_unique_id" ]
[ -n "$debug_unique_id" ]
[ -n "$release_product_code" ]
[ -n "$debug_product_code" ]
[ "$release_unique_id" != "$debug_unique_id" ]
[ "$release_product_code" != "$debug_product_code" ]
