/*
 * winc_compat.c — ncurses implementation of the Turbo C 3 console API
 *
 * Provides window(), textcolor(), cprintf(), gettext(), getch() …
 * as thin wrappers over ncurses so the vintage windowing library runs
 * on any modern POSIX terminal without modification.
 */

#define _XOPEN_SOURCE_EXTENDED 1

#include <ncurses.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "winc_compat.h"

/* ======================================================================== */
/* Border style sentinels                                                     */
/* ======================================================================== */

/*
 * The six bytes in each array are opaque sentinels — they are compared by
 * pointer address in createwin() to choose the correct ACS / Unicode glyphs.
 */
char _winc_simples[7] = "\x01\x02\x03\x04\x05\x06";
char _winc_duplo[7]   = "\x11\x12\x13\x14\x15\x16";

/* ======================================================================== */
/* Global viewport state                                                       */
/* ======================================================================== */

typedef struct {
    int left, top, right, bottom; /* 1-based screen coords of current window */
    int fg, bg;                   /* current Turbo C colour indices (0-15)    */
} Viewport;

static Viewport vp = {1, 1, 80, 24, WHITE, BLACK};
static int ncurses_started = 0;

/* ======================================================================== */
/* Colour management                                                           */
/* ======================================================================== */

/*
 * Map a Turbo C colour (0-15) to the corresponding ncurses base colour (0-7).
 * Bright variants (8-15) use the same base colour; A_BOLD is added separately.
 */
static int tc_to_nc_color(int tc)
{
    static const int map[16] = {
        COLOR_BLACK,   /* 0  BLACK        */
        COLOR_BLUE,    /* 1  BLUE         */
        COLOR_GREEN,   /* 2  GREEN        */
        COLOR_CYAN,    /* 3  CYAN         */
        COLOR_RED,     /* 4  RED          */
        COLOR_MAGENTA, /* 5  MAGENTA      */
        COLOR_YELLOW,  /* 6  BROWN (approx) */
        COLOR_WHITE,   /* 7  LIGHTGRAY    */
        COLOR_BLACK,   /* 8  DARKGRAY     */
        COLOR_BLUE,    /* 9  LIGHTBLUE    */
        COLOR_GREEN,   /* 10 LIGHTGREEN   */
        COLOR_CYAN,    /* 11 LIGHTCYAN    */
        COLOR_RED,     /* 12 LIGHTRED     */
        COLOR_MAGENTA, /* 13 LIGHTMAGENTA */
        COLOR_YELLOW,  /* 14 YELLOW       */
        COLOR_WHITE,   /* 15 WHITE        */
    };
    if (tc < 0 || tc > 15) tc = 7;
    return map[tc];
}

/* Return 1-based ncurses colour-pair index for a fg/bg combination. */
static int color_pair_idx(int fg_tc, int bg_tc)
{
    int fg_nc = tc_to_nc_color(fg_tc);
    int bg_nc = tc_to_nc_color(bg_tc & 7); /* background uses 8 variants only */
    return fg_nc * 8 + bg_nc + 1;          /* 1-indexed; pair 0 is reserved   */
}

/* Apply vp.fg / vp.bg as the current ncurses attribute set. */
static void apply_attrs(void)
{
    int pair = color_pair_idx(vp.fg, vp.bg);
    attr_t attrs = (attr_t)COLOR_PAIR(pair);
    if (vp.fg >= 8) attrs |= A_BOLD;
    attrset(attrs);
}

void textcolor(int color)
{
    vp.fg = color;
    apply_attrs();
}

void textbackground(int color)
{
    vp.bg = color;
    apply_attrs();
}

void textattr(int attr)
{
    vp.fg =  attr & 0x0f;
    vp.bg = (attr >> 4) & 0x07;
    apply_attrs();
}

/* ======================================================================== */
/* Viewport                                                                    */
/* ======================================================================== */

void window(int x, int y, int x1, int y1)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    vp.left   = (x  < 1)    ? 1    : x;
    vp.top    = (y  < 1)    ? 1    : y;
    vp.right  = (x1 > cols) ? cols : x1;
    vp.bottom = (y1 > rows) ? rows : y1;
    move(vp.top - 1, vp.left - 1);
}

/* ======================================================================== */
/* Cursor                                                                      */
/* ======================================================================== */

void gotoxy(int x, int y)
{
    move(vp.top + y - 2, vp.left + x - 2);
}

int wherex(void)
{
    return getcurx(stdscr) - vp.left + 2;
}

int wherey(void)
{
    return getcury(stdscr) - vp.top + 2;
}

/* ======================================================================== */
/* Screen management                                                           */
/* ======================================================================== */

void clrscr(void)
{
    int row, col;
    apply_attrs();
    for (row = vp.top - 1; row <= vp.bottom - 1; row++)
        for (col = vp.left - 1; col <= vp.right - 1; col++)
            mvaddch(row, col, ' ');
    move(vp.top - 1, vp.left - 1);
    refresh();
}

void insline(void)
{
    int row, col;
    int cur_row = getcury(stdscr);

    /* Shift viewport lines downward by one, starting from the bottom */
    for (row = vp.bottom - 2; row >= cur_row; row--)
        for (col = vp.left - 1; col <= vp.right - 1; col++)
            mvaddch(row + 1, col, mvwinch(stdscr, row, col));

    /* Clear the current row within the viewport */
    apply_attrs();
    for (col = vp.left - 1; col <= vp.right - 1; col++)
        mvaddch(cur_row, col, ' ');

    move(cur_row, vp.left - 1);
    refresh();
}

/* ======================================================================== */
/* Screen region save / restore                                                */
/* ======================================================================== */

