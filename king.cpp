#include "king.h"
#include <cmath>

King::King(bool white) : Piece(white, white ? "♔" : "♚", KING) {}

bool King::canMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    return rowDiff <= 1 && colDiff <= 1;
}