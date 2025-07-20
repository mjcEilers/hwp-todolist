#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class Ultrasonic {
    int pin;
public:
    Ultrasonic(int pin);
    void begin();
    float getDistanceCenti();
};

#endif
