#include "storage.h"

#include <GyverDBFile.h>

#include "global.h"
#include "tmc2160Config.h"

namespace
{
constexpr char kDbPath[] = "/data.db";
constexpr char kDbKeyContinuousDirection[] = "continuous.direction";
constexpr char kDbKeyContinuousRps[] = "continuous.rps";
constexpr char kDbKeyVibroFrequency[] = "vibro.frequency";
constexpr char kDbKeyVibroAngle[] = "vibro.angle";
constexpr char kDbKeyVibroMicrostep[] = "vibro.microstep";
constexpr char kDbKeyTmcDriverChop[] = "tmc2160.driver_chop";
constexpr char kDbKeyTmcDriverCurrent[] = "tmc2160.driver_current";
constexpr char kDbKeyTmcDriverMicrostep[] = "tmc2160.driver_microstep";
constexpr char kDbKeyTmcInterpolation[] = "tmc2160.interpolation";
constexpr char kDbKeyTmcFastStandstill[] = "tmc2160.fast_standstill";
constexpr char kDbKeyTmcTPwmThrs[] = "tmc2160.tpwmthrs";
constexpr char kDbKeyTmcTPowerDown[] = "tmc2160.tpowerdown";
constexpr char kDbKeyTmcIrun[] = "tmc2160.irun";
constexpr char kDbKeyTmcIhold[] = "tmc2160.ihold";
constexpr char kDbKeyTmcIholdDelay[] = "tmc2160.iholddelay";
constexpr char kDbKeyTmcGlobalScaler[] = "tmc2160.global_scaler";
constexpr char kDbKeyTmcFreewheel[] = "tmc2160.freewheel";
constexpr char kDbKeyTmcToff[] = "tmc2160.toff";
constexpr char kDbKeyTmcTbl[] = "tmc2160.tbl";
constexpr char kDbKeyTmcHstrt[] = "tmc2160.hstrt";
constexpr char kDbKeyTmcHend[] = "tmc2160.hend";
constexpr char kDbKeyTmcChm[] = "tmc2160.chm";
constexpr char kDbKeyTmcDedge[] = "tmc2160.dedge";
constexpr char kDbKeyTmcVhighfs[] = "tmc2160.vhighfs";
constexpr char kDbKeyTmcVhighchm[] = "tmc2160.vhighchm";
constexpr char kDbKeyTmcPwmAutoscale[] = "tmc2160.pwm_autoscale";
constexpr char kDbKeyTmcPwmAutograd[] = "tmc2160.pwm_autograd";
constexpr char kDbKeyTmcPwmOfs[] = "tmc2160.pwm_ofs";
constexpr char kDbKeyTmcPwmGrad[] = "tmc2160.pwm_grad";
constexpr char kDbKeyTmcPwmFreq[] = "tmc2160.pwm_freq";
constexpr char kDbKeyTmcPwmReg[] = "tmc2160.pwm_reg";
constexpr char kDbKeyTmcPwmLim[] = "tmc2160.pwm_lim";
constexpr char kDbKeyTmcSgt[] = "tmc2160.sgt";
constexpr char kDbKeyTmcSfilt[] = "tmc2160.sfilt";
constexpr char kDbKeyTmcTCoolThrs[] = "tmc2160.tcoolthrs";
constexpr char kDbKeyTmcTHigh[] = "tmc2160.thigh";
constexpr char kDbKeyTmcSemin[] = "tmc2160.semin";
constexpr char kDbKeyTmcSemax[] = "tmc2160.semax";
constexpr char kDbKeyTmcSeup[] = "tmc2160.seup";
constexpr char kDbKeyTmcSedn[] = "tmc2160.sedn";
constexpr char kDbKeyTmcSeimin[] = "tmc2160.seimin";
constexpr char kDbKeyTmcVdcMin[] = "tmc2160.vdcmin";
constexpr char kDbKeyTmcDcTime[] = "tmc2160.dc_time";
constexpr char kDbKeyTmcDcSg[] = "tmc2160.dc_sg";
constexpr char kDbKeyTmcRecalibrate[] = "tmc2160.recalibrate";
constexpr char kDbKeyTmcMultistepFilt[] = "tmc2160.multistep_filt";
constexpr char kDbKeyTmcStopEnable[] = "tmc2160.stop_enable";
constexpr char kDbKeyTmcDiag0Error[] = "tmc2160.diag0_error";
constexpr char kDbKeyTmcDiag0Otw[] = "tmc2160.diag0_otw";
constexpr char kDbKeyTmcDiag0Stall[] = "tmc2160.diag0_stall";
constexpr char kDbKeyTmcDiag1Stall[] = "tmc2160.diag1_stall";
constexpr char kDbKeyTmcDiag1Index[] = "tmc2160.diag1_index";
constexpr char kDbKeyTmcDiag1Onstate[] = "tmc2160.diag1_onstate";
constexpr char kDbKeyTmcDiag1StepsSkipped[] = "tmc2160.diag1_steps_skipped";
constexpr char kDbKeyTmcDiag0PushPull[] = "tmc2160.diag0_pushpull";
constexpr char kDbKeyTmcDiag1PushPull[] = "tmc2160.diag1_pushpull";
constexpr char kDbKeyTmcS2vsLevel[] = "tmc2160.s2vs_level";
constexpr char kDbKeyTmcS2gLevel[] = "tmc2160.s2g_level";
constexpr char kDbKeyTmcShortFilter[] = "tmc2160.shortfilter";
constexpr char kDbKeyTmcShortDelay[] = "tmc2160.shortdelay";
constexpr char kDbKeyTmcBbmTime[] = "tmc2160.bbmtime";
constexpr char kDbKeyTmcBbmClks[] = "tmc2160.bbmclks";
constexpr char kDbKeyTmcOtSelect[] = "tmc2160.otselect";
constexpr char kDbKeyTmcDrvStrength[] = "tmc2160.drvstrength";
constexpr char kDbKeyTmcFiltIsense[] = "tmc2160.filt_isense";

GyverDBFile db(&LittleFS, kDbPath);
bool storageReady = false;

void loadInt(const char* key, State<int32_t>& state)
{
    if (db.has(key))
    {
        state.set((int32_t)db[key]);
    }
}

bool saveInt(const char* key, State<int32_t>& state)
{
    return db.set(key, state.get());
}

bool commitSettings(const char* label, bool changed)
{
    if (!changed)
    {
        timber.i("%s settings already saved", label);
        return true;
    }

    const bool saved = db.update();
    if (saved)
    {
        timber.i("%s settings saved", label);
    }
    else
    {
        timber.e("%s settings save failed", label);
    }
    return saved;
}
}

