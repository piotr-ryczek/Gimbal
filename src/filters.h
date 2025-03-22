#ifndef FILTERS_H
#define FILTERS_H

#include <map>

const int bufferMaxSize = 5;
extern float rollBuffer[bufferMaxSize];
extern float pitchBuffer[bufferMaxSize];

float addValueToBuffer(float value, float* buffer);

#endif
