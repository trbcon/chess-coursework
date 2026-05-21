#include <iostream>
#include <ctime>
#include <cstdlib>

#include "board.h"

using namespace std;

int main() {

    srand(time(0));

    Board board;

    int choice;

    cout << "=====================\n";
    cout << "      CHESS\n";
    cout << "=====================\n\n";

    cout << "1. PvP\n";
    cout << "2. PvE\n";
    cout << "3. Выход\n\n";

    cout << "Выберите режим: ";

    cin >> choice;

    switch (choice) {
        case 1:
            board.gameLoopPvP();
            break;
        case 2:
            int difficulty;
            cout << "\nСложность:\n";
            cout << "1. Easy\n";
            cout << "2. Medium\n";
            cout << "3. Hard\n\n";

            cout << "Выберите: ";
            cin >> difficulty;
            switch (difficulty) {
                case 1:
                    board.setDifficulty(1);
                    break;
                case 2:
                    board.setDifficulty(2);
                    break;
                case 3:
                    board.setDifficulty(30);
                    break;
                default:
                    board.setDifficulty(2);
            }
            board.gameLoopPvE();
            break;
        case 3:
            return 0;
        default:
            cout << "Ошибка!\n";
    }

    return 0;
}