#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
/* Check if a character represents a wall */
int is_wall(char c)
{
	return (c == '#');
}
/*
 * Print the game map
 * map         : the 2D map array
 * width       : map width
 * height      : map height
 * player      : pointer to the player entity
 * enemies     : array of enemy entities
 * enemy_count : number of enemies
 * fruit       : pointer to the fruit entity
 */
void print_map(char map[MAX_HEIGHT][MAX_WIDTH], int width, int height,
			   Entity *player, Entity enemies[], int enemy_count, Entity *fruit)
{
	int i, j, e;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (i == player->y && j == player->x)
				printf("P");
			else if (i == fruit->y && j == fruit->x)
				printf("*");
			else
			{
				int printed = 0;
				for (e = 0; e < enemy_count; e++)
				{
					if (i == enemies[e].y && j == enemies[e].x)
					{
						printf("E");
						printed = 1;
						break;
					}
				}
				if (!printed)
					printf("%c", map[i][j]);
			}
		}
		printf("\n");
	}
}
/*
 * Move the player according to input
 * input : character representing direction (z/q/s/d)
 * map   : the 2D map array
 * width : map width
 * height: map height
 * player: pointer to the player entity
 */
void move_player(char input, char map[MAX_HEIGHT][MAX_WIDTH],
				 int width, int height, Entity *player)
{
	int new_x = player->x;
	int new_y = player->y;

	if (input == 'z')
		new_y--;
	else if (input == 's')
		new_y++;
	else if (input == 'q')
		new_x--;
	else if (input == 'd')
		new_x++;
	if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height)
	{
		if (!is_wall(map[new_y][new_x]))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
}
/*
 * Move an enemy towards the player
 * map   : the 2D map array
 * player: pointer to the player entity
 * enemy : pointer to the enemy entity
 */
void move_enemy(char map[MAX_HEIGHT][MAX_WIDTH], Entity *player, Entity *enemy)
{
	int dx = 0, dy = 0;

	(void)map;

	if (enemy->x < player->x)
		dx = 1;
	else if (enemy->x > player->x)
		dx = -1;

	if (enemy->y < player->y)
		dy = 1;
	else if (enemy->y > player->y)
		dy = -1;
	if (dx != 0 && !is_wall(map[enemy->y][enemy->x + dx]))
		enemy->x += dx;
	else if (dy != 0 && !is_wall(map[enemy->y + dy][enemy->x]))
		enemy->y += dy;
}
/*
 * Place the fruit randomly on the map
 * map   : the 2D map array
 * width : map width
 * height: map height
 * fruit : pointer to the fruit entity
 */
void place_fruit(char map[MAX_HEIGHT][MAX_WIDTH], int width, int height, Entity *fruit)
{
	int x, y;
	int ok;
	srand(time(NULL));
	do
	{
		x = rand() % (width - 2) + 1;
		y = rand() % (height - 2) + 1;
		ok = !is_wall(map[y][x]);
	} while (!ok);
	fruit->x = x;
	fruit->y = y;
}
