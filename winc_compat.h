/*
 * winc_compat.h — Turbo C 3 / DOS console compatibility layer for ncurses
 *
 * Maps the Turbo C 3 text-mode API (cprintf, window, textcolor, gettext, …)
 * to ncurses so that the windowing library compiles and runs on any modern
 * POSIX system with a Unicode terminal.
 *
 * Include this header (via winc.h) instead of <conio.h>.
 * Link with -lncursesw.
 */

#ifndef WINC_COMPAT_H
#define WINC_COMPAT_H

/* Request ncurses wide-character (Unicode) support */
#ifndef _XOPEN_SOURCE_EXTENDED
#  define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* ---- Turbo C colour constants ------------------------------------------- */
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15

/* ---- text_info struct (Turbo C compatible) ------------------------------- */
struct text_info {
    unsigned char winleft;
    unsigned char wintop;
    unsigned char winright;
    unsigned char winbottom;
    unsigned char attribute;   /* (bg<<4)|fg  as in Turbo C              */
    unsigned char normattr;
    unsigned char currmode;
    unsigned char screenheight;
    unsigned char screenwidth;
    unsigned char curx;        /* 1-based, relative to current viewport  */
    unsigned char cury;
};

/* ---- Border style sentinels --------------------------------------------- */
/*
 * SIMPLES / DUPLO are opaque char pointers used as style identifiers.
 * Do NOT index into them directly; createwin() handles the translation.
 * Any other char* is treated as 6 raw bytes [UL, H, UR, V, LL, LR].
 */
extern char _winc_simples[7];
extern char _winc_duplo[7];
#define SIMPLES (_winc_simples)
#define DUPLO   (_winc_duplo)

/* ---- Buffer size for screen save/restore --------------------------------- */
/*
 * savewin calls gettext(x, y, x1+1, y1+1, buf) — one column and one row
 * wider than the window — so we allocate (cols+1) * (rows+1) chtype cells.
 *
 * Usage: malloc(calmem(wind->x, wind->y, wind->x1, wind->y1))
 * (do NOT multiply by 2 as the old code did — sizeof(chtype) is baked in)
 */
#define calmem(x,y,k,z) \
    ((size_t)((k)-(x)+2) * (size_t)((z)-(y)+2) * sizeof(chtype))

/* ---- textmode codes (kept for source compatibility) --------------------- */
#define C4350  0
#define C80    1

/* ---- getch override: maps ncurses KEY_* → DOS extended scan codes ------- */
/*
 * Original Turbo C pattern:  c = ((c=getch())==0) ? getch() : c;
 * Our winc_getch() returns scan codes directly (never 0 for arrow keys),
 * so the ==0 branch is never taken — the idiom still works correctly.
 */
#ifdef getch
#  undef getch
#endif
#define getch() winc_getch()
int winc_getch(void);

/* ---- cgets: Turbo C buffered console input ------------------------------ */
/*
 * buf[0] = max chars; buf[1] = chars actually read (set on return);
 * buf[2..] = null-terminated string; return value = &buf[2].
 */
char *cgets(char *buf);

/* ---- Console output ----------------------------------------------------- */
int  cprintf(const char *fmt, ...);
int  cputs(const char *s);

/* ---- Viewport / colour / cursor ----------------------------------------- */
void window(int x, int y, int x1, int y1);
void textcolor(int color);
void textbackground(int color);
void textattr(int attr);
void gotoxy(int x, int y);
int  wherex(void);
int  wherey(void);

/* ---- Screen management -------------------------------------------------- */
void clrscr(void);
void insline(void);

/* ---- Screen region save / restore --------------------------------------- */
/* Coordinates are 1-based absolute screen positions (not viewport-relative) */
int  gettext(int x, int y, int x1, int y1, void *buf);
int  puttext(int x, int y, int x1, int y1, void *buf);

/* ---- text_info query ---------------------------------------------------- */
void gettextinfo(struct text_info *ti);

/* ---- Misc --------------------------------------------------------------- */
void delay(unsigned int ms);
void textmode(int mode);   /* initialises ncurses on first call */

/* ---- Library lifecycle -------------------------------------------------- */
void winc_init(void);      /* called automatically by textmode() */
void winc_cleanup(void);   /* called automatically via atexit()  */

#endif /* WINC_COMPAT_H */
