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
    if (r<0||r>=row||c<0||c>=col) { return false; }
    if (grid[r][c] != nullptr) {
        return false;
    }
    return true;
}

Unit* ComBoard::getUnitAt(const int r, const int c)const {
    return grid[r][c];
}

void ComBoard::moveUnitFromTo( int ini_r, int ini_c,int target_r, int target_c, Unit* unit) {
    grid[target_r][target_c] = unit;
    grid[ini_r][ini_c] = nullptr;
}

void ComBoard::marchEveryUnit() {

    //这边棋子开始行动
    for (int r = 0; r < row; ++r) {
        for (int c = 0; c < col; ++c) {
            if (grid[r][c] != nullptr && grid[r][c]->getState()==UnitState::Idle) {
                std::cout<<"-----find unit-----"<<std::endl;
                Unit* unit = grid[r][c];
                //取出棋子的行动需求
                Action action =unit->march(grid,row,col,r,c);
                std::cout<<"get action"<<std::endl;
                std::cout<< action.move << std::endl;
                std::cout<< action.attack<<std::endl;
                //第一种是行动
                if (action.move != "Null") {
                    if (action.move =="Left") {
                        //首先依照意愿行动,先左
                        if (isCellEmpty(r,c-1)) {
                            moveUnitFromTo(r,c,r,c-1,unit);
                            unit->changeStateTo(UnitState::Done);
                            continue;
                        }
                        //意愿不能执行，执行替代方案，不反向移动
                        if (randomNum()<50) {
                            if (isCellEmpty(r+1,c)) {
                                moveUnitFromTo(r,c,r+1,c,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }else {
                            if (isCellEmpty(r-1,c)) {
                                moveUnitFromTo(r,c,r-1,c,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }
                    }
                    else if (action.move =="Right") {
                        //先右
                        if (isCellEmpty(r,c+1)) {
                            moveUnitFromTo(r,c,r,c+1,unit);
                            unit->changeStateTo(UnitState::Done);
                            continue;
                        }
                        if (randomNum()<50) {
                            if (isCellEmpty(r+1,c)) {
                                moveUnitFromTo(r,c,r+1,c,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }else {
                            if (isCellEmpty(r-1,c)) {
                                moveUnitFromTo(r,c,r-1,c,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }
                    }
                    else if (action.move =="Up") {
                        //先上
                        if (isCellEmpty(r-1,c)) {
                            moveUnitFromTo(r,c,r-1,c,unit);
                            unit->changeStateTo(UnitState::Done);
                            continue;
                        }
                        if (randomNum()<50) {
                            if (isCellEmpty(r,c-1)) {
                                moveUnitFromTo(r,c,r,c-1,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }else {
                            if (isCellEmpty(r,c+1)) {
                                moveUnitFromTo(r,c,r,c+1,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }
                    }
                    else if (action.move =="Down") {
                        //先下
                        if (isCellEmpty(r+1,c)) {
                            moveUnitFromTo(r,c,r+1,c,unit);
                            unit->changeStateTo(UnitState::Done);
                            continue;
                        }
                        if (randomNum()<50) {
                            if (isCellEmpty(r,c+1)) {
                                moveUnitFromTo(r,c,r,c+1,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }else {
                            if (isCellEmpty(r,c-1)) {
                                moveUnitFromTo(r,c,r,c-1,unit);
                                unit->changeStateTo(UnitState::Done);
                                continue;
                            }
                        }
                    }
                }

                //第二种是攻击
                if (action.attack!="Null") {
                    if (action.attack=="CommonAtt") {
                        unit->changeStateTo(UnitState::Done);
                        continue;
                    }
                    if (action.attack=="SkillAtt"){
                        unit->changeStateTo(UnitState::Done);
                        continue;
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
                grid[r][c]->changeStateTo(UnitState::Idle);
                if (grid[r][c]->getHp()==0) {
                    grid[r][c]=nullptr;
                }
            }
        }
    }
}
