# c-windows-school

A DOS-based windowing library and demo applications written in C, developed as a school project around 2002/2003.

> **Note:** This is a vintage school project written for **Turbo C 3** on **MS-DOS**, targeting the old DOS text-mode console. It will not compile or run on modern systems without a DOS emulator (e.g. DOSBox) and Turbo C 3.

---

## Author

**Tiago Freitas** — 12º I1, Colégio de Gaia, 2002/2003  
📧 mephist@programmer.net

---

## Project Overview

This project implements a **windowing framework for C in DOS text mode**, providing:

- Bordered, titled windows with open/close animations
- Window save/restore (background preservation)
- Message boxes with configurable buttons and types
- Text utilities (word counting, multi-line strings, centering)
- A menu bar system
- Two demo student/article management applications

---

## Repository Structure

| File | Description |
|------|-------------|
| `WINC.C` | Core definitions: macros, constants, animation and button defines |
| `WINC.H` | Header file — include this in your project to use the library |
| `WINDOWS.C` | Window engine: open, close, move, save, restore, message box |
| `TEXT.C` | Text utilities: word/line parsing, centering, menus, input |
| `TEXTO.C` | Earlier/alternate version of the text utilities |
| `HELP.C` / `HELP.txt` | Library usage documentation |
| `ALNM.C` | Demo app: Student management system with a full menu bar UI |
| `TRABANEW.C` | Demo app: Article/movement file management with windowed UI |
| `TTRABL.C` | Earlier version of the article/movement management app (no windowing) |
| `TRAB2.C` | Student records file management (binary file I/O) |
| `NOVO.C` | Article catalogue manager with union-based article types (shoes/boxes) |
| `SAMPLE.C` | Early prototype/sample using the windowing library |
| `TEMP.C` | Scratch file: prints extended ASCII characters |
| `TEMP2.C` | Scratch file: demonstrates `gettext`/`puttext` screen save/restore |
| `TECLAS.CPP` | Scratch file: prints key scan codes |
| `SNIFSNIF.C` | Scratch file: reads and prints raw video memory |
| `WINC.PRJ` | Turbo C project file |
| `WINC.DSK` | Turbo C desktop/workspace state file |

---

## The Windowing Library

### Setup

Include the header in your source file:

```c
#include "winc.h"
```

### Window Structure

```c
struct win mywindow = {
    x, y, x1, y1,          // position (top-left to bottom-right)
    cb, cf,                 // window background & foreground colour
    bb, bf,                 // border background & foreground colour
    titcolor,               // title colour
    "Window Title",         // title string
    SIMPLES,                // border style (SIMPLES or DUPLO)
    NULL                    // internal buffer — always initialise to NULL
};
```

### Border Styles

| Constant | Style |
|----------|-------|
| `SIMPLES` | Single-line border (`┌─┐│└┘`) |
| `DUPLO` | Double-line border (`╔═╗║╚╝`) |

### Window Animations

| Constant | Effect |
|----------|--------|
| `aNO` | No animation |
| `aHORIZ` | Horizontal expansion |
| `aVERT` | Vertical expansion |
| `aOBLIQ` | Diagonal expansion |

### Key Functions

```c
// Open a window (saves background, draws border, sets focus)
int openwin(struct win *wind, int anim);

// Close a window (restores background)
int closewin(struct win *wind);

// Set focus to a window (sets colour/coordinate context)
void selectwin(struct win wind);

// Move a window to a new position
int movewin(struct win *wind, int x, int y);

// Display a message box
int msgbox(char botoes, int tipo, char *mensagem, char anim);
```

### Message Box

```c
// Buttons (combine with |)
OK | CANCEL | ABORTAR | REPETIR

// Types
INFO        // Information
CRITIC      // Critical information
ERRO        // Error
CRITICERRO  // Critical error

// Example
msgbox(OK | CANCEL, ERRO, "Something went wrong!\nTry again?", aHORIZ);

// Return values
bOK, bCANCELADO, bABORTADO, bREPETIDO
```

---

## Text & Menu Utilities (`TEXT.C`)

```c
int  getpalavras(char *texto);                         // count words (supports 'multi word' groups)
char *getpalavra(int num, char *texto);                // get word by index
int  getpalavrapos(int num, char *linha);              // get character position of a word
int  getlines(char *texto);                            // count lines in a multi-line string
char *getline(int num, char *texto);                   // get a line by index
int  centra(char *s, int tam);                         // centre-print a string
void gotoop(int pos, char *s, int cb, int cf);         // highlight a menu item
int  callmenu(int *pos, char *string, int cb, int cf); // interactive menu bar
char leitura(char *v);                                 // read one char from an allowed set
```

**Multi-word groups** are enclosed in single quotes, e.g.:
```
'Insert Article'  List  Exit
```

---

## Demo Applications

### `ALNM.C` — Student System

A full-screen application featuring:

- Top menu bar with three menus: **Ficheiro** (File), **Bloquear** (Lock), **Sobre** (About)
- Dropdown submenus navigable with arrow keys or hotkeys (`F`, `B`, `S`)
- **Ficheiro** submenu: Insert, Modify, Consult, List, Save As, Open, Exit
- **Bloquear** submenu: Lock screen, Define PIN (with old/new/confirm PIN dialog)
- **Sobre** submenu: Help window, About/Info window
- Welcome splash window on startup

### `TRABANEW.C` — Article & Movement Manager

A windowed application for managing articles and sales movements, featuring:

- Binary file storage (`artigos.dat`, `movimentos.dat`)
- Insert, list, and query operations for articles and movements
- Full use of the windowing library including `msgbox` for confirmations and errors

### `TRAB2.C` — Student Records (File I/O)

A simpler console application for inserting and consulting student records:

- Stores grades, date of birth, and gender
- Binary file persistence

### `NOVO.C` — Article Catalogue

Console-based catalogue manager supporting two article types using a C `union` for type-specific fields:

- **Shoes**: size, colour
- **Boxes**: dimensions (height, width, depth)

---

## Building

This project targets **Turbo C 3** on **MS-DOS**.

1. Install [DOSBox](https://www.dosbox.com/) and Turbo C 3.
2. Mount your working directory inside DOSBox.
3. Open `WINC.PRJ` in the Turbo C IDE.
4. Press **F9** to compile and build.

> The include paths in `WINC.PRJ` are hardcoded to `K:\TC3\INCLUDE` and `K:\TC3\LIB`. Adjust these in the IDE under **Options → Directories** if your Turbo C installation is in a different location.

---

## Notes

- All source files use **DOS/Windows-1252 encoding** with Portuguese characters.
- The library relies heavily on Turbo C–specific functions: `cprintf`, `getch`, `window`, `textcolor`, `textbackground`, `gettext`, `puttext`, `clrscr`, `delay`, etc. — none of which are standard C.
- Files such as `TEMP.C`, `SNIFSNIF.C`, and `TECLAS.CPP` are simple scratch/test programs not part of the main library.
- Some menu actions in `ALNM.C` (Insert, Modify, Consult, List, Save As, Open) close the submenu but have no further implementation — they were left as stubs.
- Several known bugs exist in the demo apps (e.g. `acumulados` loop in `TTRABL.C`), consistent with their nature as student exercises.
