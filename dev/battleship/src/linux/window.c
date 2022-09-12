/*
 * linux/window.c
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#ifdef linux
#include "battleship/window.h"
#include <ncurses.h>

void Window_Init(uint x, uint y, uint w, uint h)
{
   UNUSED(x); UNUSED(y); UNUSED(w); UNUSED(h);
   initscr();
   raw();
   //cbreak();
   keypad(stdscr, TRUE);
   noecho();
}

void Window_Start(void)
{
   printw("Hello world!\n");
   printw("(%d,%d)\n", COLS, LINES);
   refresh();
}

void Window_Wait(void)
{
   mvaddstr(0, 0, "Press any key...\n");
   refresh();
   getch();
}

void Window_Destroy(void)
{
   clear();
   refresh();
   endwin();
}
#endif