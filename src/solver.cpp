#include "solver.h"
#include "gameBoard.h"

Solver::Solver(GameBoard& gameboard) : gameBoardRef(gameboard) {

    pos.x = gameBoardRef.gridSize * 1.5;
    pos.y = gameBoardRef.gridSize * 1.5;
    target = pos;

    moveDistance = gameBoardRef.gridSize * 8;
    renderSize = gameBoardRef.gridSize / 4;
    textSize = gameBoardRef.gridSize / 2;
    gridSize = gameBoardRef.gridSize;

    min = gameBoardRef.GetPos(0,0);
    memory = gameBoardRef.GetPos(0,0);

    GenerateOrder();

};

void Solver::GenerateOrder() {
    int x = 0, y = 0, prevx = 0, count = 0;
    while(order.size() != gameBoardRef.width*gameBoardRef.height) {
        if (y >= gameBoardRef.height || x < 0) {
            x = ++prevx;
            y = 0;
        }
        if (x < gameBoardRef.width) {
            std::cout << x << "," << y << std::endl;
            order.push_back(std::make_pair(x,y));
        }
        y += 1;
        x -= 1;
    }
}

void Solver::Draw() {
    // Solver Pos
    DrawCircle(pos.x,pos.y,renderSize,Color{ 0, 121, 241, 255 });
    // if (gameBoardRef.drawNums) {
    //     DrawCircle(pos.x,pos.y,renderSize,Color{ 0, 121, 241, 20 });
    // } else {
    //     DrawCircle(pos.x,pos.y,renderSize,Color{ 0, 121, 241, 255 });
    // }
    
    // Memory Vals
    DrawText("Min:",1,1,textSize,BLACK);
    if (gameBoardRef.drawNums) {
        DrawText(TextFormat("%d", min), gridSize, 1, textSize, BLACK);
    } else {
        DrawRectangle(gridSize,1,textSize,textSize,Color{min,0,0,255});
    }
    
    DrawText("Memory:",textSize*4,1,textSize,BLACK);
    if (gameBoardRef.drawNums) {
        DrawText(TextFormat("%d", memory), gridSize*4, 1, textSize, BLACK);
    } else {
        DrawRectangle(gridSize*4,1,textSize,textSize,Color{memory,0,0,255});
    }

    // ClearBackground(RAYWHITE);
    // for (int i = 0; i < order.size(); i++) {
    //     DrawText(TextFormat("%d", i),(order.at(i).first + 1) * gridSize,(order.at(i).second + 1) * gridSize,textSize,WHITE);
    // }

};

void Solver::Move() {
    Vector2 direction = { target.x - pos.x, target.y - pos.y };
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);

    if (length < moveDistance || length == 0) {
        pos = target;
        return;
    }

    direction.x /= length;
    direction.y /= length;

    pos.x += direction.x * moveDistance;
    pos.y += direction.y * moveDistance;
}

void Solver::Update() {
    if (pos.x != target.x && pos.y != target.y) {
        Move();
    } else {
        SelectionSort();
    }
};

void Solver::Finished() {
    WaitTime(2);
    gameBoardRef.Randomise();

    min = gameBoardRef.GetPos(0,0);
    memory = gameBoardRef.GetPos(0,0);

    target.x = gridSize*1.5;
    target.y = gridSize*1.5;

    memoryPos = 0, minPos = 0;
    headPos = 0, currPos = 0;
    swapping = 0;
}

void Solver::SelectionSort() {

    if (headPos < order.size()) {
        int x, y;
        if (currPos == headPos) {
            x = order.at(headPos).first;
            y = order.at(headPos).second;
            minPos=headPos;
            min = gameBoardRef.GetPos(x,y);

            memoryPos=minPos;
            memory=min;
        }
        if (++currPos < order.size()) {
            x = order.at(currPos).first;
            y = order.at(currPos).second;
            unsigned char val = gameBoardRef.GetPos(x,y);
            
            target.x = (x+1) * gridSize + gridSize*0.5f;
            target.y = (y+1) * gridSize + gridSize*0.5f;

            if (val > min) {
                min = val;
                minPos = currPos;
            }
        } else {
            if (swapping == 0) {
                x = order.at(headPos).first;
                y = order.at(headPos).second;
                target.x = (x+1) * gridSize + gridSize*0.5f;
                target.y = (y+1) * gridSize + gridSize*0.5f;
                swapping++;
            } else if (swapping == 1) {
                x = order.at(headPos).first;
                y = order.at(headPos).second;
                gameBoardRef.SetPos(x,y,0);
                x = order.at(minPos).first;
                y = order.at(minPos).second;
                target.x = (x+1) * gridSize + gridSize*0.5f;
                target.y = (y+1) * gridSize + gridSize*0.5f;
                swapping++;
            } else if (swapping == 2) {
                x = order.at(minPos).first;
                y = order.at(minPos).second;
                gameBoardRef.SetPos(x,y,memory);
                x = order.at(headPos).first;
                y = order.at(headPos).second;
                target.x = (x+1) * gridSize + gridSize*0.5f;
                target.y = (y+1) * gridSize + gridSize*0.5f;
                swapping++;
            } else if (swapping == 3) {
                x = order.at(headPos).first;
                y = order.at(headPos).second;
                gameBoardRef.SetPos(x,y,min);
                swapping=0;
                headPos++;
                currPos=headPos;
            }

        }
    } else {
        Finished();
    }
};