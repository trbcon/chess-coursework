#include "piece.h"

Piece::Piece(
    bool white,
    string symbol,
    PieceType type
) {
    this->white = white;
    this->symbol = symbol;
    this->type = type;
    moved = false;
}

Piece::~Piece() {
}

bool Piece::isWhite() {

    return white;
}

string Piece::getSymbol() {
    return symbol;
}

PieceType Piece::getType() {
    return type;
}

bool Piece::hasMoved() {
    return moved;
}

void Piece::setMoved(bool value) {
    moved = value;
}