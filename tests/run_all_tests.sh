#!/bin/bash
# Run all host tests and report results
set -u

TEST_ROOT="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
passed=0
failed=0
total=0
failures=""

for test_script in "$TEST_ROOT"/test_*.sh; do
  # Skip run_all_tests.sh itself
  basename="$(basename "$test_script")"
  if [ "$basename" = "run_all_tests.sh" ]; then
    continue
  fi
  
  total=$((total + 1))
  name="$(basename "$test_script" .sh)"
  
  if (cd "$TEST_ROOT/.." && bash "$test_script") 2>/tmp/test_err_$$.txt; then
    printf "  PASS  %s\n" "$name"
    passed=$((passed + 1))
  else
    printf "  FAIL  %s\n" "$name"
    failed=$((failed + 1))
    failures="$failures\n  - $name"
    cat /tmp/test_err_$$.txt 2>/dev/null | head -5
  fi
  rm -f /tmp/test_err_$$.txt
done

echo ""
echo "Results: $passed/$total passed, $failed failed"
if [ $failed -gt 0 ]; then
  echo "Failed tests:$failures"
  exit 1
fi
