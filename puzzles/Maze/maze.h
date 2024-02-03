#pragma once

#include <array>
#include <vector>

class Cell {
    public:
        int x, y;
        std::array<bool, 4> walls;
        bool visited;
        std::vector<Cell> neighbors;

        Cell();
        Cell(int rows, int cols);
        int getRow();
        int getColumn();
        bool getVisited();
        void findNeighbors(const std::vector<Cell> &grid);
};
