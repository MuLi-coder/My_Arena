//
// Created by ml on 2026/5/26.
//

#include "PreBoard.h"

PreBoard::PreBoard(const int r,const int c, const int pos)
: row(r), col(c),ben(pos){
    grid.resize(row, std::vector<Unit*>(col, nullptr));
    bench.resize(ben,nullptr);
}


PreBoard::~PreBoard() {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            delete grid[r][c];
            grid[r][c] = nullptr;
        }
    }

    for (int k=0;k<ben;k++) {
        delete bench[k];
        bench[k] = nullptr;
    }
}


int PreBoard::getRow() const { return row; }
int PreBoard::getCol() const { return col; }
int PreBoard::getBen() const { return ben; }

Unit* PreBoard::getUnitAt(const int r, const int c) const {
    if (this->isPositionValid(r,c)) {
        return grid[r][c];
    }
    return nullptr;
}

Unit* PreBoard::getUnitAt(const int pos) const {
    if (this->isPositionValid(pos)) {
        return bench[pos];
    }
    return nullptr;
}


bool PreBoard::isPositionValid(const int r, const int c) const {
    if (r < 0 || r >= row || c < 0 || c >= col) {
        return false;
    }
    return true;
}

bool PreBoard::isPositionValid(const int pos) const {
    if (pos<0 || pos >= ben) {
        return false;
    }
    return true;
}

bool PreBoard::isCellEmpty(const int r, const int c) const {
    if (this->isPositionValid(r,c)) {
        return grid[r][c] == nullptr;
    };
    return false;
}

bool PreBoard::isCellEmpty(const int pos) const {
    if (this->isPositionValid(pos)) {
        return bench[pos] == nullptr;
    }
    return false;
}


void PreBoard::placeUnitAt(const int r, const int c, Unit* hero) {
    if (this->isCellEmpty(r, c)) {
        grid[r][c] = hero ;
        hero->setX(r);
        hero->setY(c);
    }
}

void PreBoard::placeUnitAt(const int pos, Unit* hero) {
    if (this->isCellEmpty(pos)) {
        bench[pos] = hero ;
    }
}

void PreBoard::removeUnitAt(int r, int c) {
    if (this->isPositionValid(r,c)) {
        grid[r][c] = nullptr;
    }
}

void PreBoard::removeUnitAt(int pos) {
    if (this->isPositionValid(pos)) {
        bench[pos] = nullptr;
    }
}


void PreBoard::rePrepare() {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            if (!this->isCellEmpty(r,c)) {
                Unit* unit = this->getUnitAt(r,c);
                unit->setHp(unit->getMaxHp());
            }
        }
    }
}