#ifndef STORAGE_H
#define STORAGE_H

bool initStorage();
void tickStorage();
void loadContinuousScreenSettings();
void loadTmc2160Settings();
void loadVibroScreenSettings();
bool saveContinuousScreenSettings();
bool saveTmc2160Settings();
bool saveVibroScreenSettings();

#endif
