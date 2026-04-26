#ifndef TMC2160_CONFIG_H
#define TMC2160_CONFIG_H

#include <Arduino.h>

struct Tmc2160Status
{
    uint32_t drvStatus = 0;
    uint8_t gstat = 0;
    uint32_t ioin = 0;
    uint32_t pwmScale = 0;
    uint32_t lostSteps = 0;
    uint16_t sgResult = 0;
    uint8_t csActual = 0;
    uint8_t version = 0;
    bool fsActive = false;
    bool stallGuard = false;
    bool overtemperature = false;
    bool overtemperatureWarning = false;
    bool shortToGroundA = false;
    bool shortToGroundB = false;
    bool openLoadA = false;
    bool openLoadB = false;
    bool standstill = false;
    bool reset = false;
    bool driverError = false;
    bool chargePumpUndervoltage = false;
    bool diag0 = false;
    bool diag1 = false;
    bool driverEnabledInput = false;
};

void applyTmc2160Config();
void applyTmc2160BasicConfig();
void applyTmc2160CurrentConfig();
void applyTmc2160ChopperConfig();
void applyTmc2160StealthConfig();
void applyTmc2160StallCoolConfig();
void applyTmc2160DcStepConfig();
void applyTmc2160ExpertConfig();

Tmc2160Status readTmc2160Status();
void clearTmc2160Status();

String tmcStatusDrvLine();
String tmcStatusGstatLine();
String tmcStatusIoLine();
String tmcStatusLoadLine();
String tmcStatusPwmLine();
String tmcStatusLostStepsLine();
String tmcStatusCsActualLine();
String tmcStatusSgLine();

#endif
