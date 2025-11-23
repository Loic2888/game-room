#include "tetris.h"
/**
 * main - entry point for Tetris game
 *
 * Initializes the game board, enables raw terminal mode, and runs
 * the main game loop. Handles user input for piece movement and
 * rotation, manages automatic piece descent based on timing, and
 * detects game over conditions.
 * 
 * @p: current active piece structure
 * @board: 2D array representing the game board (HEIGHT x WIDTH)
 * @i: loop counter for board initialization (rows)
 * @j: loop counter for board initialization (columns)
 * @score: current player score
 * @game_over: flag indicating if game has ended (1) or continues (0)
 * @moved: flag indicating if piece moved during current input (1 or 0)
 * @last_tick: timestamp of last automatic piece descent (milliseconds)
 * @now: current timestamp (milliseconds)
 *
 * Game controls:
 * - 'q': move piece left
 * - 'd': move piece right
 * - 'z': rotate counterclockwise
 * - 's': rotate clockwise
 * - ' ': hard drop (instant descent)
 * - 'e': quit game
 *
 * The game loop runs at 100Hz (10ms per iteration) for responsive
 * input handling. Pieces automatically descend every TICK_USEC
 * milliseconds (default 500ms). When a piece can no longer descend,
 * it is fixed to the board, lines are cleared if complete, and a
 * new piece spawns. Game ends when a new piece cannot be placed.
 *
 * Return: 0 on successful execution
 */
int main(void)
{
	piece_t p;
	int board[HEIGHT][WIDTH];
	int i, j;
	int score = 0;
	int game_over = 0;
	int moved;
	long last_tick;
	long now;

	srand(time(NULL));

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++)
			board[i][j] = 0;

	enable_raw_mode();
	spawn_piece(&p);

	last_tick = get_time_msec();
	print_board(&p, board, score);

	while (!game_over)
	{
		usleep(10000); 

		if (kbhit())
		{
			char c;
			c = getch_nonblock();
			moved = 0;

			if (c == 'q' && valid_position(p.x - 1, p.y, p.mat, board))
			{
				p.x--;
				moved = 1;
			}
			else if (c == 'd' && valid_position(p.x + 1, p.y, p.mat, board))
			{
				p.x++;
				moved = 1;
			}
			else if (c == 'z')
			{
				rotate_left(&p, board);
				moved = 1;
			}
			else if (c == 's')
			{
				rotate_right(&p, board);
				moved = 1;
			}
			else if (c == ' ')
			{
				while (valid_position(p.x, p.y + 1, p.mat, board))
					p.y++;
				moved = 1;
			}
			else if (c == 'e')
			{
				game_over = 1;
				break;
			}

			if (moved)
				print_board(&p, board, score);
		}

		now = get_time_msec();
		if (now - last_tick >= TICK_USEC / 1000)
		{
			last_tick = now;

			if (valid_position(p.x, p.y + 1, p.mat, board))
			{
				p.y++;
			}
			else
			{
				fix_piece(&p, board);
				clear_lines(board, &score);
				spawn_piece(&p);

				if (!valid_position(p.x, p.y, p.mat, board))
					game_over = 1;
			}

			print_board(&p, board, score);
		}
	}

	disable_raw_mode();
	printf("\n\033[31mGame Over!\033[0m Final score: %d\n", score);
	printf("Merci d'avoir joué!\n");
	return 0;
}
