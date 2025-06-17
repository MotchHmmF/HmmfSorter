#include "solver.h"
#include "gameBoard.h"

Solver::Solver(GameBoard& gameboard) : gameBoardRef(gameboard) {

    pos.x = gameBoardRef.gridSize * 1.5;
    pos.y = gameBoardRef.gridSize * 1.5;
    target = pos;

    moveDistance = gameBoardRef.gridSize*8;
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
        BubbleSort();
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

void Solver::BubbleSort() {
    // std::cout<<"Call"<<std::endl;
    
    if (minPos == 0) {
        // std::cout<<"    init"<<std::endl;
        headPos = order.size();
        currPos = 1;
    }

    if (headPos > 1) {
        // std::cout<<"n > 1"<<std::endl;
        if (currPos == 1) {
            // std::cout<<"    i++"<<std::endl;
            minPos = 1;
        };

        if (currPos < headPos) {
            int px = order.at(currPos-1).first;
            int py = order.at(currPos-1).second;
            int cx = order.at(currPos).first;
            int cy = order.at(currPos).second;

            memory = gameBoardRef.GetPos(px,py);
            target.x = (cx+1) * gridSize + gridSize*0.5f;
            target.y = (cy+1) * gridSize + gridSize*0.5f;

            if (gameBoardRef.GetPos(px,py) < gameBoardRef.GetPos(cx,cy)) {
                // std::cout<<"    Swap"<<std::endl;
                gameBoardRef.SetPos(px,py,gameBoardRef.GetPos(cx,cy));
                gameBoardRef.SetPos(cx,cy,memory);

                minPos = currPos;
            }
            currPos++;
        } else {
            target.x = gridSize + gridSize*0.5f;
            target.y = gridSize + gridSize*0.5f;
            
            headPos = minPos;
            currPos = 1;
        }
    } else {
        Finished();
    }

    // headPos = n
    // minPos = newn
    // currPos = i

    // int n = order.size();

    // while (n > 1) {
    //     int newn = 0;
    //     for (int i = 1; i < n; i++) {
    //         int px = order.at(i-1).first;
    //         int py = order.at(i-1).second;
    //         int cx = order.at(i).first;
    //         int cy = order.at(i).second;
    //         if (gameBoardRef.GetPos(px,py) > gameBoardRef.GetPos(cx,cy)) {
    //             memory = gameBoardRef.GetPos(px,py);
    //             gameBoardRef.SetPos(px,py,gameBoardRef.GetPos(cx,cy));
    //             gameBoardRef.SetPos(cx,cy,memory);

    //             newn = i;
    //         }
    //     }
    //     n = newn;
    // }

    // Finished();
}