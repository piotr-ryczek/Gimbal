#include <filters.h>

float rollBuffer[bufferMaxSize] = {0};
float pitchBuffer[bufferMaxSize] = {0};

std::map<int, int> wageMap = {
    {0, 100},
    {1, 20},
    {2, 10},
    {3, 5},
    {4, 2}
};

int bufferSize = 0;

float addValueToBuffer(float value, float* buffer) {
    for (int i = bufferMaxSize - 1; i > 0; i--) {
        buffer[i] = buffer[i - 1];
    }

    buffer[0] = value;

    if (bufferSize < bufferMaxSize) {
        bufferSize++;
    }

    float sum = 0;
    int divider = 0;
    for (int i = 0; i < bufferSize; i++) {
        sum += buffer[i] * wageMap[i];
        divider += wageMap[i];
    }

    return sum / divider;
}