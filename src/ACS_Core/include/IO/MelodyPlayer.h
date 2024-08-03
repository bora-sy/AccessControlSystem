#pragma once

#include <Arduino.h>
#include "Pinout.h"

#define MELODY_MAXTONECOUNT 100

#define BUZZER_CHANNEL 1

struct Melody
{
    uint32_t tones[MELODY_MAXTONECOUNT];
    uint32_t toneDurations[MELODY_MAXTONECOUNT];
    uint16_t toneCount = 0;

    void AddNote(uint32_t tone, uint32_t duration);

    void AddRest(uint32_t duration);
};


class MelodyPlayer
{
    private:
    static bool alarm;
    static Melody melodySlot;

    static void t_melodyplayer(void *args);

    static void alarmCycle();
    public:
    
    static void Initialize();

    static void PlayMelody(Melody melody);

    static void SetAlarm(bool state);
};