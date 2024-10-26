#include "Calc_Newton.h"

float convertToNewtons(float massInGrams)
{
    const float gravity = 9.81;              // gravitational acceleration in m/s^2
    return (massInGrams * gravity) / 1000.0; // Convert grams to kilograms and calculate force in newtons
}