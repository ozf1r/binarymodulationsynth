#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265
#define SAMPLE_RATE 44100  // Adjust as needed
#define DURATION 60         // Adjust as needed

typedef struct {
    char chunkId[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1Id[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2Id[4];
    uint32_t subchunk2Size;
} WavHeader;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <frequency1> <frequency2> <operation>\n", argv[0]);
        fprintf(stderr, "   Available operations: AND, XOR, OR\n");
        return 1;
    }

    double frequency1 = atof(argv[1]);
    double frequency2 = atof(argv[2]);
    char *operation = argv[3];

    FILE *outputFile;
    outputFile = fopen("wave.wav", "wb");

    if (outputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Create WAV header
    WavHeader header = {
        .chunkId = "RIFF",
        .chunkSize = 36 + (SAMPLE_RATE * DURATION * sizeof(int16_t)),
        .format = "WAVE",
        .subchunk1Id = "fmt ",
        .subchunk1Size = 16,
        .audioFormat = 1,
        .numChannels = 1,
        .sampleRate = SAMPLE_RATE,
        .byteRate = SAMPLE_RATE * sizeof(int16_t),
        .blockAlign = sizeof(int16_t),
        .bitsPerSample = 16,
        .subchunk2Id = "data",
        .subchunk2Size = SAMPLE_RATE * DURATION * sizeof(int16_t),
    };

    fwrite(&header, sizeof(WavHeader), 1, outputFile);

    // Generate and write modulated sine wave samples
    for (double t = 0; t < DURATION; t += 1.0 / SAMPLE_RATE) {
        int16_t sample1 = (int16_t)(32767.0 * sin(2.0 * PI * frequency1 * t));
        int16_t sample2 = (int16_t)(32767.0 * sin(2.0 * PI * frequency2 * t));
        int16_t modulatedSample;

        if (strcmp(operation, "AND") == 0) {
            modulatedSample = sample1 & sample2;
        } else if (strcmp(operation, "XOR") == 0) {
            modulatedSample = sample1 ^ sample2;
        } else if (strcmp(operation, "OR") == 0) {
            modulatedSample = sample1 | sample2;
        } else {
            fprintf(stderr, "Invalid operation: %s\n", operation);
            fclose(outputFile);
            return 1;
        }

        fwrite(&modulatedSample, sizeof(int16_t), 1, outputFile);
    }

    fclose(outputFile);

    return 0;
}
