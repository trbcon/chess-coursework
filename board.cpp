#include <iostream>
#include <fstream>

#include "board.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

using namespace std;

Board::Board() {
    whiteTurn = true;
    aiDepth = 2;

    whiteTime = 600;
    blackTime = 600;

    lastFromRow = -1;
    lastFromCol = -1;
    lastToRow = -1;
    lastToCol = -1;

    moveCount = 0;

    capturedWhiteCount = 0;
    capturedBlackCount = 0;

    setup();
}

void Board::setup() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }

    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn(false);
        board[6][i] = new Pawn(true);
    }

    board[0][0] = new Rook(false);
    board[0][7] = new Rook(false);

    board[7][0] = new Rook(true);
    board[7][7] = new Rook(true);

    board[0][1] = new Knight(false);
    board[0][6] = new Knight(false);

    board[7][1] = new Knight(true);
    board[7][6] = new Knight(true);


    board[0][2] = new Bishop(false);
    board[0][5] = new Bishop(false);

    board[7][2] = new Bishop(true);
    board[7][5] = new Bishop(true);

    board[0][3] = new Queen(false);
    board[7][3] = new Queen(true);

    board[0][4] = new King(false);
    board[7][4] = new King(true);
}

void Board::saveGame(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка сохранения!\n";
        return;
    }
    for (int i = 0; i < moveCount; i++) {
        file << moveHistory[i] << endl;
    }

    file.close();

    cout << "\nИгра сохранена!\n";
}

void Board::loadGame(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Файл не найден!\n";
        return;
    }

    setup();

    whiteTurn = true;
    moveCount = 0;

    string from;
    string to;


    while (file >> from >> to) {
        movePiece(from, to);
    }
    file.close();

    cout << "\nИгра загружена!\n";
}


void Board::print() {
    cout << "\033[2J\033[1;1H";cout << "\n";

    cout << "Белые: "
        << whiteTime / 60
        << ":"
        << (whiteTime % 60 < 10 ? "0" : "")
        << whiteTime % 60
        << "\n";

    cout << "Черные: "
        << blackTime / 60
        << ":"
        << (blackTime % 60 < 10 ? "0" : "")
        << blackTime % 60
        << "\n";

    cout << "    a  b  c  d  e  f  g  h";
    cout << "        Последние ходы\n\n";

    for (int i = 0; i < 8; i++) {
        cout << " " << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            bool dark = (i + j) % 2;
            if (dark)
                cout << "\033[48;5;94m";
            else
                cout << "\033[48;5;180m";
            if (board[i][j] == nullptr)
                cout << "   ";
            else
                cout << " "
                     << board[i][j]->getSymbol()
                     << " ";

            cout << "\033[0m";
        }

        cout << "   ";

        int historyIndex =
            moveCount - 8 + i;

        if (historyIndex >= 0 &&
            historyIndex < moveCount)
        {
            cout <<
                moveHistory[historyIndex];
        }

        cout << "\n";
    }

    cout << "\n";
    cout << "Съедены белые: ";
    for (int i = 0;
         i < capturedWhiteCount;
         i++)
    {
        cout <<
            capturedWhite[i]
             << " ";
    }

    cout << "\n";
    cout << "Съедены черные: ";

    for (int i = 0;
         i < capturedBlackCount;
         i++)
    {
        cout <<
            capturedBlack[i]
             << " ";
    }
    cout << "\n";
}

int Board::generateMoves(
    bool whitePlayer,
    Move moves[]
) {
    int moveCount = 0;
    bool currentTurn = whiteTurn;
    whiteTurn = whitePlayer;
    for (int fromRow = 0;
         fromRow < 8;
         fromRow++)
    {
        for (int fromCol = 0;
             fromCol < 8;
             fromCol++)
        {
            Piece* piece =
                board[fromRow][fromCol];
            if (piece == nullptr)
                continue;
            if (piece->isWhite() != whitePlayer) continue;

            for (int toRow = 0;
                 toRow < 8;
                 toRow++)
            {
                for (int toCol = 0;
                     toCol < 8;
                     toCol++)
                {
                    if (isLegalMove(
                            fromRow,
                            fromCol,
                            toRow,
                            toCol))
                    {
                        moves[moveCount].fromRow = fromRow;
                        moves[moveCount].fromCol = fromCol;
                        moves[moveCount].toRow = toRow;
                        moves[moveCount].toCol = toCol;
                        moveCount++;
                    }
                }
            }
        }
    }
    whiteTurn = currentTurn;
    return moveCount;
}

