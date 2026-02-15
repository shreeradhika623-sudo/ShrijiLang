#!/bin/bash
set -e

BIN="./shrijilang"

pass=0
fail=0
total_start=$(date +%s%N)

run_test () {
  local name="$1"
  local script="$2"
  local expect="$3"

  local start_ns end_ns dur_ns dur_ms
  start_ns=$(date +%s%N)

  out=$($BIN "$script" 2>&1 || true)

  end_ns=$(date +%s%N)
  dur_ns=$((end_ns - start_ns))
  dur_ms=$((dur_ns / 1000000))

  if echo "$out" | grep -q "$expect"; then
    echo "✅ PASS: $name   (${dur_ms} ms)"
    pass=$((pass+1))
  else
    echo "❌ FAIL: $name   (${dur_ms} ms)"
    echo "---- expected to find ----"
    echo "$expect"
    echo "---- got ----"
    echo "$out"
    fail=$((fail+1))
  fi
}

echo "======================================="
echo "ShrijiLang Self Test"
echo "======================================="

make clean >/dev/null
make >/dev/null

run_test "basic test" "test.sri" "Universe-Class V1"
run_test "full test"  "test_full.sri" "Calculate karne par 5 aaya"
run_test "gold test"  "gold_test.sri" "Calculate karne par 1 aaya"

total_end=$(date +%s%N)
total_ns=$((total_end - total_start))
total_ms=$((total_ns / 1000000))

echo "======================================="
echo "PASS=$pass FAIL=$fail"
echo "TOTAL TIME: ${total_ms} ms"
echo "======================================="

if [ $fail -ne 0 ]; then
  exit 1
fi
