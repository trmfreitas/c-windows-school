/*
 * WINDOWS.C — Janelas Para C — window engine
 *
 * Original: Tiago Freitas, 12º I1, Colégio de Gaia, 2002/2003
 * Ported to ncurses for modern compilers; all known bugs corrected.
 *
 * Bug fixes applied:
 *  1. savewin / loadwin: gettext/puttext region now consistently uses
 *     x1+1, y1+1 in both functions (loadwin previously used y1, not y1+1).
 *  2. createawin: added missing break before case aNO so aOBLIQ does not
 *     fall through and draw the final frame a second time.
 *  3. createwin: NULL-pointer checks use != NULL instead of != '\0'.
 *  4. msgbox: button-separator check reads barraops[strlen-1] instead of
 *     the byte past the null terminator.
 *  5. msgbox: getpalavra() results are stored and freed to prevent leaks.
 *  6. gotoop: getpalavra() result stored in a local and freed after use.
 *  7. movewin: completely rewritten; old version saved background at the
 *     wrong (old) position and had type-narrowing bugs in char variables.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "WINC.H"

/* ======================================================================== */
/* Internal helpers                                                            */
/* ======================================================================== */

/*
 * Emit one border character at the current cursor position.
 *
 * SIMPLES  → ncurses ACS line-drawing characters (work on any terminal)
 * DUPLO    → Unicode double-line box characters via wide-char API
 * other    → the raw byte is output as-is
 *
 * idx:  0=UL corner  1=horizontal  2=UR corner
 *       3=vertical   4=LL corner   5=LR corner
 */
static void add_border_char(const char *borders, int idx)
{
    if (borders == _winc_simples) {
        /* ACS_* are runtime values; cannot use as static array initializers */
        chtype acs[6];
        acs[0] = ACS_ULCORNER; acs[1] = ACS_HLINE;    acs[2] = ACS_URCORNER;
        acs[3] = ACS_VLINE;    acs[4] = ACS_LLCORNER;  acs[5] = ACS_LRCORNER;
        addch(acs[idx]);
    } else if (borders == _winc_duplo) {
        static const wchar_t dbl[6] = {
            (wchar_t)0x2554, (wchar_t)0x2550, (wchar_t)0x2557,  /* ╔ ═ ╗ */
            (wchar_t)0x2551, (wchar_t)0x255a, (wchar_t)0x255d   /* ║ ╚ ╝ */
        };
        cchar_t cch;
        wchar_t ws[2];
        attr_t  cur_attr;
        short   cur_pair;
        ws[0] = dbl[idx];
        ws[1] = L'\0';
        wattr_get(stdscr, &cur_attr, &cur_pair, NULL);
        setcchar(&cch, ws, cur_attr & ~A_COLOR, cur_pair, NULL);
        wadd_wch(stdscr, &cch);
    } else if (borders != NULL) {
        addch((chtype)(unsigned char)borders[idx]);
    }
}

/* ======================================================================== */
/* savewin / loadwin                                                           */
/* ======================================================================== */

/*
 * Save everything behind a window (background + old viewport state).
 * wind->buf must be NULL; returns 0 on success, 1 on error.
 */
int savewin(struct win *wind)
{
    struct text_info ti;

    if (wind->buf != NULL)
        return 1;  /* error: buf must start as NULL */

    gettextinfo(&ti);
    wind->oldtextmodes      = ti;
    wind->oldtextmodes.curx = (unsigned char)wherex();
    wind->oldtextmodes.cury = (unsigned char)wherey();

    wind->buf = (char *)malloc(calmem(wind->x, wind->y, wind->x1, wind->y1));
    if (wind->buf == NULL)
        return 1;

    /* Save one extra column and row (x1+1, y1+1) to capture the full border */
    if (gettext(wind->x, wind->y, wind->x1 + 1, wind->y1 + 1, wind->buf) == 0) {
        free(wind->buf);
        wind->buf = NULL;
        return 1;
    }
    return 0;
}

/*
 * Restore everything behind a window and release memory.
 * Returns 0 on success, 1 if the window was not open.
 */
