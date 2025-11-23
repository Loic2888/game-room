#include "tetris.h"

static struct termios orig_termios;
/**
 * enable_raw_mode - activates raw mode for terminal input
 *
 * Disables canonical mode and echo to allow immediate character reading
 * without waiting for newline. Saves original terminal attributes in
 * orig_termios for later restoration.
 *
 * Return: void
 */
void enable_raw_mode(void)
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, &orig_termios);
	raw = orig_termios;
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
/**
 * disable_raw_mode - restores original terminal settings
 *
 * Reapplies the terminal attributes that were saved before entering
 * raw mode. Should be called before program termination.
 *
 * Return: void
 */
void disable_raw_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}
/**
 * kbhit - checks if keyboard input is available
 *
 * Tests for pending input without blocking program execution.
 * Temporarily sets stdin to non-blocking mode, attempts to read
 * a character, then restores the original flags.
 *
 * Return: 1 if a key is available, 0 otherwise
 */

int kbhit(void)
{
	int oldf;
	int ch;
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}
/**
 * getch_nonblock - reads a character without blocking
 *
 * Retrieves one character from standard input in non-blocking mode.
 * Works together with kbhit() to provide responsive input handling.
 *
 * Return: character read, or 0 if none available
 */
char getch_nonblock(void)
{
	int ch = getchar();
	if (ch == EOF)
		return 0;
	return (char)ch;
}
/**
 * get_time_msec - gets current time in milliseconds
 *
 * Uses gettimeofday() to retrieve system time and converts it to
 * milliseconds. Used for timing automatic piece descent.
 *
 * Return: current time in milliseconds as long integer
 */
long get_time_msec(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
static int pieces[7][4][4] = {
	{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};
/**
 * spawn_piece - creates a new random tetromino
 * @p: pointer to piece structure to initialize
 *
 * Selects a random shape from the 7 available tetrominos and
 * initializes the piece at the top center of the board.
 * Sets coordinates to (WIDTH/2 - 2, 0) and copies the
 * shape matrix into the piece structure.
 *
 * Return: void
 */
void spawn_piece(piece_t *p)
{
	int i, j;
	int idx;
	idx = rand() % 7;
	p->x = WIDTH / 2 - 2;
	p->y = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			p->mat[i][j] = pieces[idx][i][j];
}
/**
 * valid_position - checks if piece placement is valid
 * @nx: x coordinate to test
 * @ny: y coordinate to test
 * @mat: 4x4 matrix representing the piece shape
 * @board: game board array
 *
 * Verifies whether a piece can be placed at the given position
 * without colliding with board boundaries or existing blocks.
 *
 * Return: 1 if position is valid, 0 otherwise
 */

int valid_position(int nx, int ny, int mat[4][4], int board[HEIGHT][WIDTH])
{
	int px, py, x, y;
	for (py = 0; py < 4; py++)
	{
		for (px = 0; px < 4; px++)
		{
			if (mat[py][px])
			{
				x = nx + px;
				y = ny + py;
				if (x < 0 || x >= WIDTH || y >= HEIGHT)
					return 0;
				if (y >= 0 && board[y][x])
					return 0;
			}
		}
	}
	return 1;
}
/**
 * fix_piece - permanently places piece on the board
 * @p: pointer to piece to fix
 * @board: game board array
 *
 * Transfers all blocks from the piece matrix to the game board
 * at their current positions. Blocks above the board (y < 0)
 * are ignored.
 *
 * Return: void
 */
void fix_piece(piece_t *p, int board[HEIGHT][WIDTH])
{
	int px, py, x, y;
	for (py = 0; py < 4; py++)
	{
		for (px = 0; px < 4; px++)
		{
			if (p->mat[py][px] && p->y + py >= 0)
			{
				x = p->x + px;
				y = p->y + py;
				board[y][x] = 1;
			}
		}
	}
}
/**
 * clear_lines - removes completed lines and updates score
 * @board: game board array
 * @score: pointer to score variable
 *
 * Scans the board for complete horizontal lines. For each
 * complete line found, increments score by WIDTH, moves all
 * lines above down by one position, and clears the top line.
 *
 * Return: void
 */
void clear_lines(int board[HEIGHT][WIDTH], int *score)
{
	int y, x, ty, tx, full;
	for (y = 0; y < HEIGHT; y++)
	{
		full = 1;
		for (x = 0; x < WIDTH; x++)
		{
			if (board[y][x] == 0)
				full = 0;
		}
		if (full)
		{
			*score += WIDTH;
			for (ty = y; ty > 0; ty--)
			{
				for (tx = 0; tx < WIDTH; tx++)
					board[ty][tx] = board[ty - 1][tx];
			}
			for (tx = 0; tx < WIDTH; tx++)
				board[0][tx] = 0;
		}
	}
}
/**
 * rotate_right - rotates piece 90 degrees clockwise
 * @p: pointer to piece to rotate
 * @board: game board array
 *
 * Performs clockwise rotation using matrix transformation
 * tmp[x][3-y] = mat[y][x]. Applies rotation only if the
 * resulting position is valid.
 *
 * Return: void
 */
void rotate_right(piece_t *p, int board[HEIGHT][WIDTH])
{
	int tmp[4][4];
	int x, y;
	for (y = 0; y < 4; y++)
		for (x = 0; x < 4; x++)
			tmp[x][3 - y] = p->mat[y][x];
	if (valid_position(p->x, p->y, tmp, board))
		for (y = 0; y < 4; y++)
			for (x = 0; x < 4; x++)
				p->mat[y][x] = tmp[y][x];
}
/**
 * rotate_left - rotates piece 90 degrees counterclockwise
 * @p: pointer to piece to rotate
 * @board: game board array
 *
 * Performs counterclockwise rotation using matrix transformation
 * tmp[3-x][y] = mat[y][x]. Applies rotation only if the
 * resulting position is valid.
 *
 * Return: void
 */
void rotate_left(piece_t *p, int board[HEIGHT][WIDTH])
{
	int tmp[4][4];
	int x, y;
	for (y = 0; y < 4; y++)
		for (x = 0; x < 4; x++)
			tmp[3 - x][y] = p->mat[y][x];
	if (valid_position(p->x, p->y, tmp, board))
		for (y = 0; y < 4; y++)
			for (x = 0; x < 4; x++)
				p->mat[y][x] = tmp[y][x];
}
/**
 * print_board - displays current game state
 * @p: pointer to current active piece
 * @board: game board array
 * @score: current score value
 *
 * Clears screen and renders the complete game state including
 * the active piece, fixed blocks, empty spaces, and score.
 * Uses ANSI color codes for visual distinction.
 *
 * Return: void
 */
void print_board(piece_t *p, int board[HEIGHT][WIDTH], int score)
{
	int x, y, px, py, is_piece;
	system("clear");
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			is_piece = 0;
			for (py = 0; py < 4; py++)
				for (px = 0; px < 4; px++)
					if (p->mat[py][px] && y == p->y + py && x == p->x + px)
						is_piece = 1;
			if (is_piece || board[y][x])
				printf("\033[33m[]\033[0m");
			else
				printf("\033[34m .\033[0m");
		}
		printf("\n");
	}
	printf("Score: %d\n", score);
}
