#ifndef GAME_H
#define GAME_H

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

/* Structure representing an entity (player, enemy, or fruit) */
typedef struct Entity
{
	int x;
	int y;
} Entity;
int is_wall(char c);
void print_map(char map[MAX_HEIGHT][MAX_WIDTH], int width, int height,
			   Entity *player, Entity enemies[], int enemy_count, Entity *fruit);
void move_player(char input, char map[MAX_HEIGHT][MAX_WIDTH],
				 int width, int height, Entity *player);
void move_enemy(char map[MAX_HEIGHT][MAX_WIDTH], Entity *player, Entity *enemy);
void place_fruit(char map[MAX_HEIGHT][MAX_WIDTH], int width, int height, Entity *fruit);

#endif
