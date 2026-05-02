/*
 * WINC.H — Public API for the "Janelas Para C" windowing library
 *
 * Original: Tiago Freitas, 12º I1, Colégio de Gaia, 2002/2003
 * Ported to ncurses for modern compilers.
 *
 * Include this single header in your application:
 *   #include "WINC.H"
 * Link with:
 *   gcc … WINDOWS.o TEXT.o winc_compat.o -lncursesw
 */

#ifndef WINC_H
#define WINC_H

#include "winc_compat.h"   /* ncurses compat: colours, viewport, getch … */

/* ---- Animation modes ---------------------------------------------------- */
#define aNO    0   /* no animation         */
#define aHORIZ 1   /* horizontal expansion */
#define aVERT  2   /* vertical expansion   */
#define aOBLIQ 3   /* diagonal expansion   */

/* ---- Animation speeds (passed as 'velocidade' to createawin) ------------ */
#define vMRAPIDO  1
#define vRAPIDO  10
#define vMEDIO   30
#define vLENTO  100

/* ---- Message-box type profiles ------------------------------------------ */
#define INFO       -1
#define CRITIC     -2
#define ERRO       -3
#define CRITICERRO -4

/* ---- Message-box button bit-flags (combine with |) ---------------------- */
/* ncurses also defines OK as 0 (success code); undefine it first so our
   bitmask definition takes precedence in user code.                          */
#ifdef OK
#  undef OK
#endif
#define OK      0x01
#define CANCEL  0x02
#define ABORTAR 0x04
#define REPETIR 0x08

/* ---- Message-box return values ------------------------------------------ */
#define bOK        1
#define bCANCELADO 2
#define bABORTADO  3
#define bREPETIDO  4

/* ---- Window structure ---------------------------------------------------- */
/*
 * Initialise a window variable like this:
 *
 *   struct win mywin = {
 *       x, y, x1, y1,          // top-left and bottom-right (1-based)
 *       cb, cf,                 // window background & foreground colour
 *       bb, bf,                 // border  background & foreground colour
 *       titcolor,               // title colour
 *       "Window Title",         // title string (max 79 chars)
 *       SIMPLES,                // border style: SIMPLES or DUPLO
 *       NULL                    // internal buffer — always NULL here
 *   };
 */
struct win {
    int   x, y, x1, y1;             /* position (1-based screen coords)  */
    int   cb, cf, bb, bf, titcolor; /* colours (use the constants above)  */
    char  titulo[80];               /* window title                        */
    char *borders;                  /* SIMPLES, DUPLO, or custom 6-char*  */
    char *buf;                      /* internal — always initialise NULL   */
    struct text_info oldtextmodes;  /* internal — do not touch             */
};

/* ---- WINDOWS.C ---------------------------------------------------------- */
int  savewin(struct win *wind);
int  loadwin(struct win *wind);
void createwin(struct win wind, int x, int y, int x1, int y1);
void createawin(struct win wind, int anim, int velocidade);
int  openwin(struct win *wind, int anim);
int  closewin(struct win *wind);
void selectwin(struct win wind);
int  movewin(struct win *wind, int x, int y);
int  msgbox(char botoes, int tipo, char *mensagem, char anim);

/* ---- TEXT.C ------------------------------------------------------------- */
int   getpalavras(char *texto);
char *getpalavra(int num, char *texto);   /* caller must free() result */
int   getpalavrapos(int num, char *linha);
int   printclean(char *s, char modo);
void  cleanespaces(char *s2);
int   getlines(char *texto);
char *winc_getline(int num, char *texto); /* caller must free() result */
int   centra(char *s, int tam);
char  leitura(char *v);
void  gotoop(int pos, char *s, int corback, int corfore);
int   callmenu(int *pos, char *string, int corback, int corfore);

#endif /* WINC_H */
