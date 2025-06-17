#pragma once

#include <raylib.h>
#include "solver.h"

#include <iostream>
#include <random>
#include <queue>

class GameBoard {
    public:
        int width, height, gridSize;
        bool drawNums;

        GameBoard(int width=10, int height=10, int gridSize=40, unsigned int seed=0, bool drawNums = false);

        void Start();

        unsigned char GetPos(int x, int y);
        void SetPos(int x, int y, unsigned char val);
        void Randomise();
    private:
        unsigned char** grid;
        unsigned int seed;
        Solver* solver = nullptr;

        void Draw();
};