#pragma once

#include <vector>
#include <string>

enum class Cell {
	empty,
	block,
	cross,
	circle,
};

enum class Team {
	ai,
	player,
	none,
};


struct Rect {
	short x0;
	short y0;
	short x1;
	short y1;

	bool contains(short x, short y) {
		return x0 <= x && x <= x1 && y0 <= y && y <= y1 && x1 - x0 > 0 && y1 - y0 > 0;
	}
};

struct EmptyCell {
	size_t x, y;
};


struct Move {
	size_t x, y;
	int score;
};

typedef std::vector<std::string> Text;
typedef std::vector<std::vector<Cell>> Map;
typedef std::vector<std::vector<Rect>> Points;
typedef std::vector<EmptyCell> EmptyCells;






static char cellsCharacters[4][2] = { " ", "@", "X", "O" };


static Map defaultMap3x3 = {
	{Cell::block, Cell::block, Cell::block, Cell::block, Cell::block},
	{Cell::block, Cell::empty, Cell::empty, Cell::empty, Cell::block},
	{Cell::block, Cell::empty, Cell::empty, Cell::empty, Cell::block},
	{Cell::block, Cell::empty, Cell::empty, Cell::empty, Cell::block},
	{Cell::block, Cell::block, Cell::block, Cell::block, Cell::block},
};


static Points defaultPoints3x3 = {
	{{0,0,0,0},		{0,0,0,0},		{0,0,0,0},		{0,0,0,0},		{0,0,0,0}},
	{{0,0,0,0},		{5,5,7,7},		{9,5,11,7},		{13,5,15,7},	{0,0,0,0}},
	{{0,0,0,0},		{5,9,7,11},		{9,9,11,11},	{13,9,15,11},	{0,0,0,0}},
	{{0,0,0,0},		{5,13,7,15},	{9,13,11,15},	{13,13,15,15},	{0,0,0,0}},
	{{0,0,0,0},		{0,0,0,0},		{0,0,0,0},		{0,0,0,0},		{0,0,0,0}},
};

Text getTextFromMap(Map map);

Team nextTeam(Team current);

Team procSelect(Team currentTeam, short pX, short pY, Points& points, Map& map);

EmptyCells getEmptyCells(Map& map);

Team getTeamFromCell(Cell& cell);

bool winning(Map& map, Team team);

Move minimax(Map map, Team team);