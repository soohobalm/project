#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include <fcntl.h> // _O_U16TEXT
#include <io.h>    // _setmode

int main() {
    // 유니코드 출력을 위한 설정
    _setmode(_fileno(stdout), _O_U16TEXT);

    // 콘솔 창 크기 및 제목 설정
    system("mode con:cols=85 lines=30");
    system("title [MazeGame]");

    const int MAP_WIDTH = 21; // 맵 크기 조정 (홀수로 설정)
    const int MAP_HEIGHT = 21;

    Game game(MAP_WIDTH, MAP_HEIGHT);
    game.run(); // 인트로 메뉴가 포함된 게임 시작

    return 0;
}