void Board::makeRandomMove() {
    Move moves[500];
    int count = generateMoves(false, moves);
    if (count == 0) return;
    int index = rand() % count;

    Move move = moves[index];

    string from = "";

    from += ('a' + move.fromCol);
    from += ('8' - move.fromRow);

    string to = "";

    to += ('a' + move.toCol);
    to += ('8' - move.toRow);

    cout << "\nКомпьютер ходит: "
         << from
         << " "
         << to
         << "\n";

    movePiece(from, to);
}


void Board::parsePosition(
    string pos,
    int &row,
    int &col
) {
    col = pos[0] - 'a';
    row = 8 - (pos[1] - '0');
}


bool Board::isPathClear(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    int rowStep = 0;
    int colStep = 0;

    if (toRow > fromRow) rowStep = 1;

    if (toRow < fromRow) rowStep = -1;

    if (toCol > fromCol) colStep = 1;

    if (toCol < fromCol) colStep = -1;

    int currentRow = fromRow + rowStep;
    int currentCol = fromCol + colStep;

    while (currentRow != toRow || currentCol != toCol)
    {
        if (board[currentRow][currentCol] != nullptr) return false;

        currentRow += rowStep;
        currentCol += colStep;
    }

    return true;
}

bool Board::validatePawnMove(
    Piece* piece,
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    int direction;

    if (piece->isWhite()) direction = -1;
    else direction = 1;

    int rowDiff = toRow - fromRow;
    int colDiff = abs(toCol - fromCol);

    Piece* target = board[toRow][toCol];

    if (colDiff == 0) {
        if (target != nullptr) return false;

        if (rowDiff == direction) return true;

        if (rowDiff == 2 * direction) {
            if (piece->isWhite() && fromRow != 6) return false;

            if (!piece->isWhite() && fromRow != 1) return false;

            int middleRow = fromRow + direction;

            if (board[middleRow][fromCol] != nullptr) return false;

            return true;
        }
    }
    if (colDiff == 1 && rowDiff == direction)
    {
        if (target != nullptr && target->isWhite() != piece->isWhite()) return true;
    }

    return false;
}

bool Board::validateEnPassant(
    Piece* pawn,
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    if (pawn->getType() != PAWN) return false;

    int direction;

    if (pawn->isWhite()) direction = -1;
    else direction = 1;

    if (toRow != fromRow + direction) return false;

    if (abs(toCol - fromCol) != 1) return false;

    if (board[toRow][toCol] != nullptr) return false;

    Piece* enemyPawn = board[fromRow][toCol];

    if (enemyPawn == nullptr) return false;

    if (enemyPawn->getType() != PAWN) return false;

    if (enemyPawn->isWhite() == pawn->isWhite()) return false;

    if (lastToRow != fromRow) return false;

    if (lastToCol != toCol) return false;

    if (abs(lastFromRow - lastToRow) != 2) return false;

    return true;
}

bool Board::validateCastling(
    Piece* king,
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    if (king->getType() != KING) return false;

    if (king->hasMoved()) return false;

    if (fromRow != toRow) return false;

    int diff = toCol - fromCol;

    if (abs(diff) != 2)
        return false;

    if (diff == 2) {
        Piece* rook = board[fromRow][7];
        if (rook == nullptr) return false;
        if (rook->getType() != ROOK) return false;
        if (rook->isWhite() != king->isWhite()) return false;
        if (rook->hasMoved()) return false;
        if (board[fromRow][5] != nullptr) return false;
        if (board[fromRow][6] != nullptr) return false;
    }
    if (diff == -2) {
        Piece* rook = board[fromRow][0];
        if (rook == nullptr) return false;
        if (rook->getType() != ROOK) return false;
        if (rook->isWhite() != king->isWhite()) return false;
        if (rook->hasMoved()) return false;
        if (board[fromRow][1] != nullptr) return false;
        if (board[fromRow][2] != nullptr) return false;
        if (board[fromRow][3] != nullptr) return false;
    }
    if (isKingInCheck(king->isWhite())) return false;

    return true;
}

