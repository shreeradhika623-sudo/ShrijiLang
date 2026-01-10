# ShrijiLang V1

ShrijiLang is an experimental Hindi-syntax programming language core
built in C.

It supports:
- Variables (mavi)
- Arithmetic expressions
- Conditions (agar / warna)
- Loops (jabtak)
- Nested blocks
- A simple runtime with state and AI hooks

This is V1 – the core engine.
Future versions will add functions, strings, modules and system bindings.

## Example

mavi x = 10
mavi y = 2

agar x > y {
    x = x + y
}

x

## Build

Requirements:
- gcc
- make

To build:

make

## Run

./shrijilang tests/t1_assign.sri
