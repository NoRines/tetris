#include "shape.h"

static ShapeRotation shapeRotations[] = {
	{ 1, 0, 0, {{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} }} },
	{ 2, 1,-1, {{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} }} },
	{ 4, 0,-1, {{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} }} },
	{ 4, 0,-1, {{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} }} },
	{ 2, 0,-1, {{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} }} },
	{ 2, 1,-1, {{ {-1, 0}, { 0,-1}, { 0, 0}, { 0, 0} }} },
	{ 4, 0,-1, {{ { 0, 0}, { 0, 0}, { 0, 0}, { 0, 0} }} }
};


static byte shapes[] = {
	0, 0, 0, 0,
	0, 1, 1, 0,
	0, 1, 1, 0,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 0, 0, 0,
	2, 2, 2, 2,
	0, 0, 0, 0,

	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 3, 3, 3,
	0, 0, 0, 3,

	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 4, 4, 4,
	0, 4, 0, 0,

	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 5, 5,
	0, 5, 5, 0,

	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 6, 6, 0,
	0, 0, 6, 6,

	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 7, 7, 7,
	0, 0, 7, 0
};

const Color shapeColors[] = {
	{0xFF, 0x00, 0x00},
	{0x00, 0xFF, 0x00},
	{0x00, 0x00, 0xFF},
	{0xDD, 0xDD, 0x00},
	{0x00, 0xFF, 0xFF},
	{0xFF, 0x00, 0xFF},
	{0xAA, 0x10, 0x80}
};


static std::array<Block, 4> getBlocks(int index)
{
	std::array<Block, 4> blocks;
	int i = 0;

	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			int shapeValue = shapes[index + x + (4 * y)];
			if(shapeValue > 0 && i < 4)
				blocks[i++] = { shapeColors[shapeValue-1], x - 2, y - 2 };
		}
	}
	return blocks;
}

static Shape getShape(int blockType, int x, int y)
{
	Shape shape;

	shape.blocks = getBlocks(blockType * 16);
	shape.type = blockType;

	for(auto& b : shape.blocks)
	{
		b.p.x += x;
		b.p.y += y + shapeRotations[shape.type].startYOffset;
	}

	shape.pos.x = x;
	shape.pos.y = y + shapeRotations[shape.type].startYOffset;
	shape.rotIndex = shapeRotations[shape.type].startIndex;

	return shape;
}

Shape createShape(std::mt19937& generator, int x, int y, int lastType)
{
	std::uniform_int_distribution<> dis1(0, 7);
	std::uniform_int_distribution<> dis2(0, 6);

	int type = dis1(generator);
	if(type == lastType || type == 7)
		type = dis2(generator);

	return getShape(type, x, y);
}

Shape createShape(int blockType, int x, int y)
{
	return getShape(blockType, x, y);
}

void drawShape(const Shape& s, SDL_Renderer* renderer)
{
	for(auto& b : s.blocks)
		drawBlock(b, renderer);
}

template<typename T, typename F>
static bool moveShape(Shape& s, T check, F op)
{
	for(auto& b : s.blocks)
		if(!check(b))
			return false;

	for(auto& b : s.blocks)
		op(b.p);
	op(s.pos);

	return true;
}

bool moveShapeLeft(Shape& s)
{
	return moveShape(s, [](Block& b){ return canMoveBlockLeft(b); }, [](Pos& p){ p.x--; });
}

bool moveShapeRight(Shape& s)
{
	return moveShape(s, [](Block& b){ return canMoveBlockRight(b); }, [](Pos& p){ p.x++; });
}

bool moveShapeDown(Shape& s)
{
	return moveShape(s, [](Block& b){ return canMoveBlockDown(b); }, [](Pos& p){ p.y++; });
}

void placeShape(const Shape& s)
{
	for(auto& b : s.blocks)
		fillCell(s.type, b.p.x, b.p.y);
}

bool rotateShape(Shape& s, int dir)
{
	ShapeRotation rotInfo = shapeRotations[s.type];

	auto doRotation = [&s, &rotInfo](Block& b, int dir)
	{
		b.p.x += rotInfo.preRotOps[s.rotIndex].x;
		b.p.y += rotInfo.preRotOps[s.rotIndex].y;

		b.p.x -= s.pos.x;
		b.p.y -= s.pos.y;

		int tmp = b.p.x;
		b.p.x = dir * b.p.y;
		b.p.y = (-dir) * tmp;

		b.p.x += s.pos.x;
		b.p.y += s.pos.y;
	};


	if(rotInfo.numRotStates == 1)
		return true;

	std::array<Block, 4> tmpBlocks = s.blocks;

	if(rotInfo.numRotStates == 2)
		dir = s.rotIndex == 0 ? 1 : -1;

	for(auto& b : tmpBlocks)
	{
		doRotation(b, dir);

		if(b.p.y < 0)
			continue;
		if(!inBoard(b.p.x, b.p.y) || isCellFilled(b.p.x, b.p.y))
			return false;
	}
	s.blocks = tmpBlocks;
	s.rotIndex = (s.rotIndex + 1) % rotInfo.numRotStates;

	return true;
}