bool Board::isLegalMove(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    if (fromRow < 0 || fromRow > 7 ||
        fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 ||
        toCol < 0 || toCol > 7) return false;

    Piece* piece = board[fromRow][fromCol];

    if (piece == nullptr) return false;
    if (fromRow == toRow && fromCol == toCol) return false;
    if (piece->isWhite() != whiteTurn) return false;

    Piece* target = board[toRow][toCol];

    if (target != nullptr && target->isWhite() == piece->isWhite()) return false;

    if (target != nullptr) {
        if (target->getType() == KING) return false;
    }

    if (piece->getType() == KING) {
        if (validateCastling(
                piece,
                fromRow,
                fromCol,
                toRow,
                toCol))
        {
            return true;
        }
    }

    if (!piece->canMove(
            fromRow,
            fromCol,
            toRow,
            toCol))
    {
        return false;
    }

    string symbol =
        piece->getSymbol();

    if (piece->getType() == PAWN)
    {
        if (validateEnPassant(
                piece,
                fromRow,
                fromCol,
                toRow,
                toCol))
        {
            return true;
        }
        if (!validatePawnMove(
                piece,
                fromRow,
                fromCol,
                toRow,
                toCol))
        {
            return false;
        }
    }

    PieceType type = piece->getType();

    if (type == ROOK ||
        type == BISHOP ||
        type == QUEEN)
    {
        if (!isPathClear(
                fromRow,
                fromCol,
                toRow,
                toCol)) return false;
    }

    Piece* captured =
        board[toRow][toCol];

    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;

    if (piece->getType() == PAWN &&
        fromCol != toCol &&
        target == nullptr)
    {
        delete board[fromRow][toCol];

        board[fromRow][toCol] = nullptr;
    }

    bool check =
        isKingInCheck(piece->isWhite());

    board[fromRow][fromCol] = piece;
    board[toRow][toCol] = captured;

    if (check)
        return false;

    return true;
}

bool Board::isKingInCheck(bool whiteKing) {

    int kingRow = -1;
    int kingCol = -1;

    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < 8; j++) {

            if (board[i][j] == nullptr)
                continue;

            string symbol = board[i][j]->getSymbol();

            if (whiteKing && symbol == "♔") {
                kingRow = i;
                kingCol = j;
            }

            if (!whiteKing && symbol == "♚") {
                kingRow = i;
                kingCol = j;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board[i][j];
            if (piece == nullptr) continue;
            if (piece->isWhite() == whiteKing) continue;
            if (piece->canMove(
                    i,
                    j,
                    kingRow,
                    kingCol))
            {
                bool canAttack = false;

                if (piece->getType() == PAWN) {
                    int direction;
                    if (piece->isWhite()) direction = -1;
                    else direction = 1;
                    if (kingRow == i + direction && abs(kingCol - j) == 1) canAttack = true;
                } else {
                    canAttack = piece->canMove(i,j,kingRow,kingCol);
                }
                if (canAttack) {
                PieceType type = piece->getType();
                if (type == ROOK || type == BISHOP || type == QUEEN) 
                    if (!isPathClear(i,j,kingRow,kingCol))continue;

                return true;
                }
            }
        }
    }

    return false;
}

void Board::setDifficulty(int depth) {
    aiDepth = depth;
}

bool Board::movePiece(
    string from,
    string to
) {
    int fromRow, fromCol;
    int toRow, toCol;

    parsePosition(from, fromRow, fromCol);
    parsePosition(to, toRow, toCol);

    if (!isLegalMove(
            fromRow,
            fromCol,
            toRow,
            toCol))
    {
        cout << "Неверный ход!\n";
        return false;
    }
    Piece* piece = board[fromRow][fromCol];
    Piece* target = board[toRow][toCol];

    if (target != nullptr) {
        if (target->isWhite()) {
            capturedWhite[capturedWhiteCount] = target->getSymbol();
            capturedWhiteCount++;
        } else {
            capturedBlack[capturedBlackCount] = target->getSymbol();
            capturedBlackCount++;
        }
    }
    bool castling = false;
    if (piece->getType() == KING && abs(toCol - fromCol) == 2)castling = true;

    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;

    if (piece->getType() == PAWN &&
        fromCol != toCol &&
        target == nullptr)
    {
        delete board[fromRow][toCol];
        board[fromRow][toCol] = nullptr;
    }

    if (castling) {
        if (toCol == 6) {
            board[toRow][5] =
                board[toRow][7];
            board[toRow][7] = nullptr;
            board[toRow][5]->setMoved(true);
        }
        if (toCol == 2) {
            board[toRow][3] =
                board[toRow][0];
            board[toRow][0] = nullptr;
            board[toRow][3]->setMoved(true);
        }
    }

    promotePawn(toRow, toCol);

    piece->setMoved(true);

    string moveText = from + " " + to;

    moveHistory[moveCount] = moveText;

    moveCount++;

    lastFromRow = fromRow;
    lastFromCol = fromCol;

    lastToRow = toRow;
    lastToCol = toCol;

    whiteTurn = !whiteTurn;

    return true;
}

