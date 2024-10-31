/* Measurement system for low speed wind tunnel */
//-------------------------------------------------------------------------------------
// This code calculates lift and drag forces on the new 2-axis force measurement system
// at the University of Reykjavik
// It uses the Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// from Olav Kallhovd
// Code was developed during the masters class Precision Machine Design in the FALL semester 2024
// Code was written by Matthias Reiser

//-------------------------------------------------------------------------------------
// HX711_ADC.h
// Arduino master library for HX711 24-Bit Analog-to-Digital Converter for Weigh Scales
// Olav Kallhovd sept2017
// Tested with      : HX711 asian module on channel A and YZC-133 3kg load cell
// Tested with MCU  : Arduino Nano
//-------------------------------------------------------------------------------------
// This is an example sketch on how to use this library for two ore more HX711 modules
// Settling time (number of samples) and data filtering can be adjusted in the config.h file

/* Measurement system for low speed wind tunnel */
/* two HX711 amplifiers are used for a 2 axis load cell
   three buttons are used
   two of those tare lift and drag force
   one is to reset the tare values for this the tare offset for 0g needs to be saved in eeprom or written into the scetch, when calibrating the loadcell
   a 2x16 lcd display is used for displaying the measured forces */

#include <HX711_ADC.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#include <Wire.h>
#include "button.h"
#include "Calc_Newton.h"
#include "version.h"
#include "stdbool.h"
#include "LCDDisplay.h"
#include <LiquidCrystal_I2C.h>
#include "Calc_Newton.h"

LCDDisplay lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
// LiquidCrystal_I2C lcd_raw(0x27, 16, 2);

// pins:
const int HX711_dout_1 = 4; // mcu > HX711 no 1 dout pin
const int HX711_sck_1 = 5;  // mcu > HX711 no 1 sck pin
const int HX711_dout_2 = 6; // mcu > HX711 no 2 dout pin
const int HX711_sck_2 = 7;  // mcu > HX711 no 2 sck pin

// HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell_lift(HX711_dout_1, HX711_sck_1); // HX711 1
HX711_ADC LoadCell_drag(HX711_dout_2, HX711_sck_2); // HX711 2

const int calVal_eepromAdress_1 = 0; // eeprom adress for calibration value load cell 1 (4 bytes)
const int calVal_eepromAdress_2 = 4; // eeprom adress for calibration value load cell 2 (4 bytes)
const int tareVal_eepromAdress_1 = 8;  // eeprom adress for tare value load cell 1 (8 bytes)
const int tareVal_eepromAdress_2 = 16; // eeprom adress for tare value load cell 2 (8 bytes)
unsigned long t = 0;

button BtnTareLift;
button BtnTareDrag;
button BtnRstTare;

version Version;

static char m_acRow[16] = "Ver: ";

static long tareValue_lift; // tare value load cell 1 for zero weight
static long tareValue_drag; // tare value load cell 2 for zero weight

