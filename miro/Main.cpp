#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include <fcntl.h> // _O_U16TEXT
#include <io.h>    // _setmode

int main() {
    // �����ڵ� ����� ���� ����
    _setmode(_fileno(stdout), _O_U16TEXT);

    // �ܼ� â ũ�� �� ���� ����
    system("mode con:cols=85 lines=30");
    system("title [MazeGame]");

    const int MAP_WIDTH = 21; // �� ũ�� ���� (Ȧ���� ����)
    const int MAP_HEIGHT = 21;

    Game game(MAP_WIDTH, MAP_HEIGHT);
    game.run(); // ��Ʈ�� �޴��� ���Ե� ���� ����

    return 0;
}