bool Board::isCheckmate(bool whiteKing) {
    if (!isKingInCheck(whiteKing)) return false;

    bool currentTurn = whiteTurn;

    whiteTurn = whiteKing;

    bool hasMoves = hasAnyMoves(whiteKing);

    whiteTurn = currentTurn;

    return !hasMoves;
}

void Board::promotePawn(
    int row,
    int col
) {
    Piece* piece = board[row][col];
    if (piece == nullptr) return;

    if (piece->getType() != PAWN) return;

    bool whitePromotion = piece->isWhite() && row == 0;

    bool blackPromotion = !piece->isWhite() && row == 7;
    if (!whitePromotion && !blackPromotion) return;

    cout << "\nПревращение пешки!\n";
    cout << "Вsберите фигуру:\n";
    cout << "q - ферзь\n";
    cout << "r - ладья\n";
    cout << "b - слон\n";
    cout << "n - конь\n";

    char choice;
    cin >> choice;

    bool white = piece->isWhite();
    delete board[row][col];
    switch (choice) {
        case 'q':
            board[row][col] =
                new Queen(white);
            break;
        case 'r':
            board[row][col] =
                new Rook(white);
            break;
        case 'b':
            board[row][col] =
                new Bishop(white);
            break;
        case 'n':
            board[row][col] =
                new Knight(white);
            break;
        default:
            board[row][col] =
                new Queen(white);
            break;
    }
}


bool Board::hasAnyMoves(bool whitePlayer) {
    bool currentTurn = whiteTurn;
    whiteTurn = whitePlayer;
    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            Piece* piece =
                board[fromRow][fromCol];
            if (piece == nullptr)
                continue;
            if (piece->isWhite() != whitePlayer)
                continue;
            for (int toRow = 0; toRow < 8; toRow++) {
                for (int toCol = 0; toCol < 8; toCol++) {
                    if (isLegalMove(
                            fromRow,
                            fromCol,
                            toRow,
                            toCol))
                    {
                        whiteTurn = currentTurn;
                        return true;
                    }
                }
            }
        }
    }

    whiteTurn = currentTurn;

    return false;
}

bool Board::isStalemate(bool whitePlayer) {
    if (isKingInCheck(whitePlayer))
        return false;
    return !hasAnyMoves(whitePlayer);
}

int Board::evaluateBoard() {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board[i][j];
            if (piece == nullptr)
                continue;

            int value = 0;
            switch (piece->getType()) {
                case PAWN:
                    value = 1;
                    break;
                case KNIGHT:
                    value = 3;
                    break;
                case BISHOP:
                    value = 3;
                    break;
                case ROOK:
                    value = 5;
                    break;
                case QUEEN:
                    value = 9;
                    break;
                case KING:
                    value = 1000;
                    break;
            }

            if (piece->isWhite())
                score += value;
            else
                score -= value;
        }
    }

    return score;
}


void Board::makeBestMove() {
    Move moves[500];

    int count =
        generateMoves(false, moves);

    if (count == 0)
        return;

    int bestScore = 999999;
    int bestIndex = 0;

    for (int i = 0; i < count; i++) {
        Move move = moves[i];
        Piece* piece =
            board[move.fromRow]
                 [move.fromCol];
        Piece* captured =
            board[move.toRow]
                 [move.toCol];

        board[move.toRow]
             [move.toCol] = piece;
        board[move.fromRow]
             [move.fromCol] = nullptr;

        int score =
            minimax(
                aiDepth,
                true,
                -999999,
                999999
            );

        board[move.fromRow]
             [move.fromCol] = piece;
        board[move.toRow]
             [move.toCol] = captured;


        if (score < bestScore) {
            bestScore = score;
            bestIndex = i;
        }
    }

    Move bestMove =
        moves[bestIndex];
    string from = "";
    from += ('a' + bestMove.fromCol);

    from += ('8' - bestMove.fromRow);
    string to = "";
    to += ('a' + bestMove.toCol);

    to += ('8' - bestMove.toRow);

    cout << "\nКомпьютер ходит: "
         << from
         << " "
         << to
         << "\n";

    movePiece(from, to);
}

