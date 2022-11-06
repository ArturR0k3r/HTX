#ifndef SETINGS
#define SETINGS

//required libraries;
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <VoltageReference.h>
//=================================================================
// ===                   IMPORTANT VARS                         ===
//=================================================================
#define DISPLAY_ADDR         0x3C //I2C address for the SSD1306 display
#define WHEEL_DIAM           1.83 //Measuring wheel diameter in cm
#define DEFAULT_UNITS_INCHES false //sets default units to either cm or inches
#define INCH_FRACT           16 //sets the fraction of an inch that the extra inch display will be rounded to
// ================================================================
// ===                         MODES                            ===
// ================================================================
//Defines the order of modes, you can re-order mods as you wish
//you can remove modes by seetting the mode number to greater than the NUM_MODES ex: 999
//if you do this don't forget to decrease NUM_MODES to match the number of modes remaining
//and also change the order of the remaining modes (going from 0 to however remain)
#define MEASURING_WHEEL  0
#define DIAM_MEASURE     1
#define MEASURING_INSIDE 2
#define TACHOMETER       3

uint8_t NUM_MODES =      4; //total number of active modes

volatile uint8_t mode =  0; //inital mode

// ================================================================
// ===                         PIN SETUP                        ===
// ================================================================
//Arduino Pro-Mini Pins for encoder and pushbuttons
//encoder must use at least one interrupt pin
#define ENCODER_PIN_1     2
#define ENCODER_PIN_2     3
#define MODE_BUTTON_PIN   7
#define ZERO_BUTTON_PIN   6
// ================================================================
// ===                     SCREEN SETUP                         ===
// ================================================================
#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64
#define YELLOW_BAR_HEIGHT 16
#define BASE_FONT_LENGTH  6
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1// Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// ================================================================
// ===                  NTC100K temperature sensor              ===
// ================================================================
//*************************//Ntc100k-t sensor//************************************************
#define B 3950 // B-coefficient 
#define SERIAL_R 100000 // series resistor resistance, 102 кОм 
#define THERMISTOR_R 100000 // nominal resistance of thermistor, 100 кОм 
#define NOMINAL_T 24.5 // nominal temperature(with which TR = 100 кОм) 
const byte tempPin = A0;
// ================================================================
// ===             ENCODER / GEAR RATIO SETUP                   ===
// ================================================================
//both work as interrupts so we should get good performance
Encoder myEnc(ENCODER_PIN_1, ENCODER_PIN_2);
#define WHEEL_DIAM_IN         WHEEL_DIAM * cmToIn //wheel diameter in inches
#define ENC_STEPS_PER_REV     58 //number of encoder steps per revolution
//conversion from one revolution to linear distance traveled in cm
#define REV_TO_LIN_DIST_CM    M_PI * WHEEL_DIAM
//distance covered per encoder step in cm
#define ENC_STEP_LENGTH       REV_TO_LIN_DIST_CM / ENC_STEPS_PER_REV
// ==================================+=============================
// ===                     BAT SENSE SETUP                      ===
// ================================================================
//our range for lipo voltage is 4.2-3.4V,
//after 3.4 the LiPo is 95% empty, so we should recharge
const unsigned long batteryUpdateTime = 5000; //how often we update the battery level in ms
unsigned long prevBatReadTime = 0; //the last time we read the battery in ms
uint8_t batteryLvl; //the battery percentage
#define MAX_BAT_VOLTAGE 4200 //max battery voltage
#define MIN_BAT_VOLTAGE 3400 //min battery voltage
//initialize the voltage reference library to read the Arduino's internal reference voltage
VoltageReference vRef;
// ================================================================
// ===                  PROGRAM GLOBAL VARS                     ===
// ================================================================
boolean zeroButtonToggle = false;
boolean zeroButtonRun = true;
boolean previousZeroButtonState = HIGH;
boolean previousModeButtonState = HIGH;
unsigned long currentTime = micros();
unsigned long lastZeroPress = millis();
const float cmToIn = 0.393701; //conversion from cm to in
const int32_t usInMin = 60 * 1000000; //amount of microseconds in a min
//measurment variables
uint8_t fract;
long newPosition;
double displayPos;
//units for length and flag for changing them
String units;
boolean unitSwitch;
// ================================================================
// ===                   TACHOMETER SETUP                       ===
// ================================================================
boolean tachOn = false; //if a tachometer mode is active
boolean tactReadingStarted = false; //if we're reading from rpm
double rpm, linSpeed;
unsigned long tacStartTime = 0; //start time of tachometer reading
boolean tachStartTimeSet = false;  //starting time is only set once the tach detects motion, flag for it that has happened
const uint8_t measureIntervalMaxSec = 10; //maxium measurement period in seconds
const unsigned long measureIntervalMax = measureIntervalMaxSec * 1000000; //maxium measurement period in us //10 sec
const String measureIntervalMaxString = String(measureIntervalMaxSec); //maxium measurement period as a string in sec
unsigned long elapsedTime;
#endif 