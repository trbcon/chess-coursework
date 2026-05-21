#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {
public:
    Rook(bool white);

    bool canMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    ) override;
};

#endif