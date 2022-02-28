#include <lab_m1/tema2/maze.h>
#include "components/simple_scene.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib> 
#include <ctime> 

#define MAX 50

using namespace std;

Maze::Maze(int size)
{
    grid = (int**)calloc(size, sizeof(int*));
    for (int i = 0; i < size; i++) {
        grid[i] = (int*)calloc(size, sizeof(int));
    }
}

int** Maze::getGrid()
{
    return grid;
}

bool Maze::verifyMaze(int size) 
{
    int i1 = 0, i2 = size - 1;
    int j1 = 0, j2 = size - 1;
    for (int t = 0; t < size; t++) {
        if (grid[i1][t] == 0 || grid[i2][t] == 0 ||
            grid[t][j1] == 0 || grid[t][j2] == 0) {

            return true;
        }
    }

    return false;
}

void Maze::generateMaze(int size) 
{
    Prims(size);
    while (!verifyMaze(size)) {
        Prims(size);
    }
}

void Maze::Prims(int size)
{
    vector<vector<int>> frontiers;

    int x = rand() % size;
    int y = rand() % size;
    vector<int> tmp{x, y, x, y};
    frontiers.push_back(tmp);
    tmp.clear();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            grid[i][j] = 1;
        }
    }

    while (frontiers.size() != 0) {
        vector<int> aux;

        int randomCell = rand() % frontiers.size();
        tmp = frontiers.at(randomCell);
       
        frontiers.erase(frontiers.begin() + randomCell);

        if (grid[tmp.at(2)][tmp.at(3)] == 1) {
            grid[tmp.at(0)][tmp.at(1)] = 0;
            grid[tmp.at(2)][tmp.at(3)] = 0;


            if (tmp.at(2) >= 2 && grid[tmp.at(2) - 2][tmp.at(3)] == 1) {
                aux.push_back(tmp.at(2) - 1);
                aux.push_back(tmp.at(3));
                aux.push_back(tmp.at(2) - 2);
                aux.push_back(tmp.at(3));

                frontiers.push_back(aux);
                aux.clear();
            }


            if (tmp.at(3) >= 2 && grid[tmp.at(2)][tmp.at(3) - 2] == 1) {
                aux.push_back(tmp.at(2));
                aux.push_back(tmp.at(3) - 1);
                aux.push_back(tmp.at(2));
                aux.push_back(tmp.at(3) - 2);

                frontiers.push_back(aux);
                aux.clear();
            }


            if (tmp.at(2) < size - 2 && grid[tmp.at(2) + 2][tmp.at(3)] == 1) {
                aux.push_back(tmp.at(2) + 1);
                aux.push_back(tmp.at(3));
                aux.push_back(tmp.at(2) + 2);
                aux.push_back(tmp.at(3));

                frontiers.push_back(aux);
                aux.clear();
            }

            if (tmp.at(3) < size - 2 && grid[tmp.at(2)][tmp.at(3) + 2] == 1) {
                aux.push_back(tmp.at(2));
                aux.push_back(tmp.at(3) + 1);
                aux.push_back(tmp.at(2));
                aux.push_back(tmp.at(3) + 2);

                frontiers.push_back(aux);
                aux.clear();
            }

            tmp.clear();
        }

    }
}


double heuristic(int row, int col, int destX, int destZ)
{
    return ((double)sqrt((row - destX) * (row - destX) + (col - destZ) * (col - destZ)));
}

