#include "rook.h"

Rook::Rook(bool white) : Piece(white, white ? "♖" : "♜", ROOK) {}

bool Rook::canMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    return fromRow == toRow || fromCol == toCol;
}