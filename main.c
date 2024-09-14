#include "pong.h"
#include "ftascii.h"
#include <stdint.h>

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

#define SCORE_Y 6
#define SCORE_X 6

#define FIFO_IN "/tmp/pong_out"
#define FIFO_OUT "/tmp/pong_in"

int fd_out;
int fd_in;

pthread_t audio_thread;

static term_t *term_pointer;

static void create_pipes(int *fd_in, int *fd_out) {
    if (mkfifo(FIFO_IN, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO_IN");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(FIFO_OUT, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO_OUT");
        exit(EXIT_FAILURE);
    }

    *fd_in = open(FIFO_IN, O_RDONLY);
    if (*fd_in == -1) {
        perror("Error opening FIFO_IN");
        exit(EXIT_FAILURE);
    }

    *fd_out = open(FIFO_OUT, O_WRONLY);
    if (*fd_out == -1) {
        perror("Error opening FIFO_OUT");
        exit(EXIT_FAILURE);
    }
}

static void handlectrl_c(int sig) {
    (void)sig;
    close(fd_in);
    close(fd_out);
    pthread_cancel(audio_thread);
    pthread_join(audio_thread, NULL);
    systemExit(term_pointer);
}

static void handle_sigpipe(int signal) {
    (void)signal;
}

static void setup_signal_handlers() {
    struct sigaction sa;
    sa.sa_handler = handle_sigpipe;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    sigaction(SIGPIPE, &sa, NULL);
    signal(SIGINT, handlectrl_c);
    signal(SIGTERM, handlectrl_c);
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

    *term = (term_t){   w.ws_col, w.ws_row, w.ws_col * w.ws_row, 
                        NULL, NULL, NULL, 
                        1, 1, 50,0,{0}};
    term_pointer = term;

    init_term(term);
    setup_signal_handlers();
}

static void initGame(struct ball *ball, struct player *player1, struct player *player2) {
    ball->y = term_pointer->MAX_ROW / 2;
    ball->x = term_pointer->MAX_COL / 2;
    ball->dx = 1;
    ball->dy = 1;
    ball->vel = 1;

    player1->score = 0;
    player2->score = 0;
    player1->paddle.x = 6;
    player2->paddle.x = term_pointer->MAX_COL - 8;
    player1->paddle.y = term_pointer->MAX_ROW / 2;
    player2->paddle.y = term_pointer->MAX_ROW / 2;
    player1->paddle.dy = 0;
}


void end_game(term_t *term) {
 
    close(fd_in);
    close(fd_out);
    
    pthread_cancel(audio_thread);
    pthread_join(audio_thread, NULL);
    
    usleep(3e6);
    
    write(1, CLEAR, 4);
    write(1, CURSOR, 6);
    
    system("stty echo icanon icrnl");
    system("clear");
    
    systemExit(term);
}

void drawPlayer(term_t *term, struct player *player) {

    char* paddle = "█";
    // char* paddle = "▦";

    if (player->paddle.y <= PADDING + 3) {
        player->paddle.y = PADDING + 3;
    } else if (player->paddle.y >= term->MAX_ROW - PADDING - 4) {
        player->paddle.y = term->MAX_ROW - PADDING - 4;
    }
 
    for (int i = -3; i < 5; i++) {
        map_pix(term, player->paddle.x, player->paddle.y + i, GREEN, paddle);
    }

    for (int i = -3; i < 5; i++) {
        map_pix(term, player->paddle.x + 1, player->paddle.y + i, GREEN, paddle);
    }
}

static void drawBall(term_t *term, struct ball *ball) {
    if(ball->y <= PADDING + 1) {
        ball->y = PADDING + 1;
    } else if(ball->y >= term->MAX_ROW - PADDING - 2) {
        ball->y = term->MAX_ROW - PADDING - 2;
    }

    if(ball->x <= PADDING + 1) {
        ball->x = PADDING;
    } else if(ball->x >= term->MAX_COL - PADDING - 2) {
        ball->x = term->MAX_COL - PADDING - 2;
    }

    map_pix(term, ball->x, ball->y, RED, "⬤");
}

