#ifndef _TMC_GLOBAL_
#define _TMC_GLOBAL_

#include <Arduino.h>
#include <SPI.h>
#include <TMCStepper.h>

// TMC
#define EN_PIN 14   // Enable
#define DIR_PIN 27  // Direction
#define STEP_PIN 26 // Step

#define CS_PIN 25  // Chip select
#define SW_MOSI 32 // Software Master Out Slave In (MOSI)
#define SW_MISO 13 // Software Master In Slave Out (MISO)
#define SW_SCK 33  // Software Slave Clock (SCK)

#define R_SENSE 0.075f // Match to your driver
                       // SilentStepStick series use 0.11
                       // UltiMachine Einsy and Archim2 boards use 0.2
                       // Panucatt BSD2660 uses 0.1
                       // Watterott TMC5160 uses 0.075

extern uint8_t buf20[];

extern uint8_t updateLCD;

extern TMC2160Stepper driver;

void report0();


#endif // ends #ifndef _TFT_eSPIH_