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
    // ���� ���� �� ��Ʈ�� �޴��� ǥ��
    showIntroMenu();

    map.generate();
    map.resetPlayerAndGoal(); // �÷��̾�� ��ǥ ��ġ �缳��
    minPathLength = map.bfs();
    map.showPath(minPathLength);

    while (true) {
        processInput();
        update();
        render();
    }
}

void Game::showIntroMenu() {
    system("cls"); // ȭ�� ����� (Windows)

    std::wcout << L"============================" << std::endl;
    std::wcout << L"     ���� Ž�� ����" << std::endl;
    std::wcout << L"============================" << std::endl;
    std::wcout << L"1. ���� ����" << std::endl;
    std::wcout << L"2. ���� ����" << std::endl;
    std::wcout << L"============================" << std::endl;
    std::wcout << L"�޴� ����: ";

    wchar_t choice;
    do {
        choice = _getwch();
    } while (choice != L'1' && choice != L'2');

    if (choice == L'1') {
        // ���� ����
        system("cls"); // ȭ�� �����
    }
    else {
        // ���� ����
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
            // ���� Ŭ���� ó��
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY, hConsole);

            // ���� Ŭ���� �޽���
            Move((map.getWidth() / 2) - 5, map.getHeight() / 2);
            std::wcout << L"Game Clear!";
            Move((map.getWidth() / 2) - 5, (map.getHeight() / 2) + 1);
            std::wcout << L"Score: " << score;

            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);
            Sleep(2000);
            resetGame(); // ���� �����
        }
        else if (nextTileType == TileType::STAIR) {
            // ����� ����� ���� ó��
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
            // �÷��̾� �̵� ó��
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
        // �̵��� �� ���� ��� ó��
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY, hConsole);
        Move(0, map.getHeight());
        std::wcout << L"\r���ٸ� ���Դϴ�!";
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, hConsole);
        Sleep(500);

        // ���� ��ġ�� Ŀ�� �̵�
        COORD cursorPos = { 0, 0 };
        SetConsoleCursorPosition(hConsole, cursorPos);

        // ���ٸ� �� �޽����� ����� ���� ���� ���
        for (int i = 0; i < map.getWidth(); ++i) {
            std::wcout << L" ";
        }

        // �ٽ� ���� ��ġ�� Ŀ�� �̵�
        SetConsoleCursorPosition(hConsole, cursorPos);
    }
}

void Game::update() {
    // ���� ���� ������Ʈ (����� Ư���� �� �� ����)

    // ���� ���� �� ���ھ�, �ִ� ��� ���� ��� (������ ���)
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
    // ���� ���۸��� ����Ͽ� �� �׸���
    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            int index = y * map.getWidth() + x;
            consoleBuffer[index].Char.UnicodeChar = map.getTile(x, y).getChar();
            consoleBuffer[index].Attributes = map.getTile(x, y).getColor();
        }
    }

    WriteConsoleOutput(hConsole, consoleBuffer, bufferSize, bufferCoord, &writeRegion);

    // Ŀ�� ��ġ�� (0, 0)���� �̵�
    COORD cursorPos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPos);

    // ���� ���� �� ���ھ� ���
    displayCurrentFloor();
}

void Game::displayCurrentFloor() {
    // ���� �� ��� (������ ���)
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