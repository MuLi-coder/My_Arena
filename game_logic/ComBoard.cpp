//
// Created by ml on 2026/6/8.
//

#include "ComBoard.h"



ComBoard::ComBoard(const PreBoard &preBoard)
    :row(preBoard.getRow()),col(preBoard.getCol()){
    grid.resize(row, std::vector<Unit*>(col, nullptr));

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            grid[r][c] = preBoard.getUnitAt(r,c);
        }
    }
}

void ComBoard::setComBoard(const PreBoard &preBoard) {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            grid[r][c] = preBoard.getUnitAt(r,c);
        }
    }
}

void ComBoard::reset(const PreBoard &preBoard) {
    row = preBoard.getRow();
    col = preBoard.getCol();
    grid.resize(row, std::vector<Unit*>(col, nullptr));
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            grid[r][c] = preBoard.getUnitAt(r,c);
        }
    }
}

void ComBoard::clear() {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            grid[r][c] = nullptr;
        }
    }
}

bool ComBoard::isCellEmpty(int r, int c) const {
    if (grid[r][c] != nullptr) {
        return false;
    }
    return true;
}

Unit* ComBoard::getUnitAt(const int r, const int c)const {
    return grid[r][c];
}

void ComBoard::moveUnitFromTo(const int ini_r,const int ini_c,const int target_r,const int target_c, Unit* unit) {
    grid[target_r][target_c] = unit;
    grid[ini_r][ini_c] = nullptr;
}

void ComBoard::marchEveryUnit() {

    std::vector<std::vector<Unit*>> oldGrid = grid;
    std::cout<<"copy"<<std::endl;

    //这边棋子开始行动
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            if (oldGrid[r][c] != nullptr) {
                Unit* unit = oldGrid[r][c];
                //取出棋子的行动需求
                Action action =unit->march(grid,row,col,r,c);
                //第一种是行动
                if (action.move!="Null") {
                    if (action.move =="Left") {
                        //首先依照意愿行动,先左
                        if (isCellEmpty(r,c-1)) {
                            moveUnitFromTo(r,c,r,c-1,unit);
                            continue;
                        }
                        //意愿不能执行，执行替代方案，不反向移动
                        if (isCellEmpty(r+1,c)) {
                            moveUnitFromTo(r,c,r+1,c,unit);
                            continue;
                        }
                        if (isCellEmpty(r-1,c)) {
                            moveUnitFromTo(r,c,r-1,c,unit);
                            continue;
                        }
                    }
                    else if (action.move =="Right") {
                        //先右
                        if (isCellEmpty(r,c+1)) {
                            moveUnitFromTo(r,c,r,c+1,unit);
                            continue;
                        }
                        if (isCellEmpty(r+1,c)) {
                            moveUnitFromTo(r,c,r+1,c,unit);
                            continue;
                        }
                        if (isCellEmpty(r-1,c)) {
                            moveUnitFromTo(r,c,r-1,c,unit);
                            continue;
                        }
                    }
                    else if (action.move =="Up") {
                        //先上
                        if (isCellEmpty(r-1,c)) {
                            moveUnitFromTo(r,c,r-1,c,unit);
                            continue;
                        }
                        if (isCellEmpty(r,c-1)) {
                            moveUnitFromTo(r,c,r,c-1,unit);
                            continue;
                        }
                        if (isCellEmpty(r,c+1)) {
                            moveUnitFromTo(r,c,r,c+1,unit);
                            continue;
                        }
                    }
                    else if (action.move =="Down") {
                        //先下
                        if (isCellEmpty(r+1,c)) {
                            moveUnitFromTo(r,c,r+1,c,unit);
                            continue;
                        }
                        if (isCellEmpty(r,c+1)) {
                            moveUnitFromTo(r,c,r,c+1,unit);
                            continue;
                        }
                        if (isCellEmpty(r,c-1)) {
                            moveUnitFromTo(r,c,r,c-1,unit);
                            continue;
                        }
                    }
                }
                //第二种是攻击
                if (action.attack!="Null") {
                    if (action.attack=="CommonAtt") {
                        Unit* target = getUnitAt(action.targetX,action.targetY);
                        target->takeDamage(unit->getAtt());
                    }
                }
            }
        }
    }
    // 刷新棋盘数据,去除hp==0的棋子,以及重置棋子的行动状态
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            if (grid[r][c] != nullptr) {
                grid[r][c]->resetAction();
                if (grid[r][c]->getHp()==0) {
                    grid[r][c]=nullptr;
                }
            }
        }
    }

}
