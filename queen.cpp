#include "queen.h"
#include <cmath>

Queen::Queen(bool white) : Piece(white, white ? "♕" : "♛", QUEEN) {}

bool Queen::canMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    bool rookMove =
        fromRow == toRow ||
        fromCol == toCol;

    bool bishopMove =
        abs(toRow - fromRow) ==
        abs(toCol - fromCol);

    return rookMove || bishopMove;
}