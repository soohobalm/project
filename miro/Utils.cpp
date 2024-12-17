#include "Utils.h"

void setConsoleColor(int color, HANDLE hConsole) {
    SetConsoleTextAttribute(hConsole, color);
}

void Move(int x, int y) {
    COORD pos;
    pos.X = x * 2;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}