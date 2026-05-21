#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece {
public:
    Knight(bool white);

    bool canMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    ) override;
};

#endif