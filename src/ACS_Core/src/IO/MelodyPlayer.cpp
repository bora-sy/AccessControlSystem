#include "IO/MelodyPlayer.h"

void Melody::AddTone(uint32_t tone, uint32_t duration)
{
    if (toneCount >= MELODY_MAXTONECOUNT)
        return;

    tones[toneCount] = tone;
    toneDurations[toneCount] = duration;
    toneCount++;
}

void Melody::AddNote(note_t note, uint8_t octave, uint32_t duration)
{
    const uint16_t noteFrequencyBase[12] = {
    //   C        C#       D        Eb       E        F       F#        G       G#        A       Bb        B
        4186,    4435,    4699,    4978,    5274,    5588,    5920,    6272,    6645,    7040,    7459,    7902
    };

    if(octave > 8 || note >= NOTE_MAX)
    {
        REMOTELOG_W("Invalid note or octave (Note: %d / Octave: %d)", note, octave);
        return;
    }

    uint32_t noteFreq =  (uint32_t)noteFrequencyBase[note] / (uint32_t)(1 << (8-octave));
    AddTone(noteFreq, duration);
}

void Melody::AddRest(uint32_t duration)
{
    AddTone(0, duration);
}

//----------------------------------------------

Melody MelodyPlayer::melodySlot;
bool MelodyPlayer::alarm = false;

void MelodyPlayer::Initialize()
{
    ledcSetup(BUZZER_CHANNEL, 5000, 8);
    ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);

    xTaskCreate(t_melodyplayer, "MelodyPlayer", 4096, NULL, 1, NULL);
}

void MelodyPlayer::PlayMelody(Melody melody)
{
    melodySlot = melody;
}

void MelodyPlayer::SetAlarm(bool state)
{
    REMOTELOG_D("Alarm State: %d", state);
    alarm = state;
}

void MelodyPlayer::t_melodyplayer(void *args)
{
    for (;;)
    {
        if (alarm)
        {
            // 730MS
            for (int i = 0; i < 10; i++)
            {
                int delayms = 150 / (i + 1);
                ledcWriteTone(1, 2500);
                delay(30);
                ledcWriteTone(1, 0);
                delay(delayms);
                if(!alarm)
                    break;
            }

            while (alarm)
                alarmCycle();
        }

        Melody currentMelody = melodySlot;
        melodySlot.toneCount = 0;

        for (int i = 0; i < currentMelody.toneCount; i++)
        {
            if (melodySlot.toneCount != 0)
            {
                ledcWriteTone(BUZZER_CHANNEL, 0);
                REMOTELOG_W("Melodies conflicted. Aborting previous melody.");
                delay(100);
                break;
            }

            ledcWriteTone(BUZZER_CHANNEL, currentMelody.tones[i]);
            delay(currentMelody.toneDurations[i]);
            if (alarm)
            {
                REMOTELOG_D("Alarm mode active, aborting current melody.");
                break;
            }
        }

        ledcWriteTone(BUZZER_CHANNEL, 0);
        delay(5);
    }
}

void MelodyPlayer::alarmCycle()
{
    for (int i = 25; i < 40; i++)
    {
        ledcWriteTone(BUZZER_CHANNEL, i * 100);
        delay(50);
    }
}