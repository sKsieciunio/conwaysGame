#include <raylib.h>

constexpr int screenWidth = 800;
constexpr int screenHeight = 800;
constexpr int cellSize = 3;
constexpr int fps = 60;
constexpr int gridWidth = screenWidth / cellSize;
constexpr int gridHeight = screenHeight / cellSize;

typedef struct Cell {
    Vector2 position;  
    bool isAlive;
    bool nextTick; // describes status of cell in next iteration

    void setPosition(int x, int y) {
        position.x = x;
        position.y = y;
    }
} Cell;

int numberOfAliveNeighbours(Cell (&matrix)[gridWidth][gridHeight], int x, int y) {
    int result{ matrix[x][y].isAlive ? -1 : 0 };

    for (int i = ((x > 0) ? (x - 1) : (0)); i <= ((x < gridWidth - 1) ? (x + 1) : x); ++i) {
        for (int j = ((y > 0) ? (y - 1) : (0)); j <= ((y < gridHeight - 1) ? (y + 1) : y); ++j) {
            if (matrix[i][j].isAlive == true) {
                ++result;
            }
        }
    }

    return result;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Conway's game of life");
    SetTargetFPS(fps);

    Cell cellMatrix[gridWidth][gridHeight]{ 0 };

    // Initialaizing position of each cell
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            cellMatrix[i][j].setPosition(i, j);
        }
    }

    for (int i = 0; i < gridWidth * gridHeight / 2; ++i)
        cellMatrix[GetRandomValue(0, gridWidth - 1)][GetRandomValue(0, gridHeight - 1)].isAlive = true;

    Color gridColor{ 220, 220, 220, 255 };

    while (!WindowShouldClose()) {
        for (int i = 0; i < gridWidth; ++i) {
            for (int j = 0; j < gridHeight; ++j) {
                int neighbours{ numberOfAliveNeighbours(cellMatrix, i, j) };
                bool isAlive = cellMatrix[i][j].isAlive;

                if (isAlive && neighbours < 2)
                    cellMatrix[i][j].nextTick = false;
                else if (isAlive && neighbours > 3)
                    cellMatrix[i][j].nextTick = false;
                else if (isAlive)
                    cellMatrix[i][j].nextTick = true;
                else if ((!isAlive) && neighbours == 3)     // ERROR PRONE 
                    cellMatrix[i][j].nextTick = true;
                else
                    cellMatrix[i][j].nextTick = isAlive;
            }
        }

        for (int i = 0; i < gridWidth; ++i) {
            for (int j = 0; j < gridHeight; ++j) {
                cellMatrix[i][j].isAlive = cellMatrix[i][j].nextTick;
            }
        }


        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            // drawing grid lines
            for (int i = 0; i < gridWidth + 1; ++i) {
                DrawLine(i * cellSize, 0, i * cellSize, screenHeight, gridColor);
            }
            for (int i = 0; i < gridHeight + 1; ++i) {
                DrawLine(0, i * cellSize, screenWidth, i * cellSize, gridColor);
            }

            // drawing cells
            for (int i = 0; i < gridWidth; ++i) {
                for (int j = 0; j < gridHeight; ++j) {
                    if (cellMatrix[i][j].isAlive == true) {
                        DrawRectangle(cellMatrix[i][j].position.x * cellSize - 1,
                                      cellMatrix[i][j].position.y * cellSize,
                                      cellSize + 1, cellSize + 1, BLACK);
                    }
                }
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