/*
 * Coordinates are 1-based absolute screen positions.
 * Returns 1 on success, 0 on failure (Turbo C convention).
 */
int gettext(int x, int y, int x1, int y1, void *buf)
{
    chtype *cbuf = (chtype *)buf;
    int row, col, idx = 0;
    for (row = y - 1; row <= y1 - 1; row++)
        for (col = x - 1; col <= x1 - 1; col++)
            cbuf[idx++] = mvwinch(stdscr, row, col);
    return 1;
}

int puttext(int x, int y, int x1, int y1, void *buf)
{
    chtype *cbuf = (chtype *)buf;
    int row, col, idx = 0;
    for (row = y - 1; row <= y1 - 1; row++)
        for (col = x - 1; col <= x1 - 1; col++)
            mvaddch(row, col, cbuf[idx++]);
    refresh();
    return 1;
}

/* ======================================================================== */
/* text_info                                                                   */
/* ======================================================================== */

void gettextinfo(struct text_info *ti)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    ti->winleft      = (unsigned char)vp.left;
    ti->wintop       = (unsigned char)vp.top;
    ti->winright     = (unsigned char)vp.right;
    ti->winbottom    = (unsigned char)vp.bottom;
    ti->attribute    = (unsigned char)(((vp.bg & 7) << 4) | (vp.fg & 0x0f));
    ti->normattr     = ti->attribute;
    ti->currmode     = 0;
    ti->screenheight = (unsigned char)(rows < 255 ? rows : 255);
    ti->screenwidth  = (unsigned char)(cols < 255 ? cols : 255);
    ti->curx         = (unsigned char)wherex();
    ti->cury         = (unsigned char)wherey();
}

/* ======================================================================== */
/* Console output                                                              */
/* ======================================================================== */

int cprintf(const char *fmt, ...)
{
    va_list ap;
    char buf[4096];
    int i, n;

    va_start(ap, fmt);
    n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    for (i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '\r') {
            /* Carriage return: jump to left edge of the current viewport */
            move(getcury(stdscr), vp.left - 1);
        } else if (buf[i] == '\n') {
            /* Line feed: move down one row (no implicit CR) */
            int new_row = getcury(stdscr) + 1;
            if (new_row > vp.bottom - 1) new_row = vp.bottom - 1;
            move(new_row, getcurx(stdscr));
        } else {
            addch((unsigned char)buf[i]);
        }
    }
    refresh();
    return n;
}

int cputs(const char *s)
{
    return cprintf("%s", s);
}

/* ======================================================================== */
/* Input                                                                       */
/* ======================================================================== */

int winc_getch(void)
{
    int c = wgetch(stdscr);
    /*
     * Map ncurses KEY_* constants to DOS extended scan codes so the original
     * idiom  c = ((c=getch())==0) ? getch() : c  continues to work.
     * We return the scan code directly (never 0 for arrow keys), so the ==0
     * branch is never taken — the first call delivers the final value.
     */
    switch (c) {
        case KEY_UP:        return 72;
        case KEY_DOWN:      return 80;
        case KEY_LEFT:      return 75;
        case KEY_RIGHT:     return 77;
        case KEY_ENTER:     return 13;
        case KEY_BACKSPACE: return  8;
        case '\n':          return 13;  /* Enter on most terminals */
        default:            return c;
    }
}

char *cgets(char *buf)
{
    int maxlen = (int)(unsigned char)buf[0];
    int len = 0;
    int c;

    echo();
    curs_set(1);

    while (len < maxlen) {
        c = wgetch(stdscr);
        if (c == '\r' || c == '\n' || c == KEY_ENTER) break;
        if (c == KEY_BACKSPACE || c == 8 || c == 127) {
            if (len > 0) {
                int cx = getcurx(stdscr);
                int cy = getcury(stdscr);
                len--;
                if (cx > 0) {
                    mvaddch(cy, cx - 1, ' ');
                    move(cy, cx - 1);
                    refresh();
                }
            }
        } else if (c >= 32 && c < 256) {
            buf[2 + len] = (char)c;
            len++;
            addch((unsigned char)c);
            refresh();
        }
    }

    buf[2 + len] = '\0';
    buf[1] = (char)len;
    noecho();
    curs_set(0);
    return buf + 2;
}

/* ======================================================================== */
/* Delay                                                                       */
/* ======================================================================== */

void delay(unsigned int ms)
{
    napms((int)ms);
}

/* ======================================================================== */
/* Library initialisation / cleanup                                            */
/* ======================================================================== */

static void init_color_pairs(void)
{
    /*
     * Pre-initialise 64 colour pairs: foreground in 0-7, background in 0-7.
     * Pair index = fg * 8 + bg + 1  (1-based; pair 0 is the default, reserved)
     */
    int fg, bg;
    for (fg = 0; fg < 8; fg++)
        for (bg = 0; bg < 8; bg++)
            init_pair((short)(fg * 8 + bg + 1), (short)fg, (short)bg);
}

void winc_cleanup(void)
{
    if (ncurses_started) {
        endwin();
        ncurses_started = 0;
    }
}

void winc_init(void)
{
    int rows, cols;

    if (ncurses_started) return;

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_color_pairs();
    }

    getmaxyx(stdscr, rows, cols);
    vp.left   = 1;
    vp.top    = 1;
    vp.right  = cols;
    vp.bottom = rows;

    atexit(winc_cleanup);
    ncurses_started = 1;
}

/* textmode() is called at the top of every demo's main(); we hook init here */
void textmode(int mode)
{
    (void)mode; /* ignored — ncurses adapts to the actual terminal dimensions */
    winc_init();
}
