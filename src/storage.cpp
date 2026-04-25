#include "storage.h"

#include <GyverDBFile.h>

#include "global.h"

namespace
{
constexpr char kDbPath[] = "/data.db";
constexpr char kDbKeyVibroFrequency[] = "vibro.frequency";
constexpr char kDbKeyVibroAngle[] = "vibro.angle";
constexpr char kDbKeyVibroMicrostep[] = "vibro.microstep";

GyverDBFile db(&LittleFS, kDbPath);
bool storageReady = false;
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

    bool changed = false;
    changed |= db.set(kDbKeyVibroFrequency, vibroFr.get());
    changed |= db.set(kDbKeyVibroAngle, vibroAngle.get());
    changed |= db.set(kDbKeyVibroMicrostep, tmcDriverMicrostep.get());

    if (!changed)
    {
        timber.i("Vibro settings already saved");
        return true;
    }

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
