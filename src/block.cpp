#include "block.h"

void drawBlock(const Block& b, SDL_Renderer* renderer)
{
	int y = b.p.y + rowOffset;
 	SDL_Rect r = { b.p.x * blockWidth, y * blockHeight, blockWidth, blockHeight };
	SDL_SetRenderDrawColor(renderer, b.c.r, b.c.g, b.c.b, 0xFF);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderDrawRect(renderer, &r);
}


template<typename T, typename F>
static bool canMoveBlock(T op1, F op2)
{
	if(op1())
	{
		if(op2())
			return false;
		return true;
	}
	return false;
}

bool canMoveBlockLeft(const Block& b)
{
	return canMoveBlock([&b](){ return b.p.x - 1 >= 0; }, [&b](){ return isCellFilled(b.p.x - 1, b.p.y); });
}

bool canMoveBlockRight(const Block& b)
{
	return canMoveBlock([&b](){ return b.p.x + 1 < boardColumns; }, [&b](){ return isCellFilled(b.p.x + 1, b.p.y); });
}

bool canMoveBlockDown(const Block& b)
{
	return canMoveBlock([&b](){ return b.p.y + 1 < boardRows; }, [&b](){ return isCellFilled(b.p.x, b.p.y + 1); });
}
