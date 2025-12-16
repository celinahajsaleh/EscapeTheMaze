#include "maze.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

Maze::Maze(int dim) : dimension(dim) {
    walls.resize(dimension, vector<bool>(dimension, true));
}

void Maze::generate() {
    generateSquareMaze();
}

bool Maze::isWall(int x, int y) const {
    if (x < 0 || x >= dimension || y < 0 || y >= dimension)
        return true;
    return walls[y][x];
}

void Maze::generateSquareMaze() {
    for (int y = 0; y < dimension; y++) {
        for (int x = 0; x < dimension; x++) {
            walls[y][x] = true;
        }
    }
    
    carvePassagesFrom(1, 1, walls);
    
    walls[1][1] = false;
    walls[dimension - 2][dimension - 1] = false;
}

void Maze::carvePassagesFrom(int cx, int cy, vector<vector<bool>>& grid) {
    int directions[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
    
    static random_device rd;
    static mt19937 g(rd());
    shuffle(begin(directions), end(directions), g);
    
    grid[cy][cx] = false;
    
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int nx = cx + dx * 2;
        int ny = cy + dy * 2;
        
        if (nx > 0 && nx < dimension - 1 && ny > 0 && ny < dimension - 1 && grid[ny][nx]) {
            grid[cy + dy][cx + dx] = false;
            carvePassagesFrom(nx, ny, grid);
        }
    }
}
