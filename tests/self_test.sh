#!/bin/bash
PASS=0
FAIL=0

test() {
  FILE=$1
  EXPECT=$2
  OUT=$(./shrijilang $FILE | tail -n 1)

  if echo "$OUT" | grep -q "$EXPECT"; then
    echo "✅ $FILE"
    PASS=$((PASS+1))
  else
    echo "❌ $FILE"
    echo "Expected: $EXPECT"
    echo "Got: $OUT"
    FAIL=$((FAIL+1))
  fi
}

test tests/t1_assign.sri 10
test tests/t4_if.sri 10
test tests/t6_nested_if.sri 5
test tests/t7_mod.sri 1
test tests/t8_compare.sri 9
test tests/t9_complex.sri 7

echo "===================="
echo "PASS=$PASS FAIL=$FAIL"
