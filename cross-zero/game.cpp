#include "game.h"

Text getTextFromMap(Map map)
{
	Text resultText;
	std::string startStr, endStr, middleStr;

	// ==========================================
	startStr.clear();
	middleStr.clear();

	middleStr += char(201);
	startStr += char(186);

	for (size_t x = 0; x < map[0].size() - 1; ++x) {
		middleStr += char(205);
		middleStr += char(205);
		middleStr += char(205);
		middleStr += char(203);
		startStr += "   ";
		startStr += char(186);
	}

	middleStr += char(205);
	middleStr += char(205);
	middleStr += char(205);
	middleStr += char(187);
	startStr += "   ";
	startStr += char(186);

	resultText.push_back(middleStr);

	// ==========================================
	for (size_t y = 0; y < map.size(); ++y) {
		middleStr.clear();
		endStr.clear();

		if (y != map.size() - 1) {
			endStr += char(204);
		}
		else {
			endStr += char(200);
		}
		middleStr += char(186);

		for (size_t x = 0; x < map[y].size(); ++x) {

			middleStr += " " + std::string(cellsCharacters[(int)map[y][x]]) + " " + char(186);
			if (y != map.size() - 1) {
				if (x != map[y].size() - 1) {
					endStr += char(205);
					endStr += char(205);
					endStr += char(205);
					endStr += char(206);
				}
				else {
					endStr += char(205);
					endStr += char(205);
					endStr += char(205);
					endStr += char(185);

				}
			}
			else {
				if (x != map[y].size() - 1) {
					endStr += char(205);
					endStr += char(205);
					endStr += char(205);
					endStr += char(202);
				}
				else {
					endStr += char(205);
					endStr += char(205);
					endStr += char(205);
					endStr += char(188);
				}
			}
		}
		resultText.push_back(startStr);
		resultText.push_back(middleStr);
		resultText.push_back(startStr);
		resultText.push_back(endStr);

	}



	return resultText;
}


Team nextTeam(Team current)
{
	switch (current) {
	case Team::ai: return Team::player;
	case Team::player: return Team::ai;
	}
}


Team procSelect(Team currentTeam, short pX, short pY, Points& points, Map& map)
{
	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[y].size(); ++x) {
			if (points[y][x].contains(pX, pY)) {
				if (map[y][x] == Cell::empty) {
					switch (currentTeam) {
					case Team::ai: map[y][x] = Cell::cross; break;
					case Team::player: map[y][x] = Cell::circle; break;
					}
					return nextTeam(currentTeam);
				}
			}
		}
	}
	return currentTeam;

}

EmptyCells getEmptyCells(Map& map)
{
	EmptyCells cells;

	for (size_t y = 0; y < map.size(); ++y) {
		for (size_t x = 0; x < map[y].size(); ++x) {
			if (map[y][x] == Cell::empty) {
				cells.push_back({ x, y });
			}
		}
	}
	return cells;
}

Team getTeamFromCell(Cell& cell)
{
	switch (cell)
	{
	case Cell::circle: return Team::player;
	case Cell::cross: return Team::ai;
	}
}

bool winning(Map& map, Team team)
{
	size_t mY = map.size();
	
	for (size_t y = 0; y < mY; ++y) {
		size_t mX = map[y].size();
		
		for (size_t x = 0; x < mX; ++x) {
			Cell type = map[y][x];
			if (team != getTeamFromCell(type)) {
				continue;
			}

			if (type != Cell::empty && type != Cell::block) {
				size_t tx = x, ty = y;
				int count = 0;

				// step 1 (row)
				for (tx = x + 1; tx < mX; ++tx) {
					if (map[y][tx] == type
						&& map[y][tx] != Cell::empty
						&& map[y][tx] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}
				for (tx = x; tx >= 0 && tx < mX; --tx) {
					if (map[y][tx] == type
						&& map[y][tx] != Cell::empty
						&& map[y][tx] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}

				if (count == 3) {
					return true;
				}

				// step 2 (column)
				count = 0;
				for (ty = y + 1; ty < mY; ++ty) {
					if (map[ty][x] == type
						&& map[ty][x] != Cell::empty
						&& map[ty][x] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}
				for (ty = y; ty >= 0 && ty < mY; --ty) {
					if (map[ty][x] == type
						&& map[ty][x] != Cell::empty
						&& map[ty][x] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}
				if (count == 3) {
					return true;
				}

				// step 3
				count = 0;
				for (tx = x + 1, ty = y + 1; tx < mX && ty < mY; ++tx, ++ty) {
					if (map[ty][tx] == type
						&& map[ty][tx] != Cell::empty
						&& map[ty][tx] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}
				for (tx = x, ty = y; tx >= 0 && ty >= 0 && tx < mX && ty < mY; --tx, --ty) {
					if (map[ty][tx] == type
						&& map[ty][tx] != Cell::empty
						&& map[ty][tx] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}

				if (count == 3) {
					return true;
				}


				// step 4
				count = 0;
				for (tx = x + 1, ty = y; tx < mX && ty >= 0 && ty < mY; ++tx, --ty) {
					if (map[ty][tx] == type
						&& map[ty][tx] != Cell::empty
						&& map[ty][tx] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}
				for (tx = x, ty = y + 1; tx >= 0 && ty < mY; --tx, ++ty) {
					if (map[ty][tx] == type
						&& map[ty][tx] != Cell::empty
						&& map[ty][tx] != Cell::block) {
						++count;
					}
					else {
						break;
					}
				}
				if (count == 3) {
					return true;
				}
			}
		}
	}
	return false;
}

Move minimax(Map map, Team team)
{
	EmptyCells availableMoves = getEmptyCells(map);

	if (winning(map, Team::ai)) {
		return { 0,0,10 };
	}
	else if (winning(map, Team::player)) {
		return { 0,0,-10 };
	}
	else if (availableMoves.size() == 0) {
		return { 0,0,0 };
	}

	std::vector<Move> moves;

	for (size_t i = 0; i < availableMoves.size(); ++i) {
		Move move;
		move.x = availableMoves[i].x;
		move.y = availableMoves[i].y;

		Move result;
		switch (team) {
		case Team::ai: 
			map[move.y][move.x] = Cell::cross; 
			result = minimax(map, Team::player);
			move.score = result.score;

			break;
		case Team::player: 
			map[move.y][move.x] = Cell::circle; 
			result = minimax(map, Team::ai);
			move.score = result.score;

			break;
		}

		map[move.y][move.x] = Cell::empty;

		moves.push_back(move);
	}


	size_t bestMove = 0;
	int bestScore = -10000;
	switch (team) {
	case Team::ai:
		
		for (size_t i = 0; i < moves.size(); ++i) {
			if (moves[i].score > bestScore) {
				bestScore = moves[i].score;
				bestMove = i;
			}
		}

		break;
	case Team::player:
		bestScore = 10000;

		for (size_t i = 0; i < moves.size(); ++i) {
			if (moves[i].score < bestScore) {
				bestScore = moves[i].score;
				bestMove = i;
			}
		}

		break;
	}


	return moves[bestMove];
}
