//
// Created by ml on 2026/5/26.
//

#include "Board.h"

Board::Board(const int rows,const int cols, const int ben)
: rows_(rows), cols_(cols),ben_(ben){
    // 初始化棋盘和备战区数组，0代表空
    grid_.resize(rows, std::vector<Unit*>(cols, nullptr));
    bench_.resize(ben_,nullptr);

}


Board::~Board() {
    for (int row = 0; row < rows_; row++) {
        for (int col = 0; col < cols_; col++) {
            delete grid_[row][col];
            grid_[row][col] = nullptr;
        }
    }

    for (int c=0;c<ben_;c++) {
        delete bench_[c];
        bench_[c] = nullptr;
    }
}

bool Board::isCellEmpty(const int row, const int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return false;
    return grid_[row][col] == nullptr;
}

bool Board::isCellEmpty(const int pos) const {
    if (pos < 0 || pos >= ben_) {
        return false;
    }
    return bench_[pos] == nullptr;
}


Unit* Board::getUnitAt(const int row, const int col) const {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return nullptr;
    return grid_[row][col];
}

Unit* Board::getUnitAt(const int pos) const {
    if (pos < 0 || pos >= ben_) {
        return nullptr;
    }
    return bench_[pos];
}


void Board::placeUnitAt(const int row, const int col, Unit* hero) {
    if (isCellEmpty(row, col)) {
        grid_[row][col] = hero ;
    }
}

void Board::placeUnitAt(const int pos, Unit* hero) {
    if (isCellEmpty(pos)) {
        bench_[pos] = hero ;
    }
}


void Board::removeUnitAt(int row, int col) {
    if (row >= 0 && row < rows_ && col >= 0 && col < cols_) {
        grid_[row][col] = nullptr;
    }
}

void Board::removeUnitAt(int pos) {
    if (pos >= 0 && pos < ben_) {
        bench_[pos] = nullptr;
    }
}


int Board::getRows() const { return rows_; }
int Board::getCols() const { return cols_; }
int Board::getBen() const { return ben_; }