int loadwin(struct win *wind)
{
    if (wind->buf == NULL)
        return 1;  /* window was never opened */

    /* Restore saved region — coordinates must match savewin exactly */
    puttext(wind->x, wind->y, wind->x1 + 1, wind->y1 + 1, wind->buf);
    free(wind->buf);
    wind->buf = NULL;

    /* Restore the viewport and colour state that were active before openwin */
    window(wind->oldtextmodes.winleft,
           wind->oldtextmodes.wintop,
           wind->oldtextmodes.winright,
           wind->oldtextmodes.winbottom);
    textattr(wind->oldtextmodes.attribute);
    gotoxy(wind->oldtextmodes.curx, wind->oldtextmodes.cury);
    return 0;
}

/* ======================================================================== */
/* selectwin                                                                   */
/* ======================================================================== */

/* Set the active viewport to the inner (content) area of a window. */
void selectwin(struct win wind)
{
    window(wind.x + 1, wind.y + 1, wind.x1 - 1, wind.y1 - 1);
    textcolor(wind.cf);
    textbackground(wind.cb);
}

/* ======================================================================== */
/* closewin                                                                    */
/* ======================================================================== */

/* Close a window and restore what was behind it.  Returns 0 on success. */
int closewin(struct win *wind)
{
    return loadwin(wind);
}

/* ======================================================================== */
/* createwin                                                                   */
/* ======================================================================== */

/*
 * Draw the border (and title at the final position) for a window.
 * All coordinates are 1-based absolute screen positions.
 * Uses absolute mvaddch() so the global viewport is not disturbed.
 */
void createwin(struct win wind, int x, int y, int x1, int y1)
{
    int i;

    textbackground(wind.bb);
    textcolor(wind.bf);

    if (wind.borders != NULL && wind.borders[0] != '\0') {
        /* Top border row */
        move(y - 1, x - 1);
        add_border_char(wind.borders, 0);                 /* top-left corner  */
        for (i = x; i <= x1 - 2; i++) {
            move(y - 1, i);
            add_border_char(wind.borders, 1);             /* top horizontal   */
        }
        if (x1 > x) {
            move(y - 1, x1 - 1);
            add_border_char(wind.borders, 2);             /* top-right corner */
        }

        /* Side borders */
        for (i = y + 1; i <= y1 - 1; i++) {
            move(i - 1, x - 1);
            add_border_char(wind.borders, 3);             /* left vertical    */
            if (x1 > x) {
                move(i - 1, x1 - 1);
                add_border_char(wind.borders, 3);         /* right vertical   */
            }
        }

        /* Bottom border row */
        if (y1 > y) {
            move(y1 - 1, x - 1);
            add_border_char(wind.borders, 4);             /* bottom-left      */
            for (i = x; i <= x1 - 2; i++) {
                move(y1 - 1, i);
                add_border_char(wind.borders, 1);         /* bottom horiz.    */
            }
            if (x1 > x) {
                move(y1 - 1, x1 - 1);
                add_border_char(wind.borders, 5);         /* bottom-right     */
            }
        }
    }

    /* Title — only drawn when we have reached the window's final position */
    if (wind.x == x && wind.y == y && wind.x1 == x1 && wind.y1 == y1) {
        if (wind.titulo[0] != '\0') {
            int tlen     = (int)strlen(wind.titulo);
            int win_w    = x1 - x + 1;
            int title_col = (x - 1) + win_w / 2 - tlen / 2;
            if (title_col < x)      title_col = x;
            if (title_col > x1 - 2) title_col = x1 - 2;
            textcolor(wind.titcolor);
            move(y - 1, title_col);
            for (i = 0; wind.titulo[i] != '\0' && title_col + i < x1 - 1; i++)
                addch((unsigned char)wind.titulo[i]);
        }
    }

    refresh();
}

/* ======================================================================== */
/* createawin                                                                  */
/* ======================================================================== */

/*
 * Draw a window with an optional opening animation.
 * Bug fix: added break after aOBLIQ so it no longer falls through to aNO
 * and redraws the final frame a second time.
 */
