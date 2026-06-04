//
// Created by ml on 2026/5/26.
//

#include "Board.h"

Board::Board(const int r,const int c, const int pos)
: row(r), col(c),ben(pos){
    grid.resize(row, std::vector<Unit*>(col, nullptr));
    bench.resize(ben,nullptr);
}


Board::~Board() {
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


int Board::getRow() const { return row; }
int Board::getCol() const { return col; }
int Board::getBen() const { return ben; }

Unit* Board::getUnitAt(const int r, const int c) const {
    if (this->isPositionValid(r,c)) {
        return grid[r][c];
    }
    return nullptr;
}

Unit* Board::getUnitAt(const int pos) const {
    if (this->isPositionValid(pos)) {
        return bench[pos];
    }
    return nullptr;
}


bool Board::isPositionValid(const int r, const int c) const {
    if (r < 0 || r >= row || c < 0 || c >= col) {
        return false;
    }
    return true;
}

bool Board::isPositionValid(const int pos) const {
    if (pos<0 || pos >= ben) {
        return false;
    }
    return true;
}

bool Board::isCellEmpty(const int r, const int c) const {
    if (this->isPositionValid(r,c)) {
        return grid[r][c] == nullptr;
    };
    return false;
}

bool Board::isCellEmpty(const int pos) const {
    if (this->isPositionValid(pos)) {
        return bench[pos] == nullptr;
    }
    return false;
}


void Board::placeUnitAt(const int r, const int c, Unit* hero) {
    if (this->isCellEmpty(r, c)) {
        grid[r][c] = hero ;
        hero->setX(r);
        hero->setY(c);
    }
}

void Board::placeUnitAt(const int pos, Unit* hero) {
    if (this->isCellEmpty(pos)) {
        bench[pos] = hero ;
    }
}

void Board::removeUnitAt(int r, int c) {
    if (this->isPositionValid(r,c)) {
        grid[r][c] = nullptr;
    }
}

void Board::removeUnitAt(int pos) {
    if (this->isPositionValid(pos)) {
        bench[pos] = nullptr;
    }
}

void Board::moveUnit() {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            if (!this->isCellEmpty(r,c)) {
                this->getUnitAt(r,c)->march();
            }
        }
    }
}

void Board::rePrepare() {
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            if (!this->isCellEmpty(r,c)) {
                this->getUnitAt(r,c)->setX(r);
                this->getUnitAt(r,c)->setY(c);
            }
        }
    }
}