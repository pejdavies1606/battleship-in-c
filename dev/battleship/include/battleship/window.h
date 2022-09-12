/*
 * window.h
 *
 *  Created on: 7 Oct 2018
 *      Author: pejdavies1606
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "battleship/commondefs.h"

void Window_Init(uint x, uint y, uint w, uint h);

void Window_Start(void);

void Window_Wait(void);

void Window_Destroy(void);

#endif /* WINDOW_H_ */
