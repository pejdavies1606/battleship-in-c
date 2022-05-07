#ifndef BATTLESHIP_LINE_H_
#define BATTLESHIP_LINE_H_

#define SIZE_CELL_STR 2

#define STR_TITLE_DEFENSE "DEFENSE"
#define STR_TITLE_OFFENSE "OFFENSE"
#define LEN_TITLE_STR 7

#define MAX_BUFFER_SIZE 50

typedef struct Line
{
   size_t position;
   char buffer[MAX_BUFFER_SIZE];
} Line_t;

#endif