int Board::minimax(
    int depth,
    bool maximizingPlayer,
    int alpha,
    int beta
) {
    if (isCheckmate(true))
        return -100000;
    if (isCheckmate(false))
        return 100000;
    if (isStalemate(true) ||
        isStalemate(false))
    {
        return 0;
    }

    if (depth == 0) return evaluateBoard();


    if (maximizingPlayer) {
        int bestScore = -999999;
        Move moves[500];
        int count = generateMoves(true, moves);

        for (int i = 0; i < count; i++) {
            Move move = moves[i];
            Piece* piece =
                board[move.fromRow]
                     [move.fromCol];

            Piece* captured =
                board[move.toRow]
                     [move.toCol];
            board[move.toRow]
                 [move.toCol] = piece;
            board[move.fromRow]
                 [move.fromCol] = nullptr;

            int score =
                minimax(
                    depth - 1,
                    false,
                    alpha,
                    beta
                );
            board[move.fromRow]
                 [move.fromCol] = piece;

            board[move.toRow]
                 [move.toCol] = captured;

            if (score > bestScore)
                bestScore = score;

            if (score > alpha)
                alpha = score;

            if (beta <= alpha)
                break;
        }

        return bestScore;
    } else {
        int bestScore = 999999;
        Move moves[500];
        int count =
            generateMoves(false, moves);
        for (int i = 0; i < count; i++) {
            Move move = moves[i];
            Piece* piece =
                board[move.fromRow]
                     [move.fromCol];
            Piece* captured =
                board[move.toRow]
                     [move.toCol];

            board[move.toRow]
                 [move.toCol] = piece;

            board[move.fromRow]
                 [move.fromCol] = nullptr;

            int score =
                minimax(
                    depth - 1,
                    true,
                    alpha,
                    beta
                );
            board[move.fromRow]
                 [move.fromCol] = piece;

            board[move.toRow]
                 [move.toCol] = captured;

            if (score < bestScore)
                bestScore = score;

            if (score < beta)
                beta = score;

            if (beta <= alpha)
                break;
        }

        return bestScore;
    }
}

void Board::gameLoopPvE() {
    while (true) {
        print();
        if (whiteTurn)
            cout << "\nХод белых\n";
        else
            cout << "\nХод черных\n";

        if (whiteTurn) {
            string from, to;
            cout << "Введите ход: ";
            cin >> from >> to;
            if (from == "save") {
                saveGame(to);
                continue;
            }

            if (from == "load") {
                loadGame(to);
                continue;
            }
            if (!movePiece(from, to)) {
                cout << "Неверный ход!\n";
                continue;
            }
        } else makeBestMove();


        if (isCheckmate(whiteTurn)) {
            print();
            if (whiteTurn) cout << "\nМАТ БЕЛЫМ!\n";
            else cout << "\nМАТ ЧЕРНЫМ!\n";
            break;
        }

        if (isStalemate(whiteTurn)) {
            print();
            cout << "\nПАТ! Ничья!\n";
            break;
        }
    }
}

void Board::gameLoopPvP() {
    while (true) {
        print();
        if (whiteTurn) cout << "\nХод белых\n";
        else cout << "\nХод черных\n";

        string from, to;
        cout << "Введите ход: ";
        cin >> from >> to;

        if (from == "save") {
            saveGame(to);
            continue;
        }

        if (from == "load") {
            loadGame(to);
            continue;
        }

        if (!movePiece(from, to)) {
            cout << "Неверный ход!\n";
            continue;
        }

        if (isCheckmate(whiteTurn)) {
            print();
            if (whiteTurn) cout << "\nМАТ БЕЛЫМ!\n";
            else cout << "\nМАТ ЧЕРНЫМ!\n";
            break;
        }

        if (isStalemate(whiteTurn)) {
            print();
            cout << "\nПАТ!\n";
            break;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
        }
    }
}