/*
 * TEXT.C — Janelas Para C — text and menu utilities
 *
 * Original: Tiago Freitas, 12º I1, Colégio de Gaia, 2002/2003
 * Ported to ncurses for modern compilers; all known bugs corrected.
 *
 * Bug fixes applied:
 *  1. leitura(): tolower() return value was discarded; now assigned to c.
 *  2. getpalavrapos(): getpalavra() result was leaked; now freed after use.
 *     Also added NULL guard for the strstr() result.
 *  3. getline() renamed to winc_getline() to avoid conflict with the
 *     POSIX getline() declared in <stdio.h>.
 *  4. gotoop(): getpalavra() and malloc'd s2 are freed after use.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "WINC.H"

/* ======================================================================== */
/* Word / multi-word utilities                                                 */
/* ======================================================================== */

/*
 * Count the number of words (or quoted multi-words) in a string.
 * A multi-word is enclosed in single quotes, e.g.  'hello world'.
 */
int getpalavras(char *texto)
{
    char *s3;
    int   i = 0, nw = 0;
    char  ant = '\0';

    s3 = (char *)malloc(strlen(texto) + 1);
    if (s3 == NULL) return 0;
    cleanespaces(strcpy(s3, texto));

    for (i = 0; s3[i] != '\0'; i++) {
        if (s3[i] == '\'' && ant != '\'') { nw++; }
        if (s3[i] != ' ' && s3[i] != '\'' && ant == ' ')  { nw++; }
        if (s3[i] != ' ' && s3[i] != '\'' && i == 0)      { nw++; }
        ant = s3[i];
    }
    free(s3);
    return nw;
}

/*
 * Return the nth word (1-based) from texto as a malloc'd string.
 * Caller must free() the result.
 * Strips surrounding single quotes for multi-words.
 */
char *getpalavra(int num, char *texto)
{
    char *s3, *result;
    int   i, nw = 0, start = 0, end = 0;
    char  ant = '\0';
    int   multi = 0;

    s3 = (char *)malloc(strlen(texto) + 1);
    if (s3 == NULL) return NULL;
    cleanespaces(strcpy(s3, texto));

    for (i = 0; s3[i] != '\0'; i++) {
        if (s3[i] == '\'') {
            if (ant != '\'') { nw++; multi = 1; start = i + 1; }
            else             { end = i; }
        }
        if (s3[i] != ' ' && s3[i] != '\'' && ant == ' ')  { nw++; start = i; multi = 0; }
        if (s3[i] != ' ' && s3[i] != '\'' && i == 0)      { nw++; start = i; multi = 0; }
        if (nw == num) {
            if (!multi) {
                /* find end of simple word */
                end = i;
                while (s3[end] != ' ' && s3[end] != '\0') end++;
                break;
            }
        }
        ant = s3[i];
    }

    if (nw < num) {
        free(s3);
        result = (char *)malloc(1);
        if (result) result[0] = '\0';
        return result;
    }

    result = (char *)malloc((size_t)(end - start) + 1);
    if (result != NULL) {
        strncpy(result, s3 + start, (size_t)(end - start));
        result[end - start] = '\0';
    }
    free(s3);
    return result;
}

/*
 * Return the byte offset of the nth word in linha.
 * BUG FIX: getpalavra() result is now freed; added NULL guard for strstr().
 */
int getpalavrapos(int num, char *linha)
{
    char *palavra;
    char *pos;
    int   result;

    palavra = getpalavra(num, linha);
    if (palavra == NULL) return 0;

    pos = strstr(linha, palavra);
    result = (pos != NULL) ? (int)(pos - linha) : 0;
    free(palavra);
    return result;
}

/* ======================================================================== */
/* String cleaning utilities                                                   */
/* ======================================================================== */

/*
 * Print s to the current window, stripping single-quote delimiters.
 * If modo != 0, only count the quotes without printing anything.
 * Returns the number of single-quote characters found in s.
 */
int printclean(char *s, char modo)
{
    int i, nq = 0;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == '\'') { nq++; }
        else if (!modo)   { cprintf("%c", (unsigned char)s[i]); }
    }
    return nq;
}

/*
 * Remove duplicate spaces from s2, collapsing runs of whitespace to one.
 * Modifies s2 in-place.
 */
void cleanespaces(char *s2)
{
    int i = 0, j = 0;
    char prev = '\0';

    while (s2[i] != '\0') {
        if (s2[i] == ' ' && prev == ' ') { i++; continue; }
        s2[j++] = s2[i];
        prev = s2[i++];
    }
    s2[j] = '\0';
}

