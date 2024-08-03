#include "IO/MelodyPlayer.h"



    void Melody::AddNote(uint32_t tone, uint32_t duration)
    {
        if(toneCount >= MELODY_MAXTONECOUNT) return;
        
        tones[toneCount] = tone;
        toneDurations[toneCount] = duration;
        toneCount++;
    }

    void Melody::AddRest(uint32_t duration)
    {
        AddNote(0, duration);
    }



namespace MelodyPlayer
{
    void t_melodyplayer(void* args);
    
    void Initialize()
    {
        ledcSetup(BUZZER_CHANNEL, 5000, 8);
        ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);

        xTaskCreate(t_melodyplayer, "MelodyPlayer", 2048, NULL, 1, NULL);
    }

    
    Melody melodySlot;
    
    void t_melodyplayer(void* args)
    {
        for(;;)
        {
            if(melodySlot.toneCount == 0)
            {
                delay(10);
                continue;
            }

            Melody currentMelody = melodySlot;
            melodySlot.toneCount = 0;

            for(int i = 0; i < currentMelody.toneCount; i++)
            {
                if(melodySlot.toneCount != 0) break;
                
                ledcWriteTone(BUZZER_CHANNEL, currentMelody.tones[i]);
                delay(currentMelody.toneDurations[i]);
            }

            ledcWriteTone(BUZZER_CHANNEL, 0);
        }
    }


    void PlayMelody(Melody melody)
    {
        melodySlot = melody;
    }
}