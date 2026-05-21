#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece {
public:
    King(bool white);

    bool canMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    ) override;
};

#endif