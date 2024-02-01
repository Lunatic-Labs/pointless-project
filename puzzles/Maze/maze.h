#include <vector>

class Cell {
    int coords[2]; //pointer to x and y
    bool walls[4];
    bool visited;
    std::vector<Cell> neighbors;
    public:   
        Cell::Cell(int x, int y);
        bool isVisited(const Cell &cell);
        //return coordinates
        int *getCoord(const Cell &cell);
        //return neighbors vector
        std::vector<Cell> getNeighbors(const Cell &cell);
        void findNeighbors(Cell &cell, const Grid &grid);
};

class Grid {
    Cell *grid;

    public:
        Grid::Grid(int rows, int cols);
};
