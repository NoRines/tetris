#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include <SDL2/SDL.h>
#include "windefs.h"
#include "board.h"

constexpr int blockWidth = playAreaWidth / boardColumns;
constexpr int blockHeight = playAreaHeight / numVisibleBoardRows;
constexpr int rowOffset = numVisibleBoardRows - boardRows;

using byte = uint8_t;

struct Color
{
	byte r, g, b;
};

struct Block
{
	Color c;
	Pos p;
};

void drawBlock(const Block& b, SDL_Renderer* renderer);
bool canMoveBlockLeft(const Block& b);
bool canMoveBlockRight(const Block& b);
bool canMoveBlockDown(const Block& b);

#endif
