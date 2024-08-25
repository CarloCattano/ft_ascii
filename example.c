#include "ftascii.h"
#include <signal.h>
#include <locale.h>
#include <time.h>

#define PADDING 2

#define SCORE_Y 8
#define SCORE_X 6

static term_t *term_pointer;

static void handlectrl_c(int sig) {
    (void)sig;
    systemExit(term_pointer);
}

static void initializeTerm(term_t *term)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    srand(time(NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    system("stty -echo -icanon -icrnl time 0 min 0");

    write(1, NOMOUSE, 6);  // Hide cursor
    write(1, CLEAR, 4); // Clear screen
    write(1, CURSOR, 6); 

    *term = (term_t){w.ws_col, w.ws_row, w.ws_col * w.ws_row, NULL, NULL, NULL, 1, 1, 50,0,{0}};
    term_pointer = term;

    init_term(term);
    signal(SIGINT, handlectrl_c);
}

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

static void initGame(struct ball *ball, struct player *player1, struct player *player2) {
    ball->y = term_pointer->MAX_ROW / 2;
    ball->x = term_pointer->MAX_COL / 2;
    ball->dx = 1;
    ball->dy = 1;
    ball->vel = 1;

    player1->score = 0;
    player1->paddle.x = 4;
    player1->paddle.y = term_pointer->MAX_ROW / 2;


    player2->score = 0;
    player2->paddle.x = term_pointer->MAX_COL - 4;
    player2->paddle.y = term_pointer->MAX_ROW / 2;
}

void drawPlayer(term_t *term, struct player *player) {
    
    if (player->paddle.y < PADDING) {
        player->paddle.y = PADDING;
    } else if (player->paddle.y > term->MAX_ROW - PADDING - 2) {
        player->paddle.y = term->MAX_ROW - PADDING - 2;
    }

    map_pix(term, player->paddle.x, player->paddle.y, GREEN, "▓");
    map_pix(term, player->paddle.x, player->paddle.y + 1, GREEN, "▓");
    map_pix(term, player->paddle.x, player->paddle.y + 2, GREEN, "▓");
    map_pix(term, player->paddle.x, player->paddle.y + 3, GREEN, "▓");
}

void drawBall(term_t *term, struct ball *ball) {
    map_pix(term, ball->x, ball->y, RED, "▓");
}

void moveBall(struct ball *ball) {
    
    ball->x += ball->dx * ball->vel;
    ball->y += ball->dy * ball->vel;

    if (ball->y >= term_pointer->MAX_ROW - PADDING || ball->y <= PADDING) {
        ball->dy = -ball->dy;
    }

    if (ball->x >= term_pointer->MAX_COL - PADDING || ball->x <= PADDING) {
        ball->dx = -ball->dx;
    }

    if ((ball->x == player1.paddle.x && ball->y == player1.paddle.y) || 
        (ball->x == player1.paddle.x && ball->y == player1.paddle.y + 1) ||
        (ball->x == player1.paddle.x && ball->y == player1.paddle.y + 2) ||
        (ball->x == player1.paddle.x && ball->y == player1.paddle.y + 3)) {
        ball->dx = -ball->dx;
    }

    if ((ball->x == player2.paddle.x && ball->y == player2.paddle.y)
        || (ball->x == player2.paddle.x && ball->y == player2.paddle.y + 1)
            || (ball->x == player2.paddle.x && ball->y == player2.paddle.y + 2) || 
        (ball->x == player2.paddle.x && ball->y == player2.paddle.y + 3)){

        ball->dx = -ball->dx;
    } 

    if (ball->x == PADDING) {
        player2.score++;
        usleep(1000000);
        ball->x = term_pointer->MAX_COL / 2;
        ball->y = term_pointer->MAX_ROW / 2;
    } else if (ball->x == term_pointer->MAX_COL - PADDING) {
        player1.score++;
        usleep(1000000);
        ball->x = term_pointer->MAX_COL / 2;
        ball->y = term_pointer->MAX_ROW / 2;
    }
}

void drawScore(term_t *term) {
    char (*digits[10])[SCORE_Y] = {zero, one, two, three, four, five, six, seven, eight, nine};

    // Extract tens and units digits for each player's score
    int player1_tens = player1.score / 10;
    int player1_units = player1.score % 10;
    int player2_tens = player2.score / 10;
    int player2_units = player2.score % 10;

    char (*player1_tens_digit)[SCORE_Y] = digits[player1_tens];
    char (*player1_units_digit)[SCORE_Y] = digits[player1_units];

    char (*player2_tens_digit)[SCORE_Y] = digits[player2_tens];
    char (*player2_units_digit)[SCORE_Y] = digits[player2_units];

    if (player1.score < 10) {
        // Draw only the units digit
        for (int i = 0; i < SCORE_X; i++) {
            for (int j = 0; j < SCORE_Y; j++) {
                if (player1_units_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL / 6 + j, 2 + i, GREEN, "▓");
                }
            }
        }
    } else {
        // Draw both tens and units digits
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < SCORE_Y; j++) {
                if (player1_tens_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL / 6 + j, 2 + i, GREEN, "▓");
                }
                if (player1_units_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL / 6 + j + 12, 2 + i, GREEN, "▓"); // Offset by 12 to draw the next digit
                }
            }
        }
    }

    if (player2.score < 10) {
        for (int i = 0; i < SCORE_X; i++) {
            for (int j = 0; j < SCORE_Y; j++) {
                if (player2_units_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL - term->MAX_COL / 3 + j, 2 + i, GREEN, "▓");
                }
            }
        }
    } else {
        for (int i = 0; i < SCORE_X; i++) {
            for (int j = 0; j < SCORE_Y; j++) {
                if (player2_tens_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL - term->MAX_COL / 3 + j, 2 + i, GREEN, "▓");
                }
                if (player2_units_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL - term->MAX_COL / 3 + j + 12, 2 + i, GREEN, "▓"); // Offset by 12 to draw the next digit
                }
            }
        }
    }
}

void draw_callback(term_t *term)
{
    for (int i = 0; i < term->MAX_ROW; i++) {
        if ((i + 1) % 2 == 0)
            map_pix(term, term->MAX_COL / 2, i, GREEN, "▓");
    }

    drawPlayer(term, &player1);
    drawPlayer(term, &player2);
    drawScore(term);
    drawBall(term, &ball);
}

void KeyPress(char key, term_t *term) {
    switch (key) {
        case 'c':
            term->clear = !term->clear;
            break;
        case 'w':
            player1.paddle.y -= 2;
            break;
        case 's':
            player1.paddle.y += 2;
            break;
        case 'q':
            systemExit(term);
            break;
        default:
            break;
    }
}

void keyhooks(term_t *term) 
{
    char key;
    if (read(STDIN_FILENO, &key, 1) == 1) {
	    KeyPress(key, term);
    } 
}

int main()
{
    term_t *term = malloc(sizeof(term_t));

    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }

    initializeTerm(term);

    initGame(&ball, &player1, &player2);

    while (term->draw) {
        keyhooks(term);
        if(term->frame % 2 == 0) {
            moveBall(&ball);
        }
        draw(term, &draw_callback);
        usleep(term->delay);
    }
    systemExit(term);
    return 0;
}

