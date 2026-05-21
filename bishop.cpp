#include "bishop.h"
#include <cmath>

Bishop::Bishop(bool white) : Piece(white, white ? "♗" : "♝", BISHOP){}

bool Bishop::canMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    return abs(toRow - fromRow) == abs(toCol - fromCol);
}