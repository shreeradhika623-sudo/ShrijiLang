#!/bin/bash

echo "// SHRIJILANG ULTRA STRESS TEST (AUTO-GENERATED)"
echo "// ~250 lines | Huge numbers | Deep expressions"
echo

for i in $(seq 1 250); do
  echo "mavi x$i = ("
  echo "  (99999999 * $i + 88888888) / (7777777 + $i)"
  echo "  + (66666666 - 55555 * $i)"
  echo "  - (444444 / (3 + ($i % 5 + 1)))"
  echo ")"
  echo
done
