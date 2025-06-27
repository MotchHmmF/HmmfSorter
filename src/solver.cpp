#include "solver.h"
#include "gameBoard.h"

Solver::Solver(GameBoard& gameboard) : gameBoardRef(gameboard) {

    pos.x = gameBoardRef.gridSize * 1.5;
    pos.y = gameBoardRef.gridSize * 1.5;
    target = pos;

    moveDistance = gameBoardRef.gridSize;
    renderSize = gameBoardRef.gridSize / 4;
    textSize = gameBoardRef.gridSize / 2;
    gridSize = gameBoardRef.gridSize;

    min = gameBoardRef.GetPos(0,0);
    memory = gameBoardRef.GetPos(0,0);

    faster = true;

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
            // std::cout << x << "," << y << std::endl;
            order.push_back(std::make_pair(x,y));
        }
        y += 1;
        x -= 1;
    }
}

void Solver::Draw() {

    if (!faster) {
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
    }
    

};

void Solver::Move() {
    if (!faster) {
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
    } else {
        pos = target;
    }
}

void Solver::Update() {
    if (!faster){
        if (pos.x != target.x && pos.y != target.y) {
            Move();
        } else {
            BubbleSort();
        }
    } else {
        Move();
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

unsigned char Solver::GetViaOrder(int pos) {
    int x = order.at(pos).first;
    int y = order.at(pos).second;
    return gameBoardRef.GetPos(x,y);
};

void Solver::SetViaOrder(int pos, unsigned char val) {
    int x = order.at(pos).first;
    int y = order.at(pos).second;
    gameBoardRef.SetPos(x,y,val);
};

void Solver::TargetViaOrder(int pos) {
    if (pos >= 0 && pos < order.size()) {
        target.x = (order.at(pos).first + 1) * gridSize + gridSize * 0.5f;
        target.y = (order.at(pos).second + 1) * gridSize + gridSize * 0.5f;
    }
};

void Solver::SelectionSort() {

    if (headPos < order.size()) {
        // int x, y;
        if (currPos == headPos) {
            // x = order.at(headPos).first;
            // y = order.at(headPos).second;
            minPos=headPos;
            // min = gameBoardRef.GetPos(x,y);
            min = GetViaOrder(headPos);

            memoryPos=minPos;
            memory=min;
        }
        if (++currPos < order.size()) {
            // x = order.at(currPos).first;
            // y = order.at(currPos).second;
            // unsigned char val = gameBoardRef.GetPos(x,y);
            unsigned char val = GetViaOrder(currPos);
            
            // target.x = (x+1) * gridSize + gridSize*0.5f;
            // target.y = (y+1) * gridSize + gridSize*0.5f;
            TargetViaOrder(currPos);

            if (val > min) {
                min = val;
                minPos = currPos;
            }
        } else {
            if (swapping == 0) {
                // x = order.at(headPos).first;
                // y = order.at(headPos).second;
                // target.x = (x+1) * gridSize + gridSize*0.5f;
                // target.y = (y+1) * gridSize + gridSize*0.5f;
                TargetViaOrder(headPos);
                swapping++;
            } else if (swapping == 1) {
                // x = order.at(headPos).first;
                // y = order.at(headPos).second;
                // gameBoardRef.SetPos(x,y,0);
                SetViaOrder(headPos,0);
                // x = order.at(minPos).first;
                // y = order.at(minPos).second;
                // target.x = (x+1) * gridSize + gridSize*0.5f;
                // target.y = (y+1) * gridSize + gridSize*0.5f;
                TargetViaOrder(minPos);
                swapping++;
            } else if (swapping == 2) {
                // x = order.at(minPos).first;
                // y = order.at(minPos).second;
                // gameBoardRef.SetPos(x,y,memory);
                SetViaOrder(minPos,memory);
                // x = order.at(headPos).first;
                // y = order.at(headPos).second;
                // target.x = (x+1) * gridSize + gridSize*0.5f;
                // target.y = (y+1) * gridSize + gridSize*0.5f;
                TargetViaOrder(headPos);
                swapping++;
            } else if (swapping == 3) {
                // x = order.at(headPos).first;
                // y = order.at(headPos).second;
                // gameBoardRef.SetPos(x,y,min);
                SetViaOrder(headPos,min);
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
            // int px = order.at(currPos-1).first;
            // int py = order.at(currPos-1).second;
            // int cx = order.at(currPos).first;
            // int cy = order.at(currPos).second;

            // memory = gameBoardRef.GetPos(px,py);
            memory = GetViaOrder(currPos-1);
        
            // target.x = (cx+1) * gridSize + gridSize*0.5f;
            // target.y = (cy+1) * gridSize + gridSize*0.5f;
            TargetViaOrder(currPos);

            // if (gameBoardRef.GetPos(px,py) < gameBoardRef.GetPos(cx,cy)) {
            if (GetViaOrder(currPos-1) < GetViaOrder(currPos)) {
                SetViaOrder(currPos-1,GetViaOrder(currPos));
                SetViaOrder(currPos,memory);
                // gameBoardRef.SetPos(px,py,gameBoardRef.GetPos(cx,cy));
                // gameBoardRef.SetPos(cx,cy,memory);
                
                minPos = currPos;
            }
            currPos++;
        } else {
            // target.x = gridSize + gridSize*0.5f;
            // target.y = gridSize + gridSize*0.5f;
            TargetViaOrder(0);
            
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

void Solver::InsertionSort() {

    if (headPos < order.size()) {
        if (memoryPos > 0 && GetViaOrder(memoryPos-1) < GetViaOrder(memoryPos)) {
            TargetViaOrder(memoryPos-1);
            memory = GetViaOrder(memoryPos);
            SetViaOrder(memoryPos,GetViaOrder(memoryPos-1));
            SetViaOrder(memoryPos-1,memory);
            memoryPos--;
        } else {
            headPos++;
            memoryPos=headPos;
            TargetViaOrder(headPos);
        }
    } else {
        Finished();
    }

    // int i = 0;
    // while (i < order.size()) {
    //     int j = i;
    //     while (j > 0 && GetViaOrder(j-1) > GetViaOrder(j)) {
    //         memory = GetViaOrder(j);
    //         setViaOrder(j,GetViaOrder(j-1));
    //         setViaOrder(j-1,memory);
    //         j--;
    //     }
    //     i++;
    // }


}

// void Solver::MergeSort() {
//     std::vector<unsigned char> toSort {100, 234, 63, 152, 167, 102, 2, 63, 90, 220, 17};

//     std::cout << std::endl << "Init" << std::endl;
//     for (unsigned char print : toSort) std::cout << static_cast<int>(print) << " ";
//     std::cout << std::endl << std::endl;

//     int n = toSort.size();

//     for (int curr = 1; curr <= n-1; curr = 2*curr) {
//         for (int left = 0; left < n-1; left += 2*curr) {
//             int mid = std::min(left + curr - 1, n-1);
//             int right = std::min(left + 2 * curr - 1, n-1);

//         }
//     }
    

//     std::cout << std::endl << "Final" << std::endl;
//     for (unsigned char print : toSort) std::cout << static_cast<int>(print) << " ";
//     std::cout << std::endl << std::endl;
// }