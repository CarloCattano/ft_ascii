#include "stdio.h"
#include "stdlib.h"
#include "portaudio.h"

// gcc main.c -lportaudio -lrt -lm -lasound -ljack -pthread -o pa_out_index

int main() {
    PaError err;
    err = Pa_Initialize();
    // get defaul audio input device 
    PaDeviceIndex default_input_device = Pa_GetDefaultInputDevice();
    // list input devices 
    int numDevices = Pa_GetDeviceCount();
    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        printf("device %d: %s\n", i, deviceInfo->name);
    }


    printf("default output device index %d", default_input_device);
    return EXIT_SUCCESS;
}
