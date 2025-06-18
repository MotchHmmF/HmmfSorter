#include "gameBoard.h"

GameBoard::GameBoard(int width, int height, int gridSize, unsigned int seed, bool drawnums) 
            : width(width), height(height), gridSize(gridSize), seed(seed), drawNums(drawnums) {
    grid = new unsigned char*[width];
    for (int x = 0; x < width; x++) {
        grid[x] = new unsigned char[height];
        for (int y = 0; y < height; y++) {
            grid[x][y] = 100;
        }
    }

    srand(seed);
    Randomise();

    solver = new Solver(*this);
};

void GameBoard::Start() {
    InitWindow((width+2)*gridSize,(height+2)*gridSize,"Hmmf's Sorters");
    SetTargetFPS(60);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();

    while (!WindowShouldClose()) {

        for (int _ = 0; _ < 5; _++) solver->Update();

        Draw();
    }
    CloseWindow();
};

void GameBoard::Draw() {
    BeginDrawing();
    if (drawNums) ClearBackground(RAYWHITE);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (!drawNums){
                DrawRectangle((x+1)*gridSize,(y+1)*gridSize,gridSize,gridSize,Color{grid[x][y],0,0,255});
            } else {
                DrawText(TextFormat("%d", grid[x][y]), (x+1)*gridSize, (y+1)*gridSize, gridSize/2, BLACK);
            }
            
        }
    }

    solver->Draw();
    EndDrawing();
}

void GameBoard::Randomise() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            grid[x][y] = rand()%256;
        }
    }
}

unsigned char GameBoard::GetPos(int x, int y) {
    return grid[x][y];
};

void GameBoard::SetPos(int x, int y, unsigned char val) {
    grid[x][y] = val;
}