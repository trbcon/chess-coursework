#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece {
public:
    Pawn(bool white);
    bool canMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    ) override;
};

#endif