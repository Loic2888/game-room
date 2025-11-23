#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>

#define WIDTH 10
#define HEIGHT 20
#define TICK_USEC 500000

typedef struct piece_s
{
    int x;
    int y;
    int mat[4][4];
} piece_t;

void enable_raw_mode(void);
void disable_raw_mode(void);
int kbhit(void);
char getch_nonblock(void);

void spawn_piece(piece_t *p);
int valid_position(int nx, int ny, int mat[4][4], int board[HEIGHT][WIDTH]);
void fix_piece(piece_t *p, int board[HEIGHT][WIDTH]);
void clear_lines(int board[HEIGHT][WIDTH], int *score);
void rotate_right(piece_t *p, int board[HEIGHT][WIDTH]);
void rotate_left(piece_t *p, int board[HEIGHT][WIDTH]);
void print_board(piece_t *p, int board[HEIGHT][WIDTH], int score);
long get_time_msec(void);

#endif