bool Maze::aStarSearch(int size, int startX, int startZ, int endX, int endZ)
{
    // verifies if source is a valid cell
    if (startX < 0 || startX >= size || startZ < 0 || startX >= size) {
        return false;
    }

    // verifies if the source or the destination is a wall
    if (grid[startX][startZ] == 1 || grid[endX][endZ] == 1) {
        return false;
    }

    // verifies if source and destionation are the same cell
    if (startX == endX && startZ == endZ) {
        return true;
    }

    bool** closedList = (bool**)calloc(size, sizeof(bool*));
    for (int i = 0; i < size; i++) {
        closedList[i] = (bool*)calloc(size, sizeof(bool));
    }
    // no cell has benn included yet in the closed list
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            closedList[i][j] = false;
        }
    }

    Cell** cells = (Cell**)calloc(size, sizeof(Cell*));
    for (int i = 0; i < size; i++) {
        cells[i] = (Cell*)calloc(size, sizeof(Cell));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == startX && j == startZ) {
                cells[i][j].f = cells[i][j].g = cells[i][j].h = 0;
                cells[i][j].parent = make_pair(i, j);
            }
            else {
                cells[i][j].f = cells[i][j].g = cells[i][j].h = MAX;
                cells[i][j].parent = make_pair(-1, -1);
            }
        }
    }

    set<pair<int, pair<int, int>>> openList;
    openList.insert(make_pair(0, make_pair(startX, startZ)));


    while (!openList.empty()) {
        pair<int, pair<int, int>> newPair = *openList.begin();

        openList.erase(openList.begin());

        closedList[newPair.second.first][newPair.second.second] = true;

        if (newPair.second.first - 1 >= 0 && newPair.second.first - 1 < size && newPair.second.second >= 0 &&
            newPair.second.second < size) {
            if (newPair.second.first - 1 == endX && newPair.second.second == endZ) {
                return true;
            }
            else if (closedList[newPair.second.first - 1][newPair.second.second] == false &&
                cells[newPair.second.first - 1][newPair.second.second].f >
                cells[newPair.second.first][newPair.second.second].g + 1.0 +
                heuristic(newPair.second.first - 1, newPair.second.second, endX, endZ)) {

                openList.insert(make_pair(cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first - 1, newPair.second.second, endX, endZ),
                    make_pair(newPair.second.first - 1, newPair.second.second)));

                cells[newPair.second.first - 1][newPair.second.second].f =
                    cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first - 1, newPair.second.second, endX, endZ);
                cells[newPair.second.first - 1][newPair.second.second].g =
                    cells[newPair.second.first][newPair.second.second].g + 1.0;
                cells[newPair.second.first - 1][newPair.second.second].h =
                    heuristic(newPair.second.first - 1, newPair.second.second, endX, endZ);;
                cells[newPair.second.first - 1][newPair.second.second].parent =
                    make_pair(newPair.second.first, newPair.second.second);
            }
        }

        if (newPair.second.first + 1 >= 0 && newPair.second.first + 1 < size && newPair.second.second >= 0 &&
            newPair.second.second < size) {
            if (newPair.second.first + 1 == endX && newPair.second.second == endZ) {
                return true;
            }
            else if (closedList[newPair.second.first + 1][newPair.second.second] == false &&
                cells[newPair.second.first + 1][newPair.second.second].f >
                cells[newPair.second.first][newPair.second.second].g + 1.0 +
                heuristic(newPair.second.first + 1, newPair.second.second, endX, endZ)) {

                openList.insert(make_pair(cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first + 1, newPair.second.second, endX, endZ),
                    make_pair(newPair.second.first + 1, newPair.second.second)));

                cells[newPair.second.first + 1][newPair.second.second].f =
                    cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first + 1, newPair.second.second, endX, endZ);
                cells[newPair.second.first + 1][newPair.second.second].g =
                    cells[newPair.second.first][newPair.second.second].g + 1.0;
                cells[newPair.second.first + 1][newPair.second.second].h =
                    heuristic(newPair.second.first + 1, newPair.second.second, endX, endZ);;
                cells[newPair.second.first + 1][newPair.second.second].parent =
                    make_pair(newPair.second.first, newPair.second.second);
            }
        }

        if (newPair.second.first >= 0 && newPair.second.first < size && newPair.second.second - 1 >= 0 &&
            newPair.second.second - 1 < size) {
            if (newPair.second.first == endX && newPair.second.second - 1 == endZ) {
                return true;
            }
            else if (closedList[newPair.second.first][newPair.second.second - 1] == false &&
                cells[newPair.second.first][newPair.second.second - 1].f >
                cells[newPair.second.first][newPair.second.second].g + 1.0 +
                heuristic(newPair.second.first, newPair.second.second - 1, endX, endZ)) {

                openList.insert(make_pair(cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first, newPair.second.second - 1, endX, endZ),
                    make_pair(newPair.second.first, newPair.second.second - 1)));

                cells[newPair.second.first][newPair.second.second - 1].f =
                    cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first, newPair.second.second - 1, endX, endZ);
                cells[newPair.second.first][newPair.second.second - 1].g =
                    cells[newPair.second.first][newPair.second.second].g + 1.0;
                cells[newPair.second.first][newPair.second.second - 1].h =
                    heuristic(newPair.second.first, newPair.second.second - 1, endX, endZ);;
                cells[newPair.second.first][newPair.second.second - 1].parent =
                    make_pair(newPair.second.first, newPair.second.second);
            }
        }

        if (newPair.second.first >= 0 && newPair.second.first < size && newPair.second.second + 1 >= 0 &&
            newPair.second.second + 1 < size) {
            if (newPair.second.first == endX && newPair.second.second + 1 == endZ) {
                return true;
            }
            else if (closedList[newPair.second.first][newPair.second.second + 1] == false &&
                cells[newPair.second.first][newPair.second.second + 1].f >
                cells[newPair.second.first][newPair.second.second].g + 1.0 +
                heuristic(newPair.second.first, newPair.second.second + 1, endX, endZ)) {

                openList.insert(make_pair(cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first, newPair.second.second + 1, endX, endZ),
                    make_pair(newPair.second.first, newPair.second.second + 1)));

                cells[newPair.second.first][newPair.second.second + 1].f =
                    cells[newPair.second.first][newPair.second.second].g + 1.0 +
                    heuristic(newPair.second.first, newPair.second.second + 1, endX, endZ);
                cells[newPair.second.first][newPair.second.second + 1].g =
                    cells[newPair.second.first][newPair.second.second].g + 1.0;
                cells[newPair.second.first][newPair.second.second + 1].h =
                    heuristic(newPair.second.first, newPair.second.second + 1, endX, endZ);;
                cells[newPair.second.first][newPair.second.second + 1].parent =
                    make_pair(newPair.second.first, newPair.second.second);
            }
        }
    }
    return false;

}

bool Maze :: pathExits(int size, int startX, int startZ)
{   
    int i1 = 0, i2 = size - 1;
    int j1 = 0, j2 = size - 1;
    for (int t = 0; t < size; t++) {
        if (aStarSearch(size, startX, startZ, i1, t) == true ||
            aStarSearch(size, startX, startZ, i2, t) == true ||
            aStarSearch(size, startX, startZ, t, j1) == true ||
            aStarSearch(size, startX, startZ, t, j2) == true) {
            
            return true;
        }
    }
    return false;

}
