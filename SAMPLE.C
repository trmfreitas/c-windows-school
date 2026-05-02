/*
 * SAMPLE.C — "Janelas Para C" demonstration application
 *
 * A simple in-memory task manager that exercises the full windowing library:
 *   openwin / closewin / selectwin / movewin
 *   msgbox (INFO, ERRO, CRITICERRO types)
 *   callmenu / centra / cprintf / gotoxy
 *   SIMPLES and DUPLO border styles
 *
 * Build:
 *   make          (uses the provided Makefile)
 *
 * Run:
 *   ./sample
 */

#include <string.h>
#include <stdlib.h>
#include "WINC.H"

/* ---- Application constants ---------------------------------------------- */
#define MAX_TASKS  20
#define TASK_LEN   40

/* ---- Global task list ---------------------------------------------------- */
static char tasks[MAX_TASKS][TASK_LEN];
static int  task_count = 0;

/* ---- Window definitions -------------------------------------------------- */
static struct win mainwin = {
    2, 2, 78, 48,
    BLUE, WHITE, BLUE, LIGHTCYAN, YELLOW,
    " Janelas Para C - Task Manager ",
    DUPLO, NULL, {0}
};

/* ======================================================================== */
/* draw_desktop                                                                */
/* ======================================================================== */
static void draw_desktop(void)
{
    int row, col;
    int rows, cols;

    getmaxyx(stdscr, rows, cols);

    /* Fill background with a pattern */
    textbackground(BLUE);
    textcolor(CYAN);
    window(1, 1, cols, rows);
    for (row = 1; row <= rows; row++) {
        for (col = 1; col <= cols; col++) {
            gotoxy(col, row);
            cprintf("%c", (row + col) % 4 == 0 ? '.' : ' ');
        }
    }
}

/* ======================================================================== */
/* draw_taskbar                                                                */
/* ======================================================================== */
static void draw_taskbar(void)
{
    int cols;
    int dummy;

    getmaxyx(stdscr, dummy, cols);
    (void)dummy;

    textbackground(BLACK);
    textcolor(YELLOW);
    window(1, 1, cols, 1);
    clrscr();
    gotoxy(1, 1);
    cprintf(" Janelas Para C  |  Task Manager Demo  |  2002/2003 -> %d", 2024);
}

/* ======================================================================== */
/* show_task_list                                                              */
/* ======================================================================== */
static void show_task_list(void)
{
    struct win listwin = {
        10, 8, 70, 38,
        BLUE, WHITE, BLUE, LIGHTBLUE, LIGHTGREEN,
        " Tasks ",
        SIMPLES, NULL, {0}
    };
    int i;

    openwin(&listwin, aVERT);
    gotoxy(1, 1);
    textcolor(YELLOW);
    cprintf("\r\n");
    if (task_count == 0) {
        textcolor(LIGHTRED);
        centra("No tasks yet.  Use 'Add Task' to create one.", listwin.x1 - listwin.x);
    } else {
        for (i = 0; i < task_count; i++) {
            textcolor(LIGHTCYAN);
            cprintf("  %2d. ", i + 1);
            textcolor(WHITE);
            cprintf("%s\r\n", tasks[i]);
        }
    }
    gotoxy(1, listwin.y1 - listwin.y - 2);
    textcolor(DARKGRAY);
    centra("Press any key to close", listwin.x1 - listwin.x);
    getch();
    closewin(&listwin);
}

/* ======================================================================== */
/* add_task                                                                    */
/* ======================================================================== */
static void add_task(void)
{
    struct win addwin = {
        15, 15, 65, 22,
        BLACK, WHITE, BLACK, LIGHTGREEN, YELLOW,
        " Add New Task ",
        SIMPLES, NULL, {0}
    };
    char namebuf[TASK_LEN + 3];  /* cgets layout: [0]=max [1]=read [2..]=str */

    if (task_count >= MAX_TASKS) {
        msgbox(OK, ERRO, "Task list is full!\nDelete a task first.", aNO);
        return;
    }

    openwin(&addwin, aHORIZ);

    gotoxy(2, 2);
    textcolor(YELLOW);
    cprintf("Task name:");

    gotoxy(2, 4);
    textcolor(WHITE);
    namebuf[0] = (char)(TASK_LEN - 1);
    cgets(namebuf);

    if (strlen(namebuf + 2) == 0) {
        closewin(&addwin);
        msgbox(OK, CRITIC, "Task name cannot be empty.", aNO);
        return;
    }

    if (msgbox(OK | CANCEL, INFO, "Add this task?", aNO) == bOK) {
        strncpy(tasks[task_count], namebuf + 2, TASK_LEN - 1);
        tasks[task_count][TASK_LEN - 1] = '\0';
        task_count++;
        closewin(&addwin);
        msgbox(OK, INFO, "Task added successfully!", aNO);
    } else {
        closewin(&addwin);
    }
}

