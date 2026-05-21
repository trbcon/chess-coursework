#include "pawn.h"

Pawn::Pawn(bool white) : Piece(white, white ? "♙" : "♟", PAWN) {}

bool Pawn::canMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    int direction;
    if (white) direction = -1;
    else direction = 1;

    int rowDiff = toRow - fromRow;
    int colDiff = abs(toCol - fromCol);

    if (colDiff == 0 && (rowDiff == direction || rowDiff == 2 * direction)) return true;

    if (colDiff == 1 && rowDiff == direction) return true;

    return false;
}