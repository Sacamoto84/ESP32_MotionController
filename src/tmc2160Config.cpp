#include "tmc2160Config.h"

#include "global.h"

namespace
{
constexpr uint32_t kGconfRecalibrateBit = 0;
constexpr uint32_t kGconfFastStandstillBit = 1;
constexpr uint32_t kGconfPwmModeBit = 2;
constexpr uint32_t kGconfMultistepFiltBit = 3;
constexpr uint32_t kGconfStopEnableBit = 15;

uint8_t u8(int32_t value, uint8_t minValue, uint8_t maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return static_cast<uint8_t>(value);
}

uint16_t rmsCurrentMa(int32_t value)
{
    if (value < 100)
    {
        return 100;
    }
    if (value > 3100)
    {
        return 3100;
    }
    return static_cast<uint16_t>(value);
}

uint16_t u16(int32_t value, uint16_t minValue, uint16_t maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return static_cast<uint16_t>(value);
}

uint32_t u20(int32_t value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > 0xFFFFF)
    {
        return 0xFFFFF;
    }
    return static_cast<uint32_t>(value);
}

int8_t s7(int32_t value)
{
    if (value < -64)
    {
        return -64;
    }
    if (value > 63)
    {
        return 63;
    }
    return static_cast<int8_t>(value);
}

uint32_t setBit(uint32_t value, uint32_t bit, bool enabled)
{
    const uint32_t mask = 1UL << bit;
    return enabled ? (value | mask) : (value & ~mask);
}

String hex32(uint32_t value)
{
    String out = "0x";
    if (value < 0x10000000UL)
    {
        out += "0";
    }
    if (value < 0x01000000UL)
    {
        out += "0";
    }
    if (value < 0x00100000UL)
    {
        out += "0";
    }
    if (value < 0x00010000UL)
    {
        out += "0";
    }
    if (value < 0x00001000UL)
    {
        out += "0";
    }
    if (value < 0x00000100UL)
    {
        out += "0";
    }
    if (value < 0x00000010UL)
    {
        out += "0";
    }
    out += String(value, HEX);
    out.toUpperCase();
    return out;
}
} // namespace

void applyTmc2160BasicConfig()
{
    uint32_t gconf = driver.GCONF();
    gconf = setBit(gconf, kGconfRecalibrateBit, tmcRecalibrate.get() != 0);
    gconf = setBit(gconf, kGconfFastStandstillBit, tmcFastStandstill.get() != 0);
    gconf = setBit(gconf, kGconfPwmModeBit, tmcDriverChop.get() != 0);
    gconf = setBit(gconf, kGconfMultistepFiltBit, tmcMultistepFilt.get() != 0);
    gconf = setBit(gconf, kGconfStopEnableBit, tmcStopEnable.get() != 0);
    driver.GCONF(gconf);

    driver.TPWMTHRS(u20(tmcTPwmThrs.get()));
    driver.TPOWERDOWN(u8(tmcTPowerDown.get(), 0, 255));
}

void applyTmc2160CurrentConfig()
{
    driver.rms_current(rmsCurrentMa(tmcDriverCurrent.get()));
    driver.ihold(u8(tmcIhold.get(), 0, driver.irun()));
    driver.iholddelay(u8(tmcIholdDelay.get(), 0, 15));
    driver.freewheel(u8(tmcFreewheel.get(), 0, 3));
}

void applyTmc2160ChopperConfig()
{
    driver.toff(u8(tmcToff.get(), 0, 15));
    driver.tbl(u8(tmcTbl.get(), 0, 3));
    driver.hstrt(u8(tmcHstrt.get(), 0, 7));
    driver.hend(u8(tmcHend.get(), 0, 15));
    driver.chm(tmcChm.get() != 0);
    driver.dedge(tmcDedge.get() != 0);
    driver.vhighfs(tmcVhighfs.get() != 0);
    driver.vhighchm(tmcVhighchm.get() != 0);
}

void applyTmc2160StealthConfig()
{
    driver.pwm_autoscale(tmcPwmAutoscale.get() != 0);
    driver.pwm_autograd(tmcPwmAutograd.get() != 0);
    driver.pwm_ofs(u8(tmcPwmOfs.get(), 0, 255));
    driver.pwm_grad(u8(tmcPwmGrad.get(), 0, 255));
    driver.pwm_freq(u8(tmcPwmFreq.get(), 0, 3));
    driver.pwm_reg(u8(tmcPwmReg.get(), 0, 15));
    driver.pwm_lim(u8(tmcPwmLim.get(), 0, 15));
    driver.freewheel(u8(tmcFreewheel.get(), 0, 3));
}

void applyTmc2160StallCoolConfig()
{
    driver.sgt(s7(tmcSgt.get()));
    driver.sfilt(tmcSfilt.get() != 0);
    driver.TCOOLTHRS(u20(tmcTCoolThrs.get()));
    driver.THIGH(u20(tmcTHigh.get()));
    driver.semin(u8(tmcSemin.get(), 0, 15));
    driver.semax(u8(tmcSemax.get(), 0, 15));
    driver.seup(u8(tmcSeup.get(), 0, 3));
    driver.sedn(u8(tmcSedn.get(), 0, 3));
    driver.seimin(tmcSeimin.get() != 0);
}