void createawin(struct win wind, int anim, int velocidade)
{
    int i, j;

    switch (anim) {
        case aHORIZ:
            for (i = wind.x; i <= wind.x1; i++) {
                createwin(wind, wind.x, wind.y, i, wind.y1);
                delay((unsigned int)velocidade);
            }
            break;

        case aVERT:
            for (i = wind.y; i <= wind.y1; i++) {
                createwin(wind, wind.x, wind.y, wind.x1, i);
                delay((unsigned int)velocidade);
            }
            break;

        case aOBLIQ:
            j = wind.y;
            for (i = wind.x; i <= wind.x1; i++) {
                if (j < wind.y1) j++;
                createwin(wind, wind.x, wind.y, i, j);
                delay((unsigned int)velocidade);
            }
            break;  /* BUG FIX: was missing; caused second draw of final frame */

        case aNO:
        default:
            createwin(wind, wind.x, wind.y, wind.x1, wind.y1);
            break;
    }
}

/* ======================================================================== */
/* openwin                                                                     */
/* ======================================================================== */

/* Save background, draw the window, and set focus.  Returns 0 on success. */
int openwin(struct win *wind, int anim)
{
    if (savewin(wind) != 0)
        return 1;

    createawin(*wind, anim, 1);

    /* Focus the inner content area */
    if (wind->borders != NULL)
        selectwin(*wind);
    textbackground(wind->cb);
    textcolor(wind->cf);
    clrscr();
    return 0;
}

/* ======================================================================== */
/* movewin                                                                     */
/* ======================================================================== */

/*
 * Move a window to a new (x, y) position.
 *
 * Algorithm:
 *  1. Capture the rendered window pixels into a temporary buffer.
 *  2. Restore the background at the old position (loadwin frees wind->buf).
 *  3. Update the window coordinates.
 *  4. Save the new background at the new position into wind->buf.
 *  5. Stamp the captured pixels at the new position.
 *
 * Returns 0 on success, 1 on error.
 *
 * Bug fixes vs original:
 *  - curx/cury/x1/y1 were char (sign-overflow on positions > 127); now int.
 *  - Old code saved background at the OLD position (already cleared), not NEW.
 *  - Old code called gettext twice on the old region (double work, wrong data).
 */
int movewin(struct win *wind, int x, int y)
{
    char *buf0;
    int   curx, cury, new_x1, new_y1;

    new_x1 = x + (wind->x1 - wind->x);
    new_y1 = y + (wind->y1 - wind->y);

    /* Step 1: capture the rendered window at the current position */
    buf0 = (char *)malloc(calmem(wind->x, wind->y, wind->x1, wind->y1));
    if (buf0 == NULL) return 1;
    gettext(wind->x, wind->y, wind->x1, wind->y1, buf0);

    /* Remember cursor position relative to the window interior */
    curx = wherex();
    cury = wherey();

    /* Step 2: restore the background at the OLD position */
    loadwin(wind);   /* sets wind->buf = NULL */

    /* Step 3: update coordinates */
    wind->x  = x;
    wind->y  = y;
    wind->x1 = new_x1;
    wind->y1 = new_y1;

    /* Step 4: save background at the NEW position */
    wind->buf = (char *)malloc(calmem(wind->x, wind->y, wind->x1, wind->y1));
    if (wind->buf == NULL) {
        free(buf0);
        return 1;
    }
    if (gettext(wind->x, wind->y, wind->x1 + 1, wind->y1 + 1, wind->buf) == 0) {
        free(wind->buf);
        wind->buf = NULL;
        free(buf0);
        return 1;
    }

    /* Step 5: stamp the window pixels at the new position */
    puttext(wind->x, wind->y, wind->x1, wind->y1, buf0);
    free(buf0);

    selectwin(*wind);
    gotoxy(curx, cury);
    return 0;
}

/* ======================================================================== */
/* msgbox                                                                      */
/* ======================================================================== */

/*
 * Display a modal message box.
 * Returns bOK / bCANCELADO / bABORTADO / bREPETIDO, or 0 on error.
 *
 * Bug fixes vs original:
 *  - Separator check read barraops[strlen(barraops)] (past null terminator).
 *    Fixed to check barraops[strlen-1] when barraops is non-empty.
 *  - getpalavra() results are now stored and freed to prevent memory leaks.
 */
