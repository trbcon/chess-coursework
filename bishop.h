#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {

public:
    Bishop(bool white);
    bool canMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    ) override;
};

#endif