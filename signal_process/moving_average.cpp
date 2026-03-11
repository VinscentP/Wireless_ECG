#pragma once

template<int WINDOW_SIZE>
class MovingAverage {
private:
    float buffer[WINDOW_SIZE];
    float sum;
    int   index;
    int   count;

public:
    MovingAverage() : sum(0.0f), index(0), count(0) {
        for (int i = 0; i < WINDOW_SIZE; i++) buffer[i] = 0.0f;
    }

    float moving_average(float input) {
        sum -= buffer[index];
        buffer[index] = input;
        sum += input;
        index = (index + 1) % WINDOW_SIZE;
        if (count < WINDOW_SIZE) count++;
        return sum / count;  // also fixed the early-sample bug — was dividing by WINDOW_SIZE before buffer filled
    }

    void reset() {
        for (int i = 0; i < WINDOW_SIZE; i++) buffer[i] = 0.0f;
        sum = 0.0f; index = 0; count = 0;
    }
};