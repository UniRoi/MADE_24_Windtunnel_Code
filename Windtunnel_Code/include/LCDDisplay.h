#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H
#include <LiquidCrystal_I2C.h>

class LCDDisplay
{
private:
    LiquidCrystal_I2C lcd; // I2C LCD instance
    uint8_t rowLength;     // Length of each row on the LCD
    uint8_t column;
    uint8_t row;

public:
    LCDDisplay(uint8_t address, uint8_t columns, uint8_t rows)
        : lcd(address, columns, rows), rowLength(columns)
    {
        column = columns;
        row = rows;
    }

    void init()
    {
        lcd.init();
        lcd.backlight(); // Turn on the LCD backlight
        lcd.clear();     // Clear the LCD screen
    }

    void writeRow(uint8_t row, const char *text, float data)
    {
        lcd.setCursor(0, row); // Set cursor at the beginning of the row
        lcd.print(text);        // Write the text
        lcd.print(data);
        // clearRemaining(row, strlen(text)); // Clear any extra characters if text is shorter than rowLength
    }

    void writeFixedRows(const char *text1, const char *text2, float data1, float data2)
    {
        writeRow(0, text1, data1);
        writeRow(1, text2, data2);
    }

    void writeRow(uint8_t row, const char *text)
    {
        lcd.setCursor(0, row); // Set cursor at the beginning of the row
        lcd.print(text);        // Write the text
        clearRemaining(row, strlen(text)); // Clear any extra characters if text is shorter than rowLength
    }

    void writeFixedRows(const char *text1, const char *text2)
    {
        writeRow(0, text1);
        writeRow(1, text2);
    }

private:
    void clearRemaining(uint8_t row, uint8_t writtenChars)
    {
        // Fill the remaining space in the row with spaces
        for (uint8_t i = writtenChars; i < rowLength; i++)
        {
            lcd.setCursor(i, row);
            lcd.print(" ");
        }
    }
};

#endif
