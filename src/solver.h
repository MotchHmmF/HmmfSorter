#pragma once

#include <raylib.h>
#include <vector>
#include <stack>

class GameBoard;

class Solver {
    public:
        Solver(GameBoard& gameboard);
        void Draw();
        void Update();
    private:
        GameBoard& gameBoardRef;
        Vector2 pos{0,0};
        Vector2 target{0,0};
        int moveDistance;

        bool faster;

        int renderSize;
        int textSize;
        int gridSize;

        unsigned char memory, min;
        int memoryPos = 0, minPos = 0;
        int headPos = 0, currPos = 0;
        int swapping = 0;

        std::vector<std::pair<int, int>> order;

        void GenerateOrder();
        void Move();
        void Finished();

        unsigned char GetViaOrder(int pos);
        void SetViaOrder(int pos, unsigned char val);
        void TargetViaOrder(int pos);

        void SelectionSort();
        void BubbleSort();
        void InsertionSort();
        // void MergeSort();

};