void setup()
{
  BtnTareLift.init(12);
  BtnTareDrag.init(11);
  BtnRstTare.init(10);
  lcd.init();

  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  Version.getString(&m_acRow[5]);
  lcd.writeFixedRows(m_acRow, "Status: Ready"); // Write to both rows


  float calibrationValue_lift; // calibration value load cell 1
  float calibrationValue_drag; // calibration value load cell 2



  // calibrationValue_lift = 696.0; // uncomment this if you want to set this value in the sketch
  // calibrationValue_drag = 733.0; // uncomment this if you want to set this value in the sketch
  tareValue_lift = 8392525;
  tareValue_drag = 8396492;
#if defined(ESP8266) || defined(ESP32)
  // EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch the value from eeprom
#endif
  EEPROM.get(calVal_eepromAdress_1, calibrationValue_lift); // uncomment this if you want to fetch the value from eeprom
  EEPROM.get(calVal_eepromAdress_2, calibrationValue_drag); // uncomment this if you want to fetch the value from eeprom
  // EEPROM.get(tareVal_eepromAdress_1, tareValue_lift); // uncomment this if you want to fetch the value from eeprom
  // EEPROM.get(tareVal_eepromAdress_2, tareValue_drag); // uncomment this if you want to fetch the value from eeprom

  LoadCell_lift.begin();
  LoadCell_drag.begin();
  // LoadCell_lift.setReverseOutput();
  // LoadCell_drag.setReverseOutput();
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  bool _tare = false;                    // set this to false if you don't want tare to be performed in the next step
  uint8_t loadcell_1_rdy = 0;
  uint8_t loadcell_2_rdy = 0;

  while ((loadcell_1_rdy + loadcell_2_rdy) < 2)
  { // run startup, stabilization and tare, both modules simultaniously
    if (!loadcell_1_rdy)
    {
      loadcell_1_rdy = LoadCell_lift.startMultiple(stabilizingtime, _tare);
    }
    if (!loadcell_2_rdy)
    {
      loadcell_2_rdy = LoadCell_drag.startMultiple(stabilizingtime, _tare);
    }
  }

  if (LoadCell_lift.getTareTimeoutFlag())
  {
    Serial.println("Timeout, check MCU>HX711 lift wiring and pin designations");
  }
  if (LoadCell_drag.getTareTimeoutFlag())
  {
    Serial.println("Timeout, check MCU>HX711 drag wiring and pin designations");
  }

  LoadCell_lift.setCalFactor(calibrationValue_lift); // user set calibration value (float)
  LoadCell_drag.setCalFactor(calibrationValue_drag); // user set calibration value (float)
  Serial.println("Startup is complete");
}

/* Measurement system for low speed wind tunnel */
static char m_acLift[16] = {0};
static char m_acDrag[16] = {0};
void loop()
{
  static bool newDataReady = false;
  const int serialPrintInterval = 100; // increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell_lift.update())
  {
    newDataReady = true;
  }
  LoadCell_drag.update();

  // get smoothed value from data set
  if ((newDataReady))
  {
    if (millis() > t + serialPrintInterval)
    {
      char tempStr[8];
      float a = LoadCell_lift.getData();
      float b = LoadCell_drag.getData();
      long offset = LoadCell_drag.getTareOffset();

      float liftN = convertToNewtons(a);
      float DragN = convertToNewtons(b);
      dtostrf(liftN, 6, 2, tempStr);
      snprintf(m_acLift, sizeof(m_acLift), "Lift: %sN", tempStr);
      dtostrf(DragN, 6, 3, tempStr);
      snprintf(m_acDrag, sizeof(m_acDrag), "Drag: %sN", tempStr);
      Serial.print("Load_cell 1 output val: ");
      Serial.print(a);
      Serial.print("    Drag tare offest: ");
      Serial.print(offset);
      Serial.print("    Load_cell 2 output val: ");
      Serial.println(b);
      newDataReady = false;
      t = millis();

      lcd.writeFixedRows(m_acLift, m_acDrag);
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0)
  {
    char inByte = Serial.read();
    if (inByte == 't')
    {
      LoadCell_lift.tareNoDelay();
      LoadCell_drag.tareNoDelay();
    }
    if(inByte == 'r')
    {
     long offset = LoadCell_lift.getTareOffset(); 
     
     float callift = LoadCell_lift.getCalFactor();
    LoadCell_lift.setTareOffset(offset/callift);
    }
  }

  if(BtnTareLift.was_pressed() == true)
  {
    LoadCell_lift.tareNoDelay();
  }

  if(BtnTareDrag.was_pressed() == true)
  {
    LoadCell_drag.tareNoDelay();
  }

  if(BtnRstTare.was_pressed() == true)
  {
    LoadCell_lift.setTareOffset(tareValue_lift);
    LoadCell_drag.setTareOffset(tareValue_drag);
  }



  // check if last tare operation is complete
  if (LoadCell_lift.getTareStatus() == true)
  {
    Serial.println("Tare load cell 1 complete");
  }
  if (LoadCell_drag.getTareStatus() == true)
  {
    Serial.println("Tare load cell 2 complete");
  }
}