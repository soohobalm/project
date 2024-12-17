#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Tile.h"

class Map {
public:
    Map(int width, int height);

    void generate();
    void placePlayerAndStair(int playerX, int playerY);
    void resetPlayerAndGoal();
    bool isValidMove(int x, int y) const;
    Tile& getTile(int x, int y);
    const Tile& getTile(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

    int bfs();
    void showPath(int pathLength);

private:
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;
    std::vector<std::vector<int>> numSet;
    std::vector<std::pair<int, int>> shortestPath;
};

#endif