#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Player.h"
#include <windows.h>

class Game {
public:
    Game(int mapWidth, int mapHeight);

    void run();
    void showIntroMenu(); // ��Ʈ�� �޴� �Լ� �߰�

private:
    void processInput();
    void update();
    void render();
    void displayCurrentFloor();
    void resetGame();

    Map map;
    Player player;
    int currentFloor;
    int score;
    int minPathLength;

    // Windows �ܼ� ����
    HANDLE hConsole;
    CHAR_INFO* consoleBuffer;
    COORD bufferSize;
    COORD bufferCoord;
    SMALL_RECT writeRegion;
};

#endif