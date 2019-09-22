/*
 * window.c
 *
 *  Created on: 7 Oct 2018
 *      Author: pejdavies1606
 */

#include "window.h"

#ifdef _WIN32
#include <windows.h>
#endif

void Window_Init(uint pos_x, uint pos_y, uint size_w, uint size_h)
{
#ifdef _WIN32
   // https://batchloaf.wordpress.com/2012/04/17/how-a-console-application-can-set-the-size-of-its-window/
   HWND wh = GetConsoleWindow();
   MoveWindow(wh, (int)pos_x, (int)pos_y, (int)size_w, (int)size_h, TRUE);
#endif
}
