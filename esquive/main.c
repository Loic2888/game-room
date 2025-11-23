#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "game.h"

#define ENEMY_COUNT 2
/*
 * main.c - Simple Dodge Game
 *
 * This is the main file for a small terminal-based game where the player
 * must avoid enemies and collect fruit. The player moves using the keys:
 *   z - up
 *   s - down
 *   q - left
 *   d - right
 * Press 'e' to quit the game.
 *
 * Features:
 * - Two enemies that follow the player, moving one step at a time and
 *   cannot pass through walls.
 * - Randomly spawning fruit that repop after being collected.
 * - Score tracking displayed on the terminal.
 * - Walls and map layout defined in a 2D array.
 *
 * This file handles:
 * - Game initialization (player, enemies, fruit, map)
 * - Main game loop
 * - Input handling
 * - Collision detection with enemies and fruit
 * - Updating and printing the game map
 *
 * Compatible with C89 / Betty style and Linux terminals.
 */
int main(void)
{
	int width = 20;
	int height = 7;
	int score;
	char input;
	int e;
	Entity player;
	Entity enemies[ENEMY_COUNT];
	Entity fruit;
	char map[MAX_HEIGHT][MAX_WIDTH] = {
		"####################",
		"#.............#....#",
		"#..##..##...#......#",
		"#..#..........#....#",
		"#....###.#......#..#",
		"#..........#.......#",
		"####################"};
	srand(time(NULL));
	player.x = 1;
	player.y = 1;
	enemies[0].x = 18;
	enemies[0].y = 5;
	enemies[1].x = 18;
	enemies[1].y = 1;
	score = 0;

	place_fruit(map, width, height, &fruit);
	while (1)
	{
		system("clear");
		print_map(map, width, height, &player, enemies, ENEMY_COUNT, &fruit);
		printf("\nScore : %d\n", score);
		/* Collision fruit */
		if (player.x == fruit.x && player.y == fruit.y)
		{
			score++;
			place_fruit(map, width, height, &fruit);
		}
		/* Collision ennemis */
		for (e = 0; e < ENEMY_COUNT; e++)
		{
			if (player.x == enemies[e].x && player.y == enemies[e].y)
			{
				printf("\n💀 Game Over ! L'ennemi vous a attrapé.\n");
				return 0;
			}
		}
		printf("Déplacez-vous (z/q/s/d, e pour quitter) : ");
		scanf(" %c", &input);
		if (input == 'e')
		{
			printf("\nVous quittez le jeu.\n");
			return 0;
		}
		move_player(input, map, width, height, &player);
		/* Déplacement des ennemis */
		for (e = 0; e < ENEMY_COUNT; e++)
			move_enemy(map, &player, &enemies[e]);

		usleep(150000);
	}
	return 0;
}