bool initStorage()
{
    const bool dbReady = db.begin();
    storageReady = dbReady || LittleFS.exists(kDbPath);

    if (storageReady)
    {
        timber.i("Storage ready: %s", kDbPath);
        if (!dbReady)
        {
            timber.w("Storage file is empty or unreadable, defaults will be used");
        }
    }
    else
    {
        timber.e("Storage init failed: %s", kDbPath);
    }

    return storageReady;
}

void tickStorage()
{
    if (storageReady)
    {
        db.tick();
    }
}

void loadContinuousScreenSettings()
{
    if (!storageReady)
    {
        return;
    }

    if (db.has(kDbKeyContinuousDirection))
    {
        constDirection.set((int32_t)db[kDbKeyContinuousDirection]);
    }
    if (db.has(kDbKeyContinuousRps))
    {
        constRps.set((float)db[kDbKeyContinuousRps]);
    }

    timber.i("Continuous settings loaded: direction=%d rps=%.2f",
             constDirection.get(), constRps.get());
}

void loadVibroScreenSettings()
{
    if (!storageReady)
    {
        return;
    }

    if (db.has(kDbKeyVibroMicrostep))
    {
        tmcDriverMicrostep.set((int32_t)db[kDbKeyVibroMicrostep]);
    }
    if (db.has(kDbKeyVibroAngle))
    {
        vibroAngle.set((float)db[kDbKeyVibroAngle]);
    }
    if (db.has(kDbKeyVibroFrequency))
    {
        vibroFr.set((float)db[kDbKeyVibroFrequency]);
    }

    timber.i("Vibro settings loaded: fr=%.2f angle=%.2f microstep=%d",
             vibroFr.get(), vibroAngle.get(), tmcDriverMicrostep.get());
}

