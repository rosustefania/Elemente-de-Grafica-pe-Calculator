#pragma once

#include <string>
#include <stdbool.h>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include <set>

using namespace std;

typedef struct Cell {
	pair<int, int> parent;
	int f, g, h;
} cell;

class Maze
{
public:
	Maze(int size);
	~Maze();

	int** getGrid();
	void generateMaze(int size);
	void Prims(int size);
	bool verifyMaze(int size);
	bool aStarSearch(int size, int startX, int startZ, int endX, int endZ);
	bool pathExits(int size, int startX, int startZ);


protected:
	int** grid;
};