#include <array>

#include "maze.h"

Cell::Cell() {
    this->x = 10;
    this->y = 10;
    this->walls = {true, true, true, true};
    this->visited =  false;
    this->neighbors = {};
}

Cell::Cell(int rows, int cols) {
    this->x = rows;
    this->y = cols;
    this->walls = {true, true, true, true};
    this->visited =  false;
    this->neighbors = {};
}

int Cell::getRow() { return this->x; }

int Cell::getColumn() { return this->y;}

bool Cell::getVisited() { return this->visited;}

void Cell::findNeighbors(const std::vector<Cell>& grid, int columns) {
    if (this->x > 0) {
        this->neighbors.push_back(grid[(this->x - 1) * columns + this->y]);  // Top
    }
    if (this->y < columns - 1) {
        this->neighbors.push_back(grid[this->x * columns + (this->y + 1)]);  // Right
    }
    if (this->x < grid.size() / columns - 1) {
        this->neighbors.push_back(grid[(this->x + 1) * columns + this->y]);  // Bottom
    }
    if (this->y > 0) {
        this->neighbors.push_back(grid[this->x * columns + (this->y - 1)]);  // Left
    }
}

