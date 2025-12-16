#ifndef MAZE_H
#define MAZE_H

#include <vector>

using namespace std;

class Maze {
public:
    int dimension;
    vector<vector<bool>> walls;
    
    Maze(int dim);
    void generate();
    bool isWall(int x, int y) const;
    
private:
    void generateSquareMaze();
    void carvePassagesFrom(int cx, int cy, vector<vector<bool>>& grid);
};

#endif
