#include "storage.h"

#include <GyverDBFile.h>

#include "global.h"

namespace
{
constexpr char kDbPath[] = "data.db";
constexpr char kDbKeyVibroFrequency[] = "vibro.frequency";
constexpr char kDbKeyVibroAngle[] = "vibro.angle";
constexpr char kDbKeyVibroMicrostep[] = "vibro.microstep";

GyverDBFile db(&LittleFS, kDbPath);
bool storageReady = false;
}

bool initStorage()
{
    storageReady = db.begin();

    if (storageReady)
    {
        timber.i("Storage ready: %s", kDbPath);
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

bool saveVibroScreenSettings()
{
    if (!storageReady)
    {
        timber.e("Storage is not ready");
        return false;
    }

    db.set(kDbKeyVibroFrequency, vibroFr.get());
    db.set(kDbKeyVibroAngle, vibroAngle.get());
    db.set(kDbKeyVibroMicrostep, tmcDriverMicrostep.get());

    const bool saved = db.update();
    if (saved)
    {
        timber.i("Vibro settings saved");
    }
    else
    {
        timber.e("Vibro settings save failed");
    }
    return saved;
}
