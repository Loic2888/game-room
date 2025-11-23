# Esquive

A simple terminal-based game where you must eat fruits while avoiding enemies.

## Overview

Esquive is a Linux terminal game featuring:

- **Player** - Character controlled by the player
- **2 Enemies** - Dynamic enemies that move around the game area
- **Fruit** - Objective to collect and eat
- **Terminal Display** - Real-time game board visualization

## Objective

Collect the fruit (`*`) while avoiding the two enemies (`E`). The game ends if you touch an enemy.

## Controls

- **Q** - Move up
- **Z** - Move down
- **S** - Move right
- **D** - Move left

## How to Play

1. The player character will spawn on the game board
2. Navigate using QZSD keys to reach the fruit (`*`)
3. Avoid colliding with enemies (`E`)
4. Eat the fruit to win or get caught by an enemy to lose

## Getting Started

### Prerequisites

- Linux terminal
- GCC compiler (to compile from source)

### Installation

Navigate to the esquive directory
cd esquive

Compile the game (if needed)
gcc -o game game.c main.c

Run the game
./game


## Project Structure

- `game` - Compiled executable
- `game.c` - Main game logic
- `game.h` - Game header file
- `main.c` - Entry point
- `README.md` - This file


## Technical Details

- **Language** - C
- **Platform** - Linux Terminal
- **Build** - GCC

## Author

Developed as a personal learning project.

---

*Last Updated: November 2025*
