# Game of Life

Conway's Game of Life implemented in C using ncurses

## Overview

Basic TUI implementation of Conway's Game of Life using the ncurses library. 

Controls:
- `Q`: Quit
- `R`: Randomize grid
- `C`: Clear grid
- `<Space>`: Pause

## Building

This project uses [@tsoding](https://github.com/tsoding)'s `nob` build system

Using any C compiler, run:

```sh
clang -o nob nob.c
./nob
```