void loadTmc2160Settings()
{
    if (!storageReady)
    {
        return;
    }

    loadInt(kDbKeyTmcDriverChop, tmcDriverChop);
    loadInt(kDbKeyTmcDriverCurrent, tmcDriverCurrent);
    loadInt(kDbKeyTmcDriverMicrostep, tmcDriverMicrostep);
    loadInt(kDbKeyTmcInterpolation, tmcInterpolation);
    loadInt(kDbKeyTmcFastStandstill, tmcFastStandstill);
    loadInt(kDbKeyTmcTPwmThrs, tmcTPwmThrs);
    loadInt(kDbKeyTmcTPowerDown, tmcTPowerDown);
    loadInt(kDbKeyTmcIrun, tmcIrun);
    loadInt(kDbKeyTmcIhold, tmcIhold);
    loadInt(kDbKeyTmcIholdDelay, tmcIholdDelay);
    loadInt(kDbKeyTmcGlobalScaler, tmcGlobalScaler);
    loadInt(kDbKeyTmcFreewheel, tmcFreewheel);
    loadInt(kDbKeyTmcToff, tmcToff);
    loadInt(kDbKeyTmcTbl, tmcTbl);
    loadInt(kDbKeyTmcHstrt, tmcHstrt);
    loadInt(kDbKeyTmcHend, tmcHend);
    loadInt(kDbKeyTmcChm, tmcChm);
    loadInt(kDbKeyTmcDedge, tmcDedge);
    loadInt(kDbKeyTmcVhighfs, tmcVhighfs);
    loadInt(kDbKeyTmcVhighchm, tmcVhighchm);
    loadInt(kDbKeyTmcPwmAutoscale, tmcPwmAutoscale);
    loadInt(kDbKeyTmcPwmAutograd, tmcPwmAutograd);
    loadInt(kDbKeyTmcPwmOfs, tmcPwmOfs);
    loadInt(kDbKeyTmcPwmGrad, tmcPwmGrad);
    loadInt(kDbKeyTmcPwmFreq, tmcPwmFreq);
    loadInt(kDbKeyTmcPwmReg, tmcPwmReg);
    loadInt(kDbKeyTmcPwmLim, tmcPwmLim);
    loadInt(kDbKeyTmcSgt, tmcSgt);
    loadInt(kDbKeyTmcSfilt, tmcSfilt);
    loadInt(kDbKeyTmcTCoolThrs, tmcTCoolThrs);
    loadInt(kDbKeyTmcTHigh, tmcTHigh);
    loadInt(kDbKeyTmcSemin, tmcSemin);
    loadInt(kDbKeyTmcSemax, tmcSemax);
    loadInt(kDbKeyTmcSeup, tmcSeup);
    loadInt(kDbKeyTmcSedn, tmcSedn);
    loadInt(kDbKeyTmcSeimin, tmcSeimin);
    loadInt(kDbKeyTmcVdcMin, tmcVdcMin);
    loadInt(kDbKeyTmcDcTime, tmcDcTime);
    loadInt(kDbKeyTmcDcSg, tmcDcSg);
    loadInt(kDbKeyTmcRecalibrate, tmcRecalibrate);
    loadInt(kDbKeyTmcMultistepFilt, tmcMultistepFilt);
    loadInt(kDbKeyTmcStopEnable, tmcStopEnable);
    loadInt(kDbKeyTmcDiag0Error, tmcDiag0Error);
    loadInt(kDbKeyTmcDiag0Otw, tmcDiag0Otw);
    loadInt(kDbKeyTmcDiag0Stall, tmcDiag0Stall);
    loadInt(kDbKeyTmcDiag1Stall, tmcDiag1Stall);
    loadInt(kDbKeyTmcDiag1Index, tmcDiag1Index);
    loadInt(kDbKeyTmcDiag1Onstate, tmcDiag1Onstate);
    loadInt(kDbKeyTmcDiag1StepsSkipped, tmcDiag1StepsSkipped);
    loadInt(kDbKeyTmcDiag0PushPull, tmcDiag0PushPull);
    loadInt(kDbKeyTmcDiag1PushPull, tmcDiag1PushPull);
    // Hardware uses 5V DIAG through a resistor divider to ESP32, so DIAG must actively drive logic levels.
    tmcDiag0PushPull.set(1);
    tmcDiag1PushPull.set(1);
    loadInt(kDbKeyTmcS2vsLevel, tmcS2vsLevel);
    loadInt(kDbKeyTmcS2gLevel, tmcS2gLevel);
    loadInt(kDbKeyTmcShortFilter, tmcShortFilter);
    loadInt(kDbKeyTmcShortDelay, tmcShortDelay);
    loadInt(kDbKeyTmcBbmTime, tmcBbmTime);
    loadInt(kDbKeyTmcBbmClks, tmcBbmClks);
    loadInt(kDbKeyTmcOtSelect, tmcOtSelect);
    loadInt(kDbKeyTmcDrvStrength, tmcDrvStrength);
    loadInt(kDbKeyTmcFiltIsense, tmcFiltIsense);

    applyTmc2160Config();
    timber.i("TMC2160 settings loaded");
}

