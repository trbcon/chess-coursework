#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "move.h"

class Board {

private:
    Piece* board[8][8];

    string moveHistory[1000];
    string capturedWhite[32];
    string capturedBlack[32];

    bool whiteTurn;
    int aiDepth;

    int whiteTime;
    int blackTime;

    int lastFromRow;
    int lastFromCol;

    int lastToRow;
    int lastToCol;

    int moveCount;
    int capturedWhiteCount;
    int capturedBlackCount;

public:
    Board();

    void setup();

    void print();

    void setDifficulty(int depth);

    bool isPathClear(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    );

    bool validatePawnMove(
        Piece* piece,
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    );

    void promotePawn(
        int row,
        int col
    );

    bool isKingInCheck(bool whiteKing);

    void parsePosition(
        string pos,
        int &row,
        int &col
    );

    bool hasAnyMoves(bool whitePlayer);

    bool isStalemate(bool whitePlayer);

    bool isCheckmate(bool whiteKing);

    bool movePiece(
        string from,
        string to
    );

    bool validateCastling(
        Piece* king,
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    );

    bool isLegalMove(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    );

    bool validateEnPassant(
        Piece* pawn,
        int fromRow,
        int fromCol,
        int toRow,
        int toCol
    );

    int generateMoves(
        bool whitePlayer,
        Move moves[]
    );

    void makeRandomMove();

    int evaluateBoard();

    void makeBestMove();

    int minimax(
        int depth,
        bool maximizingPlayer,
        int alpha,
        int beta
    );

    void gameLoopPvP();

    void gameLoopPvE();

    void saveGame(string filename);

    void loadGame(string filename);

    ~Board();

};

#endif