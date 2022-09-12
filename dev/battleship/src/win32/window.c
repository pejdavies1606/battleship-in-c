/*
 * win32/window.c
 *
 *  Created on: 7 Oct 2018
 *      Author: pejdavies1606
 */

#ifdef _WIN32
#include "battleship/window.h"
#include <windows.h>

void Window_Init(uint pos_x, uint pos_y, uint size_w, uint size_h)
{
   // https://batchloaf.wordpress.com/2012/04/17/how-a-console-application-can-set-the-size-of-its-window/
   HWND wh = GetConsoleWindow();
   MoveWindow(wh, (int)pos_x, (int)pos_y, (int)size_w, (int)size_h, TRUE);
}

void Window_Start(void)
{
   // TODO
   return;
}

void Window_Destroy(void)
{
   // TODO
   return;
}
#endif