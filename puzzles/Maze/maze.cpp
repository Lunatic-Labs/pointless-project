#include <iostream>
#include <fstream>
#include <vector>

#include "maze.h"

//rows and cols will be 10 for now

void makeGrid(std::vector<Cell> &grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid.push_back(Cell(i, j));
    }
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        grid[i * cols + j].findNeighbors(grid, cols);
    }
  }
}

void mazeGenerator(std::vector<Cell>& grid, int rows, int columns) {
    std::vector<Cell> stack;
    Cell current = grid[0];
    stack.push_back(current);

    while (!stack.empty()) {
        current = stack.back();
        stack.pop_back();

        if (!current.visited) {
            current.visited = true;

            std::vector<Cell> unvisitedNeighbors;
            for (const auto& neighbor : current.neighbors) {
                if (!neighbor.visited) {
                    unvisitedNeighbors.push_back(neighbor);
                }
            }

            if (!unvisitedNeighbors.empty()) {
                stack.push_back(current);

                int randomIndex = rand() % unvisitedNeighbors.size();
                Cell next = unvisitedNeighbors[randomIndex];

                int x = current.x - next.x;
                if (x == 1) {
                    current.walls[0] = false;
                    next.walls[2] = false;
                } else if (x == -1) {
                    current.walls[2] = false;
                    next.walls[0] = false;
                }

                int y = current.y - next.y;
                if (y == 1) {
                    current.walls[3] = false;
                    next.walls[1] = false;
                } else if (y == -1) {
                    current.walls[1] = false;
                    next.walls[3] = false;
                }

                grid[current.x * columns + current.y] = current;
                grid[next.x * columns + next.y] = next;
                stack.push_back(next);
            }
        }
    }
}




std::string generateSVG(const std::vector<Cell>& grid, int rows, int columns, float cellSize) {
    std::string svg = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    svg += "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
           "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
    svg += "<svg width=\"" + std::to_string(columns * cellSize) + "\" height=\"" +
           std::to_string(rows * cellSize) + "\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";

    for (const auto& cell : grid) {
        float x = cell.x * cellSize;
        float y = cell.y * cellSize;

        // Draw walls based on the cell's state
        for (int i = 0; i < 4; i++) {
            if (cell.walls[i]) {
                svg += "<line x1=\"" + std::to_string(x) + "\" y1=\"" + std::to_string(y) +
                       "\" x2=\"" + std::to_string(x + cellSize) + "\" y2=\"" + std::to_string(y) +
                       "\" stroke=\"black\" stroke-width=\"1\" />\n";
            }
        }
        

        // Draw the cell itself (optional)
        svg += "<rect x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) +
               "\" width=\"" + std::to_string(cellSize) + "\" height=\"" + std::to_string(cellSize) +
               "\" fill=\"white\" stroke=\"black\" />\n";
    }

    // Write the SVG footer
    svg += "</svg>\n";

    return svg;
}

int main() 
{

    int rows, cols;
    rows = cols = 10;

    std::vector<Cell> grid;
    makeGrid(grid, rows, cols);
    mazeGenerator(grid, rows, cols);



    std::string svg = generateSVG(grid, 10, 10, 40.0);

    // Write HTML to file
    std::ofstream file("maze.svg");
    if (file.is_open()) {
        file << svg;
        file.close();
        std::cout << "Maze successfully saved to maze.svg\n";
    } else {
        std::cerr << "Unable to open file for writing\n";
    }

    return 0;
    //TODO: get grid turned into a maze
}






