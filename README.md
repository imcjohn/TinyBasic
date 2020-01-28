# Tiny Basic Interpreter, in C++
### Interpreter for the Tiny Basic language, written by myself as a fun exercise in C++
### Should compile with gcc or msvc without any difficulties, I use CMake to compile it, but it is a very simple makefile

### Descriptions by file
* Expression.cpp/h - Handles evaluation of expression (numerical variables). Fun exercise in recursion
* helpers.cpp/h - Some quick helper functions to handle math and capitalization
* main.cpp - The primary basic interpreter/runner. Soon i'm going to abstract it out to a few files for clarity, but haven't had the time to do it yet
* CMakeLists - Makefile for the project

### Things I haven't added yet but plan on adding soon, so check back if curious!
* Ability to load/run from files
* Some example programs
* Comprehensive unit tests