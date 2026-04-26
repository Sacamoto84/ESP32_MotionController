#include "observer.h"

#include "tmc2160Config.h"

namespace
{
void observeBasic(State<int32_t>& state)
{
    state.addObserver([](int32_t) { applyTmc2160BasicConfig(); });
}

void observeCurrent(State<int32_t>& state)
{
    state.addObserver([](int32_t) { applyTmc2160CurrentConfig(); });
}

void observeChopper(State<int32_t>& state)
{
    state.addObserver([](int32_t) { applyTmc2160ChopperConfig(); });
}

void observeStealth(State<int32_t>& state)
{
    state.addObserver([](int32_t) { applyTmc2160StealthConfig(); });
}

void observeStallCool(State<int32_t>& state)
{
    state.addObserver([](int32_t) { applyTmc2160StallCoolConfig(); });
}

void observeExpert(State<int32_t>& state)
{
    state.addObserver([](int32_t) { applyTmc2160ExpertConfig(); });
}
} // namespace

void tmc2160SettingsObserver()
{
    observeBasic(tmcFastStandstill);
    observeBasic(tmcTPwmThrs);
    observeBasic(tmcTPowerDown);
    observeBasic(tmcRecalibrate);
    observeBasic(tmcMultistepFilt);
    observeBasic(tmcStopEnable);

    observeCurrent(tmcIrun);
    observeCurrent(tmcIhold);
    observeCurrent(tmcIholdDelay);
    observeCurrent(tmcGlobalScaler);
    observeCurrent(tmcFreewheel);

    observeChopper(tmcToff);
    observeChopper(tmcTbl);
    observeChopper(tmcHstrt);
    observeChopper(tmcHend);
    observeChopper(tmcChm);
    observeChopper(tmcDedge);
    observeChopper(tmcVhighfs);
    observeChopper(tmcVhighchm);

    observeStealth(tmcPwmAutoscale);
    observeStealth(tmcPwmAutograd);
    observeStealth(tmcPwmOfs);
    observeStealth(tmcPwmGrad);
    observeStealth(tmcPwmFreq);
    observeStealth(tmcPwmReg);
    observeStealth(tmcPwmLim);

    observeStallCool(tmcSgt);
    observeStallCool(tmcSfilt);
    observeStallCool(tmcTCoolThrs);
    observeStallCool(tmcTHigh);
    observeStallCool(tmcSemin);
    observeStallCool(tmcSemax);
    observeStallCool(tmcSeup);
    observeStallCool(tmcSedn);
    observeStallCool(tmcSeimin);

    observeExpert(tmcDiag0Error);
    observeExpert(tmcDiag0Otw);
    observeExpert(tmcDiag0Stall);
    observeExpert(tmcDiag1Stall);
    observeExpert(tmcDiag1Index);
    observeExpert(tmcDiag1Onstate);
    observeExpert(tmcDiag1StepsSkipped);
    observeExpert(tmcDiag0PushPull);
    observeExpert(tmcDiag1PushPull);
    observeExpert(tmcS2vsLevel);
    observeExpert(tmcS2gLevel);
    observeExpert(tmcShortFilter);
    observeExpert(tmcShortDelay);
    observeExpert(tmcBbmTime);
    observeExpert(tmcBbmClks);
    observeExpert(tmcOtSelect);
    observeExpert(tmcDrvStrength);
    observeExpert(tmcFiltIsense);
}
