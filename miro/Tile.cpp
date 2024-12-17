#include "Tile.h"

Tile::Tile() : type(TileType::PATH) {}

Tile::Tile(TileType type) : type(type) {}

TileType Tile::getType() const {
    return type;
}

void Tile::setType(TileType newType) {
    type = newType;
}

wchar_t Tile::getChar() const {
    switch (type) {
    case TileType::WALL:
        return WALL_CHAR;
    case TileType::PATH:
        return PATH_CHAR;
    case TileType::PLAYER:
        return PLAYER_CHAR;
    case TileType::STAIR:
        return STAIR_CHAR;
    case TileType::GOAL:
        return GOAL_CHAR;
    case TileType::VISITED:
        return VISITED_CHAR;
    default:
        return L' ';
    }
}

int Tile::getColor() const {
    switch (type) {
    case TileType::WALL:
        return WALL_COLOR;
    case TileType::PATH:
        return PATH_COLOR;
    case TileType::PLAYER:
        return PLAYER_COLOR;
    case TileType::STAIR:
        return STAIR_COLOR;
    case TileType::GOAL:
        return GOAL_COLOR;
    case TileType::VISITED:
        return VISITED_COLOR;
    default:
        return PATH_COLOR;
    }
}