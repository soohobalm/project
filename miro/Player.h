#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player(int x, int y);

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);

    void move(int dx, int dy);

private:
    int x;
    int y;
};

#endif