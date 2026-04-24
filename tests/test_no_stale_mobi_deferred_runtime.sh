#!/bin/sh
set -eu
cd "$(dirname "$0")/.."

for symbol in \
  CancelDeferredMobiParse \
  ContinueDeferredMobiParse \
  HasDeferredMobiParse \
  SetMobiDeferredReadyAtMs \
  GetMobiDeferredReadyAtMs \
  ForceSynchronousMobiFinalize \
  ForceSynchronousBookOpen \
  deferred_pumped \
  mobi_deferred_runtime
do
  if git grep -n "$symbol" -- include source tests ':!tests/test_no_stale_mobi_deferred_runtime.sh'; then
    echo "stale MOBI deferred runtime symbol remains: $symbol" >&2
    exit 1
  fi
done
