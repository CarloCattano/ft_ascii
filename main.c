#include "ftascii.h"
#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"
#include <math.h>
#include <locale.h>

#define SAMPLE_RATE 44100
/* #define SAMPLE_RATE 48000 */
#define FRAMES_PER_BUFFER 512

float g_amplitude = 0.0;

int paCallback(const void *inputBuffer, void *outputBuffer,
               unsigned long framesPerBuffer,
               const PaStreamCallbackTimeInfo *timeInfo,
               PaStreamCallbackFlags statusFlags,
               void *userData) {
    float *in = (float*)inputBuffer;
    float amplitude = 0.0;
    
    // Calculate amplitude (root mean square)
    for (int i = 0; i < framesPerBuffer; i++) {
        amplitude += (*in) * (*in);
        in++;
    }
    amplitude = sqrtf(amplitude / framesPerBuffer);
    g_amplitude = amplitude;

    return paContinue;
}

int main() {
    PaError err;
    PaStream *stream;
    PaStreamParameters inputParameters;

    // Initialize PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        //printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    // Set input parameters
    inputParameters.device = Pa_GetDefaultInputDevice();
    /* inputParameters.device = 11; */
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    // Open audio stream
    err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, paCallback, NULL);
    if (err != paNoError) {
        //printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        return 1;
    }

    // Start audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        //printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    /* keyhook variables */
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    // set locale to utf-8
    /* setlocale(LC_ALL, ""); */
    /* disable echo and buffering */
    system("stty -echo -icanon -icrnl time 0 min 0"); 

    signal(SIGINT, handlectrl_c);

    write(1, NOMOUSE, 6);   // hide cursor
    /* system("clear"); */
    usleep(1000);
    term_t term = {w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                             NULL, NULL, NULL, 1, 1, 0};
    init_term(&term);
    draw(&term, g_amplitude);
    while(1) 
    {
        ft_keyhook(&term);
        term.clear ? memset(term.buffer, ' ', term.size) : 0;
        if (g_amplitude > 0.2) {
            move_player(&term, term.players[0]);
            move_player(&term, term.players[1]);
            move_player(&term, term.players[2]);
            move_player(&term, term.players[3]);
        }
        if (g_amplitude > 0.5) {
            memset(term.buffer, ' ', term.size);
        }
        draw(&term, g_amplitude);
        copy_last_buffer(&term);
        usleep(term.delay);
    }
 
    // Stop and close audio stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        //printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        //printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    }

    // Terminate PortAudio
    err = Pa_Terminate();
    if (err != paNoError) {
        //printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }
    
    return 0;
}
