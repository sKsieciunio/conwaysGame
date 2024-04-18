#include <raylib.h>

constexpr int screenWidth{ 800 };
constexpr int screenHeight{ 800 };
constexpr int cellSize{ 10 };
constexpr int fps{ 60 };
// simulations calculations happens every x frames
constexpr int physicsRate{ 5 };
constexpr int gridWidth{ screenWidth / cellSize };
constexpr int gridHeight{ screenHeight / cellSize };

const char* binds{ "[SPACE] pause, [MOUSE] draw/erase, [R] random board, [C] clear board" };
const char* entities{ "ENTITES: [1] glider" };

typedef struct Cell {
    Vector2 position;  
    bool isAlive;
    bool nextTick; // describes status of cell in next iteration

    void setPosition(int x, int y) {
        position.x = x;
        position.y = y;
    }
} Cell;

static int numberOfAliveNeighbours(Cell(&matrix)[gridWidth][gridHeight], int x, int y);
static void drawGlider(Cell(&matrix)[gridWidth][gridHeight], int x, int y);

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

    // randomizing board
    for (int i = 0; i < gridWidth * gridHeight / 2; ++i) {
        cellMatrix[GetRandomValue(0, gridWidth - 1)][GetRandomValue(0, gridHeight - 1)].isAlive = true;
    }

    Color gridColor{ 220, 220, 220, 255 };
    bool isPaused{ true };
    int physicsTick{ 1 };

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            isPaused = !isPaused;
        }

        // running simulation 
        if (!isPaused && (physicsTick % physicsRate == 0)) {
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
                    else if ((!isAlive) && neighbours == 3)
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

            physicsTick = 1;
        }
        else {
            ++physicsTick;
        }

        // handles cell modifications
        if (isPaused) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                int x = static_cast<int>(mousePos.x / cellSize);
                int y = static_cast<int>(mousePos.y / cellSize);
                cellMatrix[x][y].isAlive = true;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                Vector2 mousePos = GetMousePosition();
                int x = static_cast<int>(mousePos.x / cellSize);
                int y = static_cast<int>(mousePos.y / cellSize);
                cellMatrix[x][y].isAlive = false;
            }
        }

        // handles input
        if (IsKeyPressed(KEY_R)) {
            for (int i = 0; i < gridWidth * gridHeight / 2; ++i) {
                cellMatrix[GetRandomValue(0, gridWidth - 1)][GetRandomValue(0, gridHeight - 1)].isAlive = true;
            }
        }
        if (IsKeyPressed(KEY_C)) {
            for (int i = 0; i < gridWidth; ++i) {
                for (int j = 0; j < gridHeight; ++j) {
                    cellMatrix[i][j].isAlive = false;
                }
            }
        }
        if (IsKeyPressed(KEY_ONE)) {
            drawGlider(cellMatrix, GetMouseX() / cellSize, GetMouseY() / cellSize);
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

            // drawing hud 
            DrawRectangle(0, 0, screenWidth, 55, Fade(BLACK, 0.7f));
            DrawText(binds, 12, 12, 20, BLACK);
            DrawText(binds, 10, 10, 20, WHITE);
            DrawText(entities, 10, 35, 10, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

static int numberOfAliveNeighbours(Cell(&matrix)[gridWidth][gridHeight], int x, int y) {
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

static void drawGlider(Cell(&matrix)[gridWidth][gridHeight], int x, int y) {
    if (x > gridWidth - 3 || y > gridHeight - 3)
        return;

    matrix[x + 0][y].isAlive = false;
    matrix[x + 1][y].isAlive = true;
    matrix[x + 2][y].isAlive = false;

    matrix[x + 0][y + 1].isAlive = false;
    matrix[x + 1][y + 1].isAlive = false;
    matrix[x + 2][y + 1].isAlive = true;

    matrix[x + 0][y + 2].isAlive = true;
    matrix[x + 1][y + 2].isAlive = true;
    matrix[x + 2][y + 2].isAlive = true;
}