bool saveContinuousScreenSettings()
{
    if (!storageReady)
    {
        timber.e("Storage is not ready");
        return false;
    }

    bool changed = false;
    changed |= db.set(kDbKeyContinuousDirection, constDirection.get());
    changed |= db.set(kDbKeyContinuousRps, constRps.get());

    return commitSettings("Continuous", changed);
}

bool saveTmc2160Settings()
{
    if (!storageReady)
    {
        timber.e("Storage is not ready");
        return false;
    }

    bool changed = false;
    changed |= saveInt(kDbKeyTmcDriverChop, tmcDriverChop);
    changed |= saveInt(kDbKeyTmcDriverCurrent, tmcDriverCurrent);
    changed |= saveInt(kDbKeyTmcDriverMicrostep, tmcDriverMicrostep);
    changed |= saveInt(kDbKeyTmcInterpolation, tmcInterpolation);
    changed |= saveInt(kDbKeyTmcFastStandstill, tmcFastStandstill);
    changed |= saveInt(kDbKeyTmcTPwmThrs, tmcTPwmThrs);
    changed |= saveInt(kDbKeyTmcTPowerDown, tmcTPowerDown);
    changed |= saveInt(kDbKeyTmcIrun, tmcIrun);
    changed |= saveInt(kDbKeyTmcIhold, tmcIhold);
    changed |= saveInt(kDbKeyTmcIholdDelay, tmcIholdDelay);
    changed |= saveInt(kDbKeyTmcGlobalScaler, tmcGlobalScaler);
    changed |= saveInt(kDbKeyTmcFreewheel, tmcFreewheel);
    changed |= saveInt(kDbKeyTmcToff, tmcToff);
    changed |= saveInt(kDbKeyTmcTbl, tmcTbl);
    changed |= saveInt(kDbKeyTmcHstrt, tmcHstrt);
    changed |= saveInt(kDbKeyTmcHend, tmcHend);
    changed |= saveInt(kDbKeyTmcChm, tmcChm);
    changed |= saveInt(kDbKeyTmcDedge, tmcDedge);
    changed |= saveInt(kDbKeyTmcVhighfs, tmcVhighfs);
    changed |= saveInt(kDbKeyTmcVhighchm, tmcVhighchm);
    changed |= saveInt(kDbKeyTmcPwmAutoscale, tmcPwmAutoscale);
    changed |= saveInt(kDbKeyTmcPwmAutograd, tmcPwmAutograd);
    changed |= saveInt(kDbKeyTmcPwmOfs, tmcPwmOfs);
    changed |= saveInt(kDbKeyTmcPwmGrad, tmcPwmGrad);
    changed |= saveInt(kDbKeyTmcPwmFreq, tmcPwmFreq);
    changed |= saveInt(kDbKeyTmcPwmReg, tmcPwmReg);
    changed |= saveInt(kDbKeyTmcPwmLim, tmcPwmLim);
    changed |= saveInt(kDbKeyTmcSgt, tmcSgt);
    changed |= saveInt(kDbKeyTmcSfilt, tmcSfilt);
    changed |= saveInt(kDbKeyTmcTCoolThrs, tmcTCoolThrs);
    changed |= saveInt(kDbKeyTmcTHigh, tmcTHigh);
    changed |= saveInt(kDbKeyTmcSemin, tmcSemin);
    changed |= saveInt(kDbKeyTmcSemax, tmcSemax);
    changed |= saveInt(kDbKeyTmcSeup, tmcSeup);
    changed |= saveInt(kDbKeyTmcSedn, tmcSedn);
    changed |= saveInt(kDbKeyTmcSeimin, tmcSeimin);
    changed |= saveInt(kDbKeyTmcVdcMin, tmcVdcMin);
    changed |= saveInt(kDbKeyTmcDcTime, tmcDcTime);
    changed |= saveInt(kDbKeyTmcDcSg, tmcDcSg);
    changed |= saveInt(kDbKeyTmcRecalibrate, tmcRecalibrate);
    changed |= saveInt(kDbKeyTmcMultistepFilt, tmcMultistepFilt);
    changed |= saveInt(kDbKeyTmcStopEnable, tmcStopEnable);
    changed |= saveInt(kDbKeyTmcDiag0Error, tmcDiag0Error);
    changed |= saveInt(kDbKeyTmcDiag0Otw, tmcDiag0Otw);
    changed |= saveInt(kDbKeyTmcDiag0Stall, tmcDiag0Stall);
    changed |= saveInt(kDbKeyTmcDiag1Stall, tmcDiag1Stall);
    changed |= saveInt(kDbKeyTmcDiag1Index, tmcDiag1Index);
    changed |= saveInt(kDbKeyTmcDiag1Onstate, tmcDiag1Onstate);
    changed |= saveInt(kDbKeyTmcDiag1StepsSkipped, tmcDiag1StepsSkipped);
    changed |= saveInt(kDbKeyTmcDiag0PushPull, tmcDiag0PushPull);
    changed |= saveInt(kDbKeyTmcDiag1PushPull, tmcDiag1PushPull);
    changed |= saveInt(kDbKeyTmcS2vsLevel, tmcS2vsLevel);
    changed |= saveInt(kDbKeyTmcS2gLevel, tmcS2gLevel);
    changed |= saveInt(kDbKeyTmcShortFilter, tmcShortFilter);
    changed |= saveInt(kDbKeyTmcShortDelay, tmcShortDelay);
    changed |= saveInt(kDbKeyTmcBbmTime, tmcBbmTime);
    changed |= saveInt(kDbKeyTmcBbmClks, tmcBbmClks);
    changed |= saveInt(kDbKeyTmcOtSelect, tmcOtSelect);
    changed |= saveInt(kDbKeyTmcDrvStrength, tmcDrvStrength);
    changed |= saveInt(kDbKeyTmcFiltIsense, tmcFiltIsense);
    return commitSettings("TMC2160", changed);
}

bool saveVibroScreenSettings()
{
    if (!storageReady)
    {
        timber.e("Storage is not ready");
        return false;
    }

    bool changed = false;
    changed |= db.set(kDbKeyVibroFrequency, vibroFr.get());
    changed |= db.set(kDbKeyVibroAngle, vibroAngle.get());
    changed |= db.set(kDbKeyVibroMicrostep, tmcDriverMicrostep.get());

    return commitSettings("Vibro", changed);
}
