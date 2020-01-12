#include "board.h"
#include "shape.h"
#include <array>
#include <cstdint>
#include <algorithm>

using Board = std::array<int8_t, boardRows * boardColumns>;

static Board board;

static int numMarkedLines = 0;
static std::array<int, 4> markedLines;

static int8_t& getCell(int x, int y)
{
	return board[x + y * boardColumns];
}

void initBoard()
{
	std::fill(board.begin(), board.end(), -1);
}

bool isCellFilled(int x, int y)
{
	return getCell(x, y) >= 0;
}

bool inBoard(int x, int y)
{
	return x >= 0 && x < boardColumns && y >= 0 && y < boardRows;
}

void fillCell(int type, int x, int y)
{
	getCell(x, y) = type;
}

void drawBoard(SDL_Renderer* renderer)
{
	int boardValue = 0;
	for(int y = 0; y < boardRows; y++)
	{
		for(int x = 0; x < boardColumns; x++)
		{
			if(isCellFilled(x, y))
			{
				boardValue = getCell(x, y);
				drawBlock({ shapeColors[boardValue], x, y }, renderer);
			}
		}
	}
}

void removeLines()
{
	for(int i = 0; i < numMarkedLines; i++)
	{
		int line = markedLines[i];
		for(int y = line; y >= 0; y--)
		{
			if(y != 0)
				for(int x = 0; x < boardColumns; x++)
					fillCell(getCell(x, y-1), x, y);
			else
				for(int x = 0; x < boardColumns; x++)
					fillCell(-1, x, y);
		}
	}
}

void markLinesForRemoval()
{
	int i = 0;

	for(int y = 0; y < boardRows; y++)
	{
		int numFilledCells = 0;
		for(int x = 0; x < boardColumns; x++)
			if(isCellFilled(x, y))
				numFilledCells++;

		if(numFilledCells == boardColumns)
			markedLines[i++] = y;
	}
	numMarkedLines = i;
}

int getNumLinesToRemove()
{
	return numMarkedLines;
}

static void animateLine(int step, int line)
{
	fillCell(-1, (boardColumns / 2) - step, line);
	fillCell(-1, (boardColumns / 2) + (step - 1), line);
}

void animateRemove(int step)
{
	for(int i = 0; i < numMarkedLines; i++)
		animateLine(step, markedLines[i]);
}
