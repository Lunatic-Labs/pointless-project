#include "maze.h"

//construct cell
Cell::Cell(int x, int y) : walls{true,true,true,true} {
    this->coords[0] = x; //row
    this->coords[1] = y; //col
    this->visited = false;
}

void Cell::findNeighbors(Cell &cell, const Grid &grid) {
    if (cell.coords[0]  > 0) {
        cell.neighbors.push_back(grid[(cell.coords[0] - 1)]);
    }
}

//construct grid of cells
Grid::Grid(int rows, int cols) {
    this->grid = (Cell *)malloc((rows * cols) * sizeof(Cell));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Cell c{i,j};
            grid = &c;
            grid++;
        }
    }
}