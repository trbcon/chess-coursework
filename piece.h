#ifndef PIECE_H
#define PIECE_H

#include <string>

using namespace std;

enum PieceType {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN
};

class Piece {
protected:
    bool white;
    string symbol;
    PieceType type;
    bool moved;

public:
    Piece(
        bool white,
        string symbol,
        PieceType type
    );

    virtual ~Piece();

    bool isWhite();

    string getSymbol();

    PieceType getType();

    bool hasMoved();
    void setMoved(bool value);

    virtual bool canMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    ) = 0;
};

#endif