void applyTmc2160DcStepConfig()
{
    driver.VDCMIN(u20(tmcVdcMin.get()));
    driver.dc_time(u16(tmcDcTime.get(), 0, 1023));
    driver.dc_sg(u8(tmcDcSg.get(), 0, 255));
}

void applyTmc2160ExpertConfig()
{
    driver.diag0_error(tmcDiag0Error.get() != 0);
    driver.diag0_otpw(tmcDiag0Otw.get() != 0);
    driver.diag0_stall(tmcDiag0Stall.get() != 0);
    driver.diag1_stall(tmcDiag1Stall.get() != 0);
    driver.diag1_index(tmcDiag1Index.get() != 0);
    driver.diag1_onstate(tmcDiag1Onstate.get() != 0);
    driver.diag1_steps_skipped(tmcDiag1StepsSkipped.get() != 0);
    driver.diag0_int_pushpull(tmcDiag0PushPull.get() != 0);
    driver.diag1_pushpull(tmcDiag1PushPull.get() != 0);

    driver.s2vs_level(u8(tmcS2vsLevel.get(), 0, 15));
    driver.s2g_level(u8(tmcS2gLevel.get(), 0, 15));
    driver.shortfilter(u8(tmcShortFilter.get(), 0, 3));
    driver.shortdelay(tmcShortDelay.get() != 0);
    driver.bbmtime(u8(tmcBbmTime.get(), 0, 24));
    driver.bbmclks(u8(tmcBbmClks.get(), 0, 15));
    driver.otselect(u8(tmcOtSelect.get(), 0, 3));
    driver.drvstrength(u8(tmcDrvStrength.get(), 0, 3));
    driver.filt_isense(u8(tmcFiltIsense.get(), 0, 3));

    applyTmc2160BasicConfig();
}

void applyTmc2160Config()
{
    applyTmc2160BasicConfig();
    applyTmc2160CurrentConfig();
    applyTmc2160ChopperConfig();
    applyTmc2160StealthConfig();
    applyTmc2160StallCoolConfig();
    applyTmc2160ExpertConfig();
}

Tmc2160Status readTmc2160Status()
{
    Tmc2160Status status;
    status.drvStatus = driver.DRV_STATUS();
    status.gstat = driver.GSTAT();
    status.ioin = driver.IOIN();
    status.pwmScale = driver.PWM_SCALE();
    status.lostSteps = driver.LOST_STEPS();
    status.sgResult = driver.sg_result();
    status.csActual = driver.cs_actual();
    status.version = driver.version();
    status.fsActive = driver.fsactive();
    status.stallGuard = driver.stallguard();
    status.overtemperature = driver.ot();
    status.overtemperatureWarning = driver.otpw();
    status.shortToGroundA = driver.s2ga();
    status.shortToGroundB = driver.s2gb();
    status.openLoadA = driver.ola();
    status.openLoadB = driver.olb();
    status.standstill = driver.stst();
    status.reset = driver.reset();
    status.driverError = driver.drv_err();
    status.chargePumpUndervoltage = driver.uv_cp();
    status.diag0 = digitalRead(DIAG0);
    status.diag1 = digitalRead(DIAG1);
    status.driverEnabledInput = !driver.drv_enn();
    return status;
}

void clearTmc2160Status()
{
    driver.GSTAT(0b111);
}

String tmcStatusDrvLine()
{
    const Tmc2160Status status = readTmc2160Status();
    return "DRV_STATUS: " + hex32(status.drvStatus);
}

String tmcStatusGstatLine()
{
    const Tmc2160Status status = readTmc2160Status();
    return "GSTAT R/E/UV: " + String(status.reset) + "/" + String(status.driverError) + "/" +
           String(status.chargePumpUndervoltage);
}

String tmcStatusIoLine()
{
    const Tmc2160Status status = readTmc2160Status();
    return "IOIN v" + String(status.version) + " D0/D1 " + String(status.diag0) + "/" + String(status.diag1);
}

String tmcStatusLoadLine()
{
    const Tmc2160Status status = readTmc2160Status();
    return "OT/OL/S2G: " + String(status.overtemperatureWarning) + String(status.overtemperature) + " " +
           String(status.openLoadA) + String(status.openLoadB) + " " + String(status.shortToGroundA) +
           String(status.shortToGroundB);
}

String tmcStatusPwmLine()
{
    const Tmc2160Status status = readTmc2160Status();
    return "PWM scale: " + String(status.pwmScale);
}

String tmcStatusLostStepsLine()
{
    return "Lost steps: " + String(readTmc2160Status().lostSteps);
}

String tmcStatusCsActualLine()
{
    return "CS actual: " + String(readTmc2160Status().csActual);
}

String tmcStatusSgLine()
{
    const Tmc2160Status status = readTmc2160Status();
    return "SG: " + String(status.sgResult) + " stall " + String(status.stallGuard) + " stst " +
           String(status.standstill);
}

String tmcStatusRmsCurrentLine()
{
    return "RMS actual: " + String(driver.rms_current()) + " mA";
}

String tmcStatusCurrentRegsLine()
{
    return "IRUN/IHOLD/GS: " + String(driver.irun()) + "/" + String(driver.ihold()) + "/" +
           String(driver.GLOBAL_SCALER());
}
