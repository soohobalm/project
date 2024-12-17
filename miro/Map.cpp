#include "Map.h"
#include "Utils.h"
#include <random>
#include <stack>
#include <queue>
#include <algorithm>
#include <tuple>

Map::Map(int width, int height) : width(width), height(height), tiles(height, std::vector<Tile>(width)) {
    // ���� �׵θ��� ������ ä��ϴ�.
    for (int x = 0; x < width; ++x) {
        tiles[0][x] = Tile(TileType::WALL);
        tiles[height - 1][x] = Tile(TileType::WALL);
    }
    for (int y = 0; y < height; ++y) {
        tiles[y][0] = Tile(TileType::WALL);
        tiles[y][width - 1] = Tile(TileType::WALL);
    }
}

void Map::generate() {
    // ���� �˰����� ���� ���� �ʱ�ȭ
    numSet.assign(height, std::vector<int>(width, 0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            numSet[i][j] = i * width + j;
        }
    }

    // ���� ������ �ʱ�ȭ
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles[y][x] = Tile(TileType::WALL);
        }
    }

    // ���� �˰��� ����
    for (int i = 0; i < height; ++i) {
        // ���� �� ����
        for (int j = 0; j < width - 1; ++j) {
            if (i == height - 1) {
                // ������ �࿡���� ������ �� ����
                if (numSet[i][j] != numSet[i][j + 1]) {
                    tiles[i * 2 + 1][j * 2 + 2] = Tile(TileType::PATH);
                    // ���� ������Ʈ
                    int oldSet = numSet[i][j + 1];
                    for (int k = 0; k < width; ++k) {
                        if (numSet[i][k] == oldSet) {
                            numSet[i][k] = numSet[i][j];
                        }
                    }
                }
            }
            else {
                // �����ϰ� �� ���� ���� ����
                if (rand() % 2 == 0 && numSet[i][j] != numSet[i][j + 1]) {
                    tiles[i * 2 + 1][j * 2 + 2] = Tile(TileType::PATH);
                    // ���� ������Ʈ
                    int oldSet = numSet[i][j + 1];
                    for (int k = 0; k < width; ++k) {
                        if (numSet[i][k] == oldSet) {
                            numSet[i][k] = numSet[i][j];
                        }
                    }
                }
            }
        }

        if (i == height - 1) break;

        // ���� �� ����
        for (int j = 0; j < width;) {
            int start = j;
            while (j < width && numSet[i][start] == numSet[i][j]) {
                ++j;
            }
            int end = j - 1;

            // �ּ� �ϳ� �̻��� ���� �� ����
            int removeCount = 1 + rand() % (end - start + 1);
            for (int k = 0; k < removeCount; ++k) {
                int removePos = start + rand() % (end - start + 1);
                tiles[i * 2 + 2][removePos * 2 + 1] = Tile(TileType::PATH);
                numSet[i + 1][removePos] = numSet[i][removePos];
            }
        }
    }
}

void Map::placePlayerAndStair(int playerX, int playerY) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribX(1, width - 2);
    std::uniform_int_distribution<> distribY(1, height - 2);

    // �÷��̾� ��ġ �缳�� (�̹� ��ġ�� �÷��̾� �����)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (tiles[y][x].getType() == TileType::PLAYER) {
                tiles[y][x] = Tile(TileType::PATH);
            }
        }
    }

    tiles[playerY][playerX] = Tile(TileType::PLAYER);

    // ��� ��ġ ���� (�÷��̾�� ��ġ�� �ʵ���)
    int stairX, stairY;
    do {
        stairX = distribX(gen);
        stairY = distribY(gen);
    } while (tiles[stairY][stairX].getType() != TileType::PATH || (stairX == playerX && stairY == playerY));

    tiles[stairY][stairX] = Tile(TileType::STAIR);
}

void Map::resetPlayerAndGoal() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribX(0, width - 1);
    std::uniform_int_distribution<> distribY(0, height - 1);

    // �� ���� �÷��̾�� ��ǥ ����, ������ �� �ʱ�ȭ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (tiles[y][x].getType() == TileType::PLAYER || tiles[y][x].getType() == TileType::GOAL || tiles[y][x].getType() == TileType::VISITED) {
                tiles[y][x] = Tile(TileType::PATH);
            }
        }
    }

    // �÷��̾� ��ġ ���� (�������� (0, 0))
    tiles[0][0] = Tile(TileType::PLAYER);

    // ��ǥ(��) ��ġ ����
    int goalX, goalY;
    do {
        goalX = distribX(gen);
        goalY = distribY(gen);
    } while (goalX == 0 && goalY == 0); // �÷��̾�� ���� ��ġ�� �������� �ʵ��� ��

    tiles[goalY][goalX] = Tile(TileType::GOAL);
    shortestPath.clear();
}

int Map::bfs() {
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<std::tuple<int, int, int>> q; // x, y, �Ÿ�
    std::vector<std::vector<std::pair<int, int>>> prev(height, std::vector<std::pair<int, int>>(width, { -1, -1 }));

    // �÷��̾� ��ġ ã��
    int startX = -1, startY = -1;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tiles[y][x].getType() == TileType::PLAYER) {
                startX = x;
                startY = y;
                break;
            }
        }
        if (startX != -1) break;
    }

    q.push({ startX, startY, 0 });
    visited[startY][startX] = true;

    int dx[] = { 0, 0, 1, -1 };
    int dy[] = { 1, -1, 0, 0 };
    int goalX = -1, goalY = -1;

    while (!q.empty()) {
        int x, y, dist;
        std::tie(x, y, dist) = q.front();
        q.pop();

        if (tiles[y][x].getType() == TileType::GOAL) {
            goalX = x;
            goalY = y;

            // �ִ� ��� ����
            while (goalX != -1 && goalY != -1) {
                shortestPath.push_back({ goalX, goalY });
                int tempX = prev[goalY][goalX].first;
                int tempY = prev[goalY][goalX].second;
                goalX = tempX;
                goalY = tempY;
            }
            std::reverse(shortestPath.begin(), shortestPath.end());

            return dist;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height && !visited[ny][nx] && tiles[ny][nx].getType() != TileType::WALL) {
                visited[ny][nx] = true;
                q.push({ nx, ny, dist + 1 });
                prev[ny][nx] = { x, y };
            }
        }
    }

    return -1; // ��ǥ�� ã�� ���� ���
}

void Map::showPath(int pathLength) {
    if (pathLength == -1) return;

    for (int i = 1; i < shortestPath.size() - 1; ++i) {
        int x = shortestPath[i].first;
        int y = shortestPath[i].second;
        tiles[y][x].setType(TileType::VISITED);
    }
}

bool Map::isValidMove(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height && tiles[y][x].getType() != TileType::WALL;
}

Tile& Map::getTile(int x, int y) {
    return tiles[y][x];
}

const Tile& Map::getTile(int x, int y) const {
    return tiles[y][x];
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}