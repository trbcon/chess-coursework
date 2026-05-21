#ifndef MOVE_H
#define MOVE_H

#include <string>

using namespace std;

struct Move {
    int fromRow;
    int fromCol;

    int toRow;
    int toCol;

    string text;
};

#endif