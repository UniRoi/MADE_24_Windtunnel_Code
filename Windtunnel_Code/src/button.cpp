#include "button.h"
#include "Arduino.h"

button::button(/* args */)
{
}

button::~button()
{
}

bool button::is_high(void)
{
    return digitalRead(pin) == HIGH;
}

bool button::is_low(void)
{
    return digitalRead(pin) == LOW;
}

int button::init(uint8_t pin_number)
{
    pin = pin_number;
    pinMode(pin, INPUT_PULLUP);         // Assumes pull-up resistor setup
    lastButtonState = digitalRead(pin); // Initialize the button state
    return 0;
}

bool button::was_pressed(void)
{
    bool currentState = is_low(); // Button pressed if low
    uint32_t currentTime = millis();

    if (currentState != lastButtonState)
    {
        lastPressTime = currentTime; // Reset debounce timer
    }

    if ((currentTime - lastPressTime) >= u32DbncTime)
    {
        // If debounce time has passed and state has changed
        if (currentState != bHasChanged && currentState == true)
        {
            bHasChanged = true; // Register a press
            lastButtonState = currentState;
            return true;
        }
        else if (currentState == false)
        {
            bHasChanged = false;
        }
    }

    lastButtonState = currentState; // Update last state for next cycle
    return false;
}