/* ======================================================================== */
/* delete_last_task                                                            */
/* ======================================================================== */
static void delete_last_task(void)
{
    if (task_count == 0) {
        msgbox(OK, ERRO, "No tasks to delete.", aNO);
        return;
    }
    if (msgbox(OK | CANCEL, CRITICERRO,
               "Delete the last task?\nThis cannot be undone.", aNO) == bOK) {
        task_count--;
        msgbox(OK, INFO, "Last task deleted.", aNO);
    }
}

/* ======================================================================== */
/* show_about                                                                  */
/* ======================================================================== */
static void show_about(void)
{
    struct win about = {
        12, 10, 68, 36,
        BLUE, WHITE, BLUE, LIGHTBLUE, YELLOW,
        " About ",
        DUPLO, NULL, {0}
    };

    openwin(&about, aOBLIQ);

    gotoxy(1, 2);
    textcolor(YELLOW);
    centra("Janelas Para C  v0.6", about.x1 - about.x);

    gotoxy(1, 4);
    textcolor(LIGHTCYAN);
    centra("A vintage DOS windowing library", about.x1 - about.x);
    gotoxy(1, 5);
    centra("ported to ncurses for modern systems.", about.x1 - about.x);

    gotoxy(1, 7);
    textcolor(WHITE);
    centra("Original author:", about.x1 - about.x);
    gotoxy(1, 8);
    textcolor(LIGHTGREEN);
    centra("Tiago Freitas, 12 I1", about.x1 - about.x);
    gotoxy(1, 9);
    textcolor(WHITE);
    centra("Colegio de Gaia, 2002/2003", about.x1 - about.x);

    gotoxy(1, 11);
    textcolor(LIGHTGRAY);
    centra("Features demonstrated:", about.x1 - about.x);
    gotoxy(1, 12);
    textcolor(WHITE);
    centra("openwin/closewin  SIMPLES/DUPLO borders", about.x1 - about.x);
    gotoxy(1, 13);
    centra("msgbox  callmenu  animation  colours", about.x1 - about.x);

    gotoxy(1, 16);
    textcolor(DARKGRAY);
    centra("Press any key to close", about.x1 - about.x);

    getch();
    closewin(&about);
}

/* ======================================================================== */
/* draw_status_line                                                            */
/* ======================================================================== */
static void draw_status_line(int task_cnt)
{
    selectwin(mainwin);
    gotoxy(1, mainwin.y1 - mainwin.y - 2);
    textcolor(DARKGRAY);
    cprintf("  Tasks in memory: %d / %d    Arrows/Enter to navigate menu",
            task_cnt, MAX_TASKS);
}

/* ======================================================================== */
/* main                                                                        */
/* ======================================================================== */
int main(void)
{
    int  pos = 1;
    int  running = 1;
    char menu[] = "'Add Task' 'View Tasks' Delete About Quit";

    textmode(C80);   /* initialises ncurses */

    draw_desktop();
    draw_taskbar();
    openwin(&mainwin, aNO);

    selectwin(mainwin);
    gotoxy(1, 1);
    textcolor(LIGHTCYAN);
    centra("Welcome to the Janelas Para C demo!", mainwin.x1 - mainwin.x);
    gotoxy(1, 2);
    textcolor(WHITE);
    centra("Use the menu below to manage your task list.", mainwin.x1 - mainwin.x);

    while (running) {
        draw_status_line(task_count);

        /* Position menu at the bottom of the main window */
        selectwin(mainwin);
        gotoxy(1, mainwin.y1 - mainwin.y - 1);

        switch (callmenu(&pos, menu, BLUE, YELLOW)) {
            case 1: add_task();        break;
            case 2: show_task_list();  break;
            case 3: delete_last_task();break;
            case 4: show_about();      break;
            case 5:
                if (msgbox(OK | CANCEL, INFO, "Quit the demo?", aNO) == bOK)
                    running = 0;
                break;
            default:
                break;
        }

        /* Refresh main window content after sub-windows close */
        if (running) {
            selectwin(mainwin);
            gotoxy(1, 1);
            textcolor(LIGHTCYAN);
            centra("Welcome to the Janelas Para C demo!", mainwin.x1 - mainwin.x);
            gotoxy(1, 2);
            textcolor(WHITE);
            centra("Use the menu below to manage your task list.", mainwin.x1 - mainwin.x);
        }
    }

    closewin(&mainwin);
    winc_cleanup();
    return 0;
}