/* ======================================================================== */
/* Multi-line string utilities                                                 */
/* ======================================================================== */

/*
 * Count the number of '\n'-separated lines in texto.
 * An empty string counts as 1 line.
 */
int getlines(char *texto)
{
    int i, n = 1;
    for (i = 0; texto[i] != '\0'; i++)
        if (texto[i] == '\n') n++;
    return n;
}

/*
 * Return the nth line (1-based) from texto as a malloc'd null-terminated
 * string with the '\n' stripped.
 * Renamed from getline() to winc_getline() to avoid conflict with POSIX.
 * Caller must free() the result.
 */
char *winc_getline(int num, char *texto)
{
    int   i, n = 1, start = 0, end;
    char *result;

    for (i = 0; texto[i] != '\0' && n < num; i++)
        if (texto[i] == '\n') { n++; start = i + 1; }

    end = start;
    while (texto[end] != '\0' && texto[end] != '\n') end++;

    result = (char *)malloc((size_t)(end - start) + 1);
    if (result != NULL) {
        strncpy(result, texto + start, (size_t)(end - start));
        result[end - start] = '\0';
    }
    return result;
}

/* ======================================================================== */
/* Output helpers                                                              */
/* ======================================================================== */

/*
 * Centre string s in a field of tam characters at the current cursor row.
 * Returns the 1-based starting column of the printed string.
 */
int centra(char *s, int tam)
{
    int t = (tam / 2) - (int)strlen(s) / 2 + printclean(s, 1) / 2;
    gotoxy(t, wherey());
    printclean(s, 0);
    return t;
}

/* ======================================================================== */
/* Single-character input                                                      */
/* ======================================================================== */

/*
 * Wait for the user to press one of the characters in v (case-insensitive).
 * Returns the character in lower-case.
 * BUG FIX: tolower() return value was previously discarded; now assigned.
 */
char leitura(char *v)
{
    char c;
    int  i, valid;
    do {
        c = (char)getch();
        c = (char)tolower((unsigned char)c);  /* BUG FIX: assign result */
        valid = 0;
        for (i = 0; v[i] != '\0'; i++)
            if (c == (char)tolower((unsigned char)v[i])) { valid = 1; break; }
    } while (!valid);
    return c;
}

/* ======================================================================== */
/* Menu helpers                                                                */
/* ======================================================================== */

/*
 * Print the entire menu string (with quotes stripped) and then overprint
 * item pos in the given highlight colours.
 *
 * BUG FIX: getpalavra() and the s2 substring are now freed after use.
 */
void gotoop(int pos, char *s, int corback, int corfore)
{
    struct text_info ti;
    int   tt, ppos;
    char  saved_attr;
    char *s2, *word;

    gettextinfo(&ti);
    saved_attr = (char)ti.attribute;

    /* Print the full menu string centred, get starting column */
    gotoxy(1, wherey());
    ppos = getpalavrapos(pos, s);
    tt   = centra(s, ti.winright - ti.winleft + 2) + ppos;

    /* Adjust for quote characters that centra strips from display */
    s2 = (char *)malloc(strlen(s) + 1);
    if (s2 != NULL) {
        strncpy(s2, s, (size_t)ppos);
        s2[ppos] = '\0';
        tt = tt - printclean(s2, 1);
        free(s2);
    }

    /* Overprint the selected item with highlight colours */
    gotoxy(tt, wherey());
    textbackground(corback);
    textcolor(corfore);
    word = getpalavra(pos, s);
    if (word != NULL) {
        cprintf("%s", word);
        free(word);
    }

    textattr((int)(unsigned char)saved_attr);
}

/*
 * Interactive horizontal menu.
 * Blocks until Enter is pressed, allowing Left/Right navigation.
 * Returns the 1-based index of the selected item.
 */
int callmenu(int *pos, char *string, int corback, int corfore)
{
    char c;
    int  nwords = getpalavras(string);
    do {
        gotoop(*pos, string, corback, corfore);
        c = (char)getch();
        c = (char)((c == 0) ? getch() : c);
        switch (c) {
            case 77:   /* Right */
                *pos = (*pos == nwords) ? 1 : *pos + 1;
                break;
            case 75:   /* Left */
                *pos = (*pos == 1) ? nwords : *pos - 1;
                break;
            default:
                break;
        }
    } while (c != 13);
    return *pos;
}