int msgbox(char botoes, int tipo, char *mensagem, char anim)
{
    struct win wmsgbox = {
        20, 18, 60, 22, BLUE, WHITE, BLUE, WHITE, LIGHTGREEN,
        " Mensagem ", SIMPLES, NULL, {0}
    };
    unsigned char ok = 0, cancel = 0, abortar = 0, repet = 0;
    unsigned char i, numlines;
    int pos, ret;
    char barraops[60] = "";
    char *word;
    char *line;

    if ((botoes & OK)      == OK)      ok      = 1;
    if ((botoes & CANCEL)  == CANCEL)  cancel  = 1;
    if ((botoes & ABORTAR) == ABORTAR) abortar = 1;
    if ((botoes & REPETIR) == REPETIR) repet   = 1;

    numlines = (unsigned char)getlines(mensagem);
    wmsgbox.y1 += numlines + 1;

    switch (tipo) {
        case INFO:
            strcpy(wmsgbox.titulo, " Informacao ");
            wmsgbox.bf       = LIGHTBLUE;
            wmsgbox.bb       = BLUE;
            wmsgbox.cb       = WHITE;
            wmsgbox.titcolor = WHITE;
            break;
        case CRITIC:
            strcpy(wmsgbox.titulo, " Informacao Critica ");
            wmsgbox.bf       = YELLOW;
            wmsgbox.bb       = LIGHTBLUE;
            wmsgbox.cb       = WHITE;
            wmsgbox.titcolor = LIGHTGREEN;
            break;
        case ERRO:
            strcpy(wmsgbox.titulo, " Erro ");
            wmsgbox.bf       = WHITE;
            wmsgbox.bb       = RED;
            wmsgbox.cb       = RED;
            wmsgbox.titcolor = YELLOW;
            break;
        case CRITICERRO:
            strcpy(wmsgbox.titulo, " Erro Critico ");
            wmsgbox.bf       = BLACK;
            wmsgbox.bb       = WHITE;
            wmsgbox.cb       = LIGHTGRAY;
            wmsgbox.titcolor = RED;
            break;
        default:
            wmsgbox.bf       = tipo + 2;
            wmsgbox.bb       = tipo;
            wmsgbox.cb       = tipo;
            wmsgbox.cf       = tipo + 1;
            wmsgbox.titcolor = YELLOW;
            break;
    }

    if (openwin(&wmsgbox, anim) != 0)
        return 0;

    /* Print message lines */
    gotoxy(1, 2);
    line = winc_getline(1, mensagem);
    centra(line, wmsgbox.x1 - wmsgbox.x - 1);
    free(line);
    for (i = 2; i <= numlines; i++) {
        cprintf("\r\n");
        line = winc_getline(i, mensagem);
        centra(line, wmsgbox.x1 - wmsgbox.x - 1);
        free(line);
    }

    /* Build button bar string */
    if (ok)      strcat(barraops, "[OK]");
    if (cancel) {
        /* BUG FIX: check last char in non-empty string, not char past '\0' */
        if (barraops[0] != '\0' && barraops[strlen(barraops) - 1] != ' ')
            strcat(barraops, "   ");
        strcat(barraops, "[CANCELAR]");
    }
    if (abortar) {
        if (barraops[0] != '\0' && barraops[strlen(barraops) - 1] != ' ')
            strcat(barraops, "   ");
        strcat(barraops, "[ABORTAR]");
    }
    if (repet) {
        if (barraops[0] != '\0' && barraops[strlen(barraops) - 1] != ' ')
            strcat(barraops, "   ");
        strcat(barraops, "[REPETIR]");
    }

    textcolor(YELLOW);
    gotoxy(1, wmsgbox.y1 - wmsgbox.y - 1);
    centra(barraops, wmsgbox.x1 - wmsgbox.x);

    pos = 1;
    callmenu(&pos, barraops, BLUE, WHITE);
    closewin(&wmsgbox);

    /* BUG FIX: free getpalavra() result after use */
    word = getpalavra(pos, barraops);
    ret  = 0;
    if (strcmp(word, "[OK]")       == 0) ret = bOK;
    else if (strcmp(word, "[CANCELAR]") == 0) ret = bCANCELADO;
    else if (strcmp(word, "[ABORTAR]")  == 0) ret = bABORTADO;
    else if (strcmp(word, "[REPETIR]")  == 0) ret = bREPETIDO;
    free(word);
    return ret;
}
