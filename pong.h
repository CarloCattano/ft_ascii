#ifndef PONG_H
#define PONG_H


// pong Game objects
struct ball {
    int x;
    int y;

    int dx;
    int dy;

    int vel;
};

struct paddle {
    int x;
    int y;
    int dy;
};

struct player {
    int score;
    struct paddle paddle;
};


struct ball ball;
struct player player1;
struct player player2;


#endif
