#include <Arduino.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <TMCStepper.h>


#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>



#define BTN 19
#define ENC0 21
#define ENC1 22
#define DIAG0 35
#define DIAG1 34

//TMC
#define EN_PIN           14 // Enable
#define DIR_PIN          27 // Direction
#define STEP_PIN         26 // Step

#define CS_PIN           25 // Chip select
#define SW_MOSI          32 // Software Master Out Slave In (MOSI)
#define SW_MISO          13 // Software Master In Slave Out (MISO)
#define SW_SCK           33 // Software Slave Clock (SCK)




#define R_SENSE 0.075f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075


TMC2160Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);

extern void BLE_init();
extern void BLE_tick();

bool shaft = false;

// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS   15  // Chip select control pin
// #define TFT_DC    2  // Data Command control pin
// #define TFT_RST   4  // Reset pin (could connect to RST pin)
TFT_eSPI tft  = TFT_eSPI();      // Invoke custom library

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB



void setup() {
  
  pinMode(CS_PIN, INPUT);
  pinMode(SW_MOSI, INPUT);
  pinMode(SW_MISO, INPUT);
  pinMode(SW_SCK, INPUT);


  pinMode(BTN, INPUT);
  pinMode(ENC0, INPUT);
  pinMode(ENC1, INPUT);
  pinMode(DIAG0, INPUT);
  pinMode(DIAG1, INPUT);
  
  Serial.begin(921600);
  Serial2.begin(2000000);
Serial2.printf("trtrtertert");

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(0x1234);
  tft.setTextColor(WHITE, BLACK);
  tft.drawString("TFT_eSPI example", 30, 30 , 2);

  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  digitalWrite(EN_PIN, LOW);      // Enable driver in hardware

  driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
                                  // UART: Init SW UART (if selected) with default 115200 baudrate
  driver.toff(5);                 // Enables driver in software

  driver.rms_current(2100);        // Set motor RMS current

  driver.microsteps(2);          // Set microsteps to 1/16th

  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160

  Serial2.print("DRV_STATUS=0b");
  Serial2.println(driver.DRV_STATUS(), BIN);

  Serial2.print("version: " );
  Serial2.println(driver.version());

  Serial2.print("run status: " );
  Serial2.println(driver.irun());

  Serial2.print("run en stat: " );
  Serial2.println(driver.encb_dcen_cfg4());

  Serial2.print("toff: " );
  Serial2.println(driver.toff());

  BLE_init();

}

void loop() {

    // Run 5000 steps and switch direction in software
  //while (true) {

    digitalWrite(STEP_PIN, HIGH);
    //lay(1);
    delayMicroseconds(2000);
    digitalWrite(STEP_PIN, LOW);
    //delay(1);
    delayMicroseconds(2000);

  //}
  //shaft = !shaft;
  //driver.shaft(shaft);
}

