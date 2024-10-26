#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"

class button
{
private:
    uint8_t pin;
    uint32_t u32DbncTime = 200; // debounce time in ms
    uint32_t lastPressTime = 0;
    bool bHasChanged = false;
    bool lastButtonState = HIGH;

    bool is_high(void);
    bool is_low(void);

public:
    button(/* args */);
    ~button();

    int init(uint8_t pin_number);
    bool was_pressed(void);
};





#endif