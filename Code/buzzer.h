#ifdef _BUZZER_H_
#define _BUZZER_H_


void MelodyTempo(int Tempo);
void PlayNote(const unsigned char PitchH, const unsigned char PitchL);
void EndNote(void);
void Melodyplay(const int Pitch, unsigned int interval);
void LooneyToons();
void delay_ms(unsigned int ms);

#endif