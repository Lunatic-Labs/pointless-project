#include <iostream>
#include <fstream>
#include <vector>

#include "maze.h"

//rows and cols will be 10 for now

void makeGrid(std::vector<Cell> &grid) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            grid.push_back(Cell(i, j));
    }
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
        grid[i * 5 + j].findNeighbors(grid);
    }
  }
}

void mazeGenerator(std::vector<Cell>& grid, int rows, int columns) {
    std::vector<Cell> stack;
    Cell current = grid[0];

    grid[current.x * columns + current.y].visited = true;

    if (!current.visited) {
        current.visited = true;
        stack.push_back(current);
    }

    std::vector<Cell> unvisited;
    for (const auto& neighbor : current.neighbors) {
        if (!neighbor.visited) {
            unvisited.push_back(neighbor);
        }
    }

    if (!unvisited.empty()) {
        int random = rand() % unvisited.size();
        Cell next = unvisited[random];

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
        current = next;
    } else if (!stack.empty()) {
        current = stack.back();
        stack.pop_back();
    } else {
        // Handle end of maze generation
        ;
    }
}

std::string maze_to_html(const std::vector<Cell>& grid, int rows, int columns) {
    std::string html = "<html><head><style>table {border-collapse: collapse;}td {width: 20px; height: 20px; border: 1px solid black;}</style></head><body><table>";

    for (int i = 0; i < rows; i++) {
        html += "<tr>";
        for (int j = 0; j < columns; j++) {
            int index = i * columns + j;
            const Cell& cell = grid[index];
            html += cell.walls[0] ? "<td></td>" : "<td style='border-top: none;'></td>";
            html += cell.walls[1] ? "<td></td>" : "<td style='border-right: none;'></td>";
            html += cell.walls[2] ? "<td></td>" : "<td style='border-bottom: none;'></td>";
            html += cell.walls[3] ? "<td></td>" : "<td style='border-left: none;'></td>";
        }
        html += "</tr>";
    }

    html += "</table></body></html>";
    return html;
}

int main() 
{

    std::vector<Cell> grid(25, Cell(0,0));
    makeGrid(grid);
    mazeGenerator(grid, 5, 5);



    std::string html = maze_to_html(grid, 5, 5);

    // Write HTML to file
    std::ofstream file("maze.html");
    if (file.is_open()) {
        file << html;
        file.close();
        std::cout << "Maze successfully saved to maze.html\n";
    } else {
        std::cerr << "Unable to open file for writing\n";
    }

    return 0;
    //TODO: ensure maze builds
    //TODO: get it to print out a visible grid
    //TODO: get grid turned into a maze
}






