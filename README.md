

ShrijiLang

ShrijiLang is an experimental scripting language written in C with its own lexer, parser, AST, and interpreter.

This project focuses on building a small, correct, and understandable language engine from scratch.
It is intended as a learning-driven and research-driven compiler project.


---

Features

ShrijiLang currently supports:

Variables

Arithmetic and comparison expressions

If conditions (agar)

While loops (jabtak)

Block-based execution

Script files (.sri)

A test-driven runtime


All components are implemented in native C:

Lexer

Parser

Abstract Syntax Tree

Interpreter



---

Example

Create a file hello.sri:

mavi x = 10
mavi y = 5
likho x + y

Run it:

./shrijilang hello.sri

Output:

15


---

Project Structure

ShrijiLang/
├── src/        # Core compiler and runtime
├── include/    # Headers
├── sakhi/      # AI interface layer (experimental)
├── niyu/
├── mira/
├── kavya/
├── tone/
├── tests/      # Test scripts and validation
├── Makefile
└── shrijilang  # Compiler executable


---

Build

git clone https://github.com/shreeradhika623-sudo/ShrijiLang.git
cd ShrijiLang
make clean
make

This builds the shrijilang executable.


---

Run

Run a test script:

./shrijilang tests/t1_assign.sri

Run any .sri file:

./shrijilang your_script.sri


---

Test Suite

./tests/self_test.sh


---

Project Status

ShrijiLang is in early development.

Planned next milestones:

Type system

Functions

Modules

Error recovery

Standard library



---

About

ShrijiLang is developed by a single independent developer as a long-term compiler and language design project.

The goal is to build a language that grows from a simple interpreter into a full system capable of structured reasoning and real-world computation.


---

License

GNU AGPLv3


--
