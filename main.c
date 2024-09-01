#include "pong.h"
#include "ftascii.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <signal.h>
#include <locale.h>
#include <time.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

#define PADDING 1

#define SCORE_Y 8
#define SCORE_X 6

#define FIFO_IN "/tmp/pong_out"
#define FIFO_OUT "/tmp/pong_in"

int fd_in, fd_out;

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

static void initGame(struct ball *ball, struct player *player1, struct player *player2) {
    ball->y = term_pointer->MAX_ROW / 2;
    ball->x = term_pointer->MAX_COL / 2;
    ball->dx = 1;
    ball->dy = 1;
    ball->vel = 1;

    player1->score = 0;
    player1->paddle.x = 4;
    player1->paddle.y = term_pointer->MAX_ROW / 2;
    player1->paddle.dy = 0;
    player2->score = 0;
    player2->paddle.x = term_pointer->MAX_COL - 4;
    player2->paddle.y = term_pointer->MAX_ROW / 2;
}

void drawPlayer(term_t *term, struct player *player) {

    char* paddle = "█";

    if (player->paddle.y < PADDING) {
        player->paddle.y = PADDING;
    } else if (player->paddle.y > term->MAX_ROW - PADDING - 4) {
        player->paddle.y = term->MAX_ROW - PADDING - 4;
    }

    map_pix(term, player->paddle.x, player->paddle.y, GREEN, paddle);
    map_pix(term, player->paddle.x, player->paddle.y + 1, GREEN, paddle);
    map_pix(term, player->paddle.x, player->paddle.y + 2, GREEN, paddle);
    map_pix(term, player->paddle.x, player->paddle.y + 3, GREEN, paddle);
}

void drawBall(term_t *term, struct ball *ball) {
    map_pix(term, ball->x, ball->y, RED, "⬤");
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
                    map_pix(term, term->MAX_COL - term->MAX_COL / 4 + j, 2 + i, GREEN, "▓");
                }
            }
        }
    } else {
        for (int i = 0; i < SCORE_X; i++) {
            for (int j = 0; j < SCORE_Y; j++) {
                if (player2_tens_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL - term->MAX_COL / 4 + j, 2 + i, GREEN, "▓");
                }
                if (player2_units_digit[i][j] == '0') {
                    map_pix(term, term->MAX_COL - term->MAX_COL / 4 + j + 12, 2 + i, GREEN, "▓"); // Offset by 12 to draw the next digit
                }
            }
        }
    }
}

void draw_callback(term_t *term)
{
    // doted line in the middle
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
    player1.paddle.dy = 0;

    switch (key) {
        case 'w':
            player1.paddle.dy = 1;
            break;
        case 's':
            player1.paddle.dy = -1;
            break;
        case 'q':
            close(fd_in);
            close(fd_out);
            systemExit(term);
            break;

        case 'i':
            // change charset
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


// Background Music setup using miniaudio
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    (void)pInput;
}

// Audio thread function
void* play_audio(void* arg) {
    const char* filename = (const char*)arg;

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file(filename, NULL, &decoder);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to initialize decoder.\n");
        return NULL;
    }

    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        fprintf(stderr, "Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return NULL;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        fprintf(stderr, "Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return NULL;
    }

    while (1) {
        usleep(100000);  // Sleep to reduce CPU usage
    }

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return NULL;
}

int main() {
    pthread_t audio_thread;  // Initialize audio playback in a separate thread
    const char* audio_file = "sounds/music.wav";

    if (pthread_create(&audio_thread, NULL, play_audio, (void*)audio_file) != 0) {
        perror("Failed to create audio thread");
        return 1;
    }

    char buffer[64]; 

    if (mkfifo(FIFO_IN, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO_IN");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(FIFO_OUT, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO_OUT");
        exit(EXIT_FAILURE);
    }

    fd_in = open(FIFO_OUT, O_WRONLY); // Non-blocking mode to avoid blocking on read
    fd_out = open(FIFO_IN, O_RDONLY | O_NONBLOCK); // Non-blocking mode to avoid blocking on write

    if (fd_in < 0 || fd_out < 0) {
        perror("Error opening FIFOs");
        exit(EXIT_FAILURE);
    }

    term_t *term = malloc(sizeof(term_t));
    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }
 
    initializeTerm(term);
    initGame(&ball, &player1, &player2);

    while (term->draw) {
        draw(term, &draw_callback);
        
        keyhooks(term);
        snprintf(buffer, sizeof(buffer), "%d\n", player1.paddle.dy);

        if (write(fd_in, buffer, strlen(buffer)) == -1) {
            perror("Error writing to FIFO_OUT");
            break;
        }

        // Read ball and player2 positions from Python
        ssize_t bytes_read = read(fd_out, buffer, sizeof(buffer) - 1); // -1 for null terminator
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the buffer
            if (sscanf(buffer, "%d %d %d %d", &player1.paddle.y, &player2.paddle.y, &ball.x, &ball.y) != 4) {
                fprintf(stderr, "Error parsing data: %s\n", buffer);
            }
        } else if (bytes_read == -1 && errno != EAGAIN) {
            perror("Error reading from FIFO_IN");
            break;
        }

        usleep(term->delay);
    }

    close(fd_in);
    close(fd_out);
    
    pthread_cancel(audio_thread);
    pthread_join(audio_thread, NULL);

    systemExit(term);
    return 0;
}
