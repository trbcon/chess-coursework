#include "knight.h"
#include <cmath>

Knight::Knight(bool white) : Piece(white, white ? "♘" : "♞", KNIGHT) {}

bool Knight::canMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}