static void drawScore(term_t *term) {
    char (*digits[12])[SCORE_Y] = {zero, one, two, three, four, five, six, seven, eight, nine, a, b};

    for (int i = 0; i < SCORE_Y; i++) {
        for (int j = 0; j < SCORE_X; j++) {
            if (digits[player1.score][i][j] == '0') {
                map_pix(term, term->MAX_COL / 6 + j, 2 + i, GREEN, "▓");
            }
            if (digits[player2.score][i][j] == '0') {
                map_pix(term, term->MAX_COL - 24 + j, 2 + i, GREEN, "▓");
            }
        }
    }
}

static void draw_callback(term_t *term)
{
    // doted line in the middle
    for (int i = 0; i < term->MAX_ROW; i++) {
        if ((i + 1) % 2 == 0)
            // map_pix(term, term->MAX_COL / 2, i, GREEN, "▓");
            map_pix(term, term->MAX_COL / 2, i, GREEN, "▏");
    }
    drawPlayer(term, &player1);
    drawPlayer(term, &player2);
    drawScore(term);
    drawBall(term, &ball);
}

static void KeyPress(char *key, term_t *term) {

    if (key[0] == '\033' && key[1] == '[') {
        if (key[2] == ARROW_UP || key[2] == ARROW_RIGHT) {
            player2.paddle.dy = 2;
        } else if (key[2] == ARROW_DOWN || key[2] == ARROW_LEFT) {
            player2.paddle.dy = 0;
        }
    } else {
        switch (key[0]) {
            case 'w':
                player2.paddle.dy = 2;
                break;
            case 's':
                player2.paddle.dy = 0;
                break;
            case 'p':
                close(fd_in);
                close(fd_out);
                pthread_cancel(audio_thread);
                pthread_join(audio_thread, NULL);
                systemExit(term);
                break;
            default:
                break;
        }
    }
}

void keyhooks(term_t *term) 
{
    char key[4];
    if (read(STDIN_FILENO, &key, 4) > 0) {
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
static void* play_audio(void* arg) {
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

unsigned long hex_to_unsigned_long(const char *hex_str) {
    return strtoul(hex_str, NULL, 16);  // Base 16 for hexadecimal
}

int main() {
    const char* audio_file = "../ft_ascii/sounds/music.wav";

    if (pthread_create(&audio_thread, NULL, play_audio, (void*)audio_file) != 0) {
        perror("Failed to create audio thread");
        return 1;
    }

    usleep(100000);  // Sleep to allow audio thread to start

    create_pipes(&fd_in, &fd_out);

    if(fd_in < 0 || fd_out < 0) {
        printf("Error opening FIFOs\n");
        printf("fd_in: %d, fd_out: %d\n", fd_in, fd_out);
        return 1;
    }

    char send_buffer[2];
    char recv_buffer[17];
    // int pipe_data[6];

    term_t *term = malloc(sizeof(term_t));
    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }
 
    initializeTerm(term);
    initGame(&ball, &player1, &player2);
 
    while (term->draw) {
        keyhooks(term);

        if(fd_in < 0 || fd_out < 0) {
            perror("Error opening FIFOs");
            return 1;
        }

        send_buffer[0] = player2.paddle.dy + '0';
        send_buffer[1] = '\n';
        
        if (write(fd_out, &send_buffer, sizeof(send_buffer)) == -1) {
            if (errno == EPIPE) {
                continue;
            } else {
                continue;
            }
            continue;
        }

        player2.paddle.dy = 1;
        
        ssize_t bytes_read = read(fd_in, recv_buffer, sizeof(recv_buffer) - 1); 
        if (bytes_read == -1) {
            continue;  
        } else if (bytes_read == 0) {
            continue;
        } else if (bytes_read > 0) {

                recv_buffer[bytes_read] = '\0';
                
                unsigned long data = hex_to_unsigned_long(recv_buffer);

                player1.score = (data >> 40) & 0xFF;
                player2.score = (data >> 32) & 0xFF;
                player2.paddle.y = (data >> 24) & 0xFF;
                player1.paddle.y = (data >> 16) & 0xFF;
                ball.x = (data >> 8) & 0xFF;
                ball.y = data & 0xFF;
            
                draw(term, &draw_callback);

                if (player1.score == 11 || player2.score == 11) {
                    end_game(term);
                }
        }
    }

    close(fd_in);
    close(fd_out);
    
    pthread_cancel(audio_thread);
    pthread_join(audio_thread, NULL);

    systemExit(term);
    return 0;
}
