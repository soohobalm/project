#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <conio.h>

Game::Game(int mapWidth, int mapHeight) : map(mapWidth, mapHeight), player(0, 0), currentFloor(1), score(0), minPathLength(0) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bufferSize = { (SHORT)mapWidth, (SHORT)mapHeight };
    bufferCoord = { 0, 0 };
    writeRegion = { 0, 0, (SHORT)(mapWidth - 1), (SHORT)(mapHeight - 1) };
    consoleBuffer = new CHAR_INFO[mapWidth * mapHeight];
}

void Game::run() {
    // 게임 시작 전 인트로 메뉴를 표시
    showIntroMenu();

    map.generate();
    map.resetPlayerAndGoal(); // 플레이어와 목표 위치 재설정
    minPathLength = map.bfs();
    map.showPath(minPathLength);

    while (true) {
        processInput();
        update();
        render();
    }
}

void Game::showIntroMenu() {
    system("cls"); // 화면 지우기 (Windows)

    std::wcout << L"============================" << std::endl;
    std::wcout << L"     던전 탐험 게임" << std::endl;
    std::wcout << L"============================" << std::endl;
    std::wcout << L"1. 게임 시작" << std::endl;
    std::wcout << L"2. 게임 종료" << std::endl;
    std::wcout << L"============================" << std::endl;
    std::wcout << L"메뉴 선택: ";

    wchar_t choice;
    do {
        choice = _getwch();
    } while (choice != L'1' && choice != L'2');

    if (choice == L'1') {
        // 게임 시작
        system("cls"); // 화면 지우기
    }
    else {
        // 게임 종료
        delete[] consoleBuffer;
        exit(0);
    }
}

void Game::processInput() {
    wchar_t input = _getwch();
    if (input == L'q') {
        delete[] consoleBuffer;
        exit(0);
    }

    int dx = 0, dy = 0;
    switch (input) {
    case L'w': dy = -1; break;
    case L's': dy = 1; break;
    case L'a': dx = -1; break;
    case L'd': dx = 1; break;
    }

    int newX = player.getX() + dx;
    int newY = player.getY() + dy;

    if (map.isValidMove(newX, newY)) {
        TileType nextTileType = map.getTile(newX, newY).getType();

        if (nextTileType == TileType::GOAL) {
            // 게임 클리어 처리
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY, hConsole);

            // 게임 클리어 메시지
            Move((map.getWidth() / 2) - 5, map.getHeight() / 2);
            std::wcout << L"Game Clear!";
            Move((map.getWidth() / 2) - 5, (map.getHeight() / 2) + 1);
            std::wcout << L"Score: " << score;

            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);
            Sleep(2000);
            resetGame(); // 게임 재시작
        }
        else if (nextTileType == TileType::STAIR) {
            // 계단을 밟았을 때의 처리
            currentFloor++;
            map.generate();
            player.setX(0);
            player.setY(0);
            map.placePlayerAndStair(player.getX(), player.getY());

            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, hConsole);
            COORD floorPos = { (SHORT)(map.getWidth() + 2), 0 };
            SetConsoleCursorPosition(hConsole, floorPos);
            std::wcout << L"Floor: " << currentFloor << L" ";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);

            COORD cursorPos = { 0, 0 };
            SetConsoleCursorPosition(hConsole, cursorPos);
            for (int i = 0; i < map.getHeight(); i++) {
                for (int j = 0; j < map.getWidth(); j++) {
                    std::wcout << L" ";
                }
                if (i < map.getHeight() - 1) std::wcout << std::endl;
            }

        }
        else {
            // 플레이어 이동 처리
            if (dx == 0 && dy == -1) {
                map.getTile(player.getX(), player.getY()).setType(TileType::VISITED);
            }
            else if (dx == 0 && dy == 1) {
                map.getTile(player.getX(), player.getY()).setType(TileType::VISITED);
            }
            else if (dx == -1 && dy == 0) {
                map.getTile(player.getX(), player.getY()).setType(TileType::VISITED);
            }
            else if (dx == 1 && dy == 0) {
                map.getTile(player.getX(), player.getY()).setType(TileType::VISITED);
            }

            player.move(dx, dy);
            map.getTile(player.getX(), player.getY()).setType(TileType::PLAYER);
            score++;
        }
    }
    else {
        // 이동할 수 없는 경우 처리
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY, hConsole);
        Move(0, map.getHeight());
        std::wcout << L"\r막다른 길입니다!";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);
        Sleep(500);

        // 원래 위치로 커서 이동
        COORD cursorPos = { 0, 0 };
        SetConsoleCursorPosition(hConsole, cursorPos);

        // 막다른 길 메시지를 지우기 위해 공백 출력
        for (int i = 0; i < map.getWidth(); ++i) {
            std::wcout << L" ";
        }

        // 다시 원래 위치로 커서 이동
        SetConsoleCursorPosition(hConsole, cursorPos);
    }
}

void Game::update() {
    // 게임 상태 업데이트 (현재는 특별히 할 것 없음)

    // 현재 층수 및 스코어, 최단 경로 길이 출력 (오른쪽 상단)
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, hConsole);
    COORD scorePos = { (SHORT)(map.getWidth() + 2), 1 };
    SetConsoleCursorPosition(hConsole, scorePos);
    std::wcout << L"Score: " << score << L" ";

    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, hConsole);
    COORD pathPos = { (SHORT)(map.getWidth() + 2), 2 };
    SetConsoleCursorPosition(hConsole, pathPos);
    std::wcout << L"Shortest Path: " << minPathLength << L" ";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);
}

void Game::render() {
    // 이중 버퍼링을 사용하여 맵 그리기
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            int index = y * map.getWidth() + x;
            consoleBuffer[index].Char.UnicodeChar = map.getTile(x, y).getChar();
            consoleBuffer[index].Attributes = map.getTile(x, y).getColor();
        }
    }

    WriteConsoleOutput(hConsole, consoleBuffer, bufferSize, bufferCoord, &writeRegion);

    // 커서 위치를 (0, 0)으로 이동
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    // 현재 층수 및 스코어 출력
    displayCurrentFloor();
}

void Game::displayCurrentFloor() {
    // 현재 층 출력 (오른쪽 상단)
    setConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, hConsole);
    COORD floorPos = { (SHORT)(map.getWidth() + 2), 0 };
    SetConsoleCursorPosition(hConsole, floorPos);
    std::wcout << L"Floor: " << currentFloor << L" ";
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);
}

void Game::resetGame() {
    map.generate();
    map.resetPlayerAndGoal();
    player.setX(0);
    player.setY(0);
    score = 0;
    minPathLength = map.bfs();
    map.showPath(minPathLength);
}