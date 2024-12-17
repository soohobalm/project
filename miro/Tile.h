#ifndef TILE_H
#define TILE_H

#include <windows.h>

// 맵 요소 타입
enum class TileType {
    WALL,
    PATH,
    PLAYER,
    STAIR,
    GOAL,
    VISITED
};

// 맵 요소 정의 (wchar_t 사용)
const wchar_t WALL_CHAR = L'█';
const int WALL_COLOR = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const wchar_t PATH_CHAR = L' ';
const int PATH_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
const wchar_t PLAYER_CHAR = L'●';
const int PLAYER_COLOR = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const wchar_t STAIR_CHAR = L'>';
const int STAIR_COLOR = FOREGROUND_RED | FOREGROUND_INTENSITY;
const wchar_t GOAL_CHAR = L'#';
const int GOAL_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const wchar_t VISITED_CHAR = L'*';
const int VISITED_COLOR = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

class Tile {
public:
    Tile();
    Tile(TileType type);

    TileType getType() const;
    void setType(TileType newType);
    wchar_t getChar() const;
    int getColor() const;

private:
    TileType type;
};

#endif