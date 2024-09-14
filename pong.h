#ifndef PONG_H
#define PONG_H


# define ARROW_UP		0x41
# define ARROW_DOWN		0x42
# define ARROW_RIGHT		0x43
# define ARROW_LEFT		0x44

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
