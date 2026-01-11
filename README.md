ShrijiLang

ShrijiLang was created by a single independent developer.
There was no company, no funding, and no ready-made framework behind this project.
Only long hours of learning, building, breaking, and rebuilding until a working language engine was formed.

This project exists because computing should also reflect Indian thinking — clarity, balance, logic, and responsibility — not only speed and abstraction.
ShrijiLang is not built to impress. It is built to be correct, stable, and meaningful.

The goal is simple: create a language that can grow into systems that think, decide, and serve real human needs.


---

What is ShrijiLang?

ShrijiLang is an experimental Indian programming language written in C.

It includes:

its own lexer, parser, and interpreter

a script-based .sri programming format

control flow (agar, jabtak, blocks)

arithmetic and comparison expressions

a test-driven runtime



---

Project Structure

ShrijiLang/ ├── src/ ├── include/ ├── sakhi/ ├── niyu/ ├── mira/ ├── kavya/ ├── tone/ ├── tests/ ├── Makefile └── shrijilang


---

How to Build

git clone https://github.com/shreeradhika623-sudo/ShrijiLang.git
cd ShrijiLang
make clean
make

This will compile the full ShrijiLang engine and create the executable shrijilang.


---

How to Run

./shrijilang tests/t1_assign.sri

You can also run any .sri file:

./shrijilang your_script.sri


---

Language Basics

Variables

mavi x = 5
mavi y = 10

Printing

likho x
likho y

Arithmetic

likho x + y
likho x * 2

If condition

agar (x > 3) {
    likho x
}

While loop

mavi i = 1
jabtak (i <= 5) {
    likho i
    i = i + 1
}


---

Example Program

Create a file called hello.sri:

mavi x = 5
mavi y = 10
likho x + y

Run it:

./shrijilang hello.sri

Output:

15


---

Test Suite

./tests/self_test.sh


---
License

Licensed under GNU AGPLv3.

Author

Created and maintained by a single independent Indian developer.


