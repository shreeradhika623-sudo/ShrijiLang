
ShrijiLang

ShrijiLang was created by a single independent developer.

There was no company, no funding, and no ready-made framework behind this project. Only long hours of learning, building, breaking, and rebuilding until a working language engine was formed.

This project exists because computing should also reflect Indian thinking — clarity, balance, logic, and responsibility — not only speed and abstraction.

ShrijiLang is not built to impress. It is built to be correct, stable, and meaningful.

The goal is simple: create a language that can grow into systems that think, decide, and serve real human needs.


---

What is ShrijiLang?

ShrijiLang is an experimental Indian programming language written in C.

It includes:

its own lexer, parser, and interpreter

a script-based .sri programming format

control flow (if, while, blocks)

arithmetic and comparison expressions

a test-driven runtime



---

Project Structure

ShrijiLang/
├── src/
├── include/
├── sakhi/
├── niyu/
├── mira/
├── kavya/
├── tone/
├── tests/
├── Makefile
└── shrijilang


---

How to Build

make clean
make


---

How to Run

./shrijilang tests/t1_assign.sri


---

Example

mavi x = 5
mavi y = 10
likho x + y

./shrijilang hello.sri

Output:

15


---

Test Suite

./tests/self_test.sh


---

License

MIT License


---

Author

ShrijiLang is created and maintained by a single independent Indian developer.
