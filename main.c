#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "portaudio.h"
#include <math.h>
#include "ftascii.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

#ifndef FFT_SIZE
  #define FFT_SIZE 64
#endif 

typedef struct {
    float left_phase;
    float right_phase;
} paTestData;

fftw_complex *in, *out;
fftw_plan plan;

// array that stores the fft values
float fft_values[FFT_SIZE];

// This callback function will be called by PortAudio when audio is available
static int audioCallback(const void *inputBuffer, void *outputBuffer,unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,void *userData) 
{
	(void)framesPerBuffer, 	(void)outputBuffer;
	(void)timeInfo, 		(void)statusFlags;

    paTestData *data = (paTestData*)userData;
    const float *inBuffer = (const float*)inputBuffer;

	(void)data;
    // Copy input data to FFT input buffer
    for (int i = 0; i < FFT_SIZE; i++) {
        in[i][0] = inBuffer[i];
        in[i][1] = 0.0;
    }

    // Execute the FFT
    fftw_execute(plan);

    // Output the magnitude spectrum
    for (int i = 0; i < FFT_SIZE; i++) {
		fft_values[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        #ifdef DEBUG
            printf("[ %d ] : %.2f ", i, fft_values[i]);
			audio_stats(fft_values, FFT_SIZE);
        #endif
    }
	#ifdef DEBUG
		printf("\n");	
	#endif

    return paContinue; // Continue streaming audio
}

int main() {
    PaError err;
    paTestData data;
    PaStream *stream;
    PaStreamParameters inputParameters;

    // Initialize PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    // Allocate memory for FFT input and output arrays
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);

    // Create FFTW plan
    plan = fftw_plan_dft_1d(FFT_SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // Set up PortAudio stream parameters
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 1; // Mono input
    inputParameters.sampleFormat = paFloat32; // Float32 format
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    // Open PortAudio stream
    err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE,
                        FRAMES_PER_BUFFER, paClipOff, audioCallback, &data);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        return 1;
    }

    // Start the stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

	signal(SIGINT, handlectrl_c);

#ifndef DEBUG
    ft_ascii(fft_values);
#else
   while(1) {};
#endif
    // Stop and close the stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    }
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    }

    // Clean up
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    Pa_Terminate();

    return 0;
}

void exit_audio(){
	fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    Pa_Terminate();
    systemExit();
}

void handlectrl_c(int sig) {
    (void)sig;
	exit_audio();
    systemExit();
}
