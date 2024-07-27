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

namespace MelodyPlayer
{
    void Initialize();

    void PlayMelody(Melody melody);
}