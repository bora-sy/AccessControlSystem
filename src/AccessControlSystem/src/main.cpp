#include "IO/CardReader.h"
#include "IO/MelodyPlayer.h"


Melody m1;

void setup()
{
  Serial.begin(921600);
  CardReader::Initialize();
  MelodyPlayer::Initialize();

  m1.AddNote(1500, 1000);
  m1.AddNote(750, 200);
  m1.AddNote(1500, 1000);
  m1.AddNote(750, 200);
}

void loop()
{
    Serial.println("Playing Melody");
    MelodyPlayer::PlayMelody(m1);
    Serial.println("---");
    delay(5000);
}