#include<reg52.h>
#include "buzzer.h"
sbit melody = P2^5;
//Dinh nghia cac not nhac
#define pitch_A5	64968
#define pitch_C6	65058
#define pitch_D6	65110
#define pitch_Eb6	65134
#define pitch_E6	65157
#define pitch_F6	65178

#define MelodyPin	melody

//Bien luu gia tri thoi gian cho cac loai nhip am nhac
unsigned int qtrN, eighthN;

//Ham tinh thoi gian cho moi nhip dua tren thoi gian Tempo
void MelodyTempo(int Tempo){
	qtrN = ((60000/Tempo));
	eighthN = (qtrN/2);
}

//Ham PlayNote duoc su dung de choi mot not nhac voi tan so duoc xac dinh boi 'PitchH' va 'PitchL;'
void PlayNote(const unsigned char PitchH, const unsigned char PitchL){
	RCAP2H = PitchH;
	RCAP2L = PitchL;
	TH2 = PitchH;
	TL2 = PitchL;
	TF2 = 0;
	TR2 = 1;
	ET2 = 1;
	EA = 1;
}
// Dung de ket thuc 
void EndNote(void){
	ET2 = 0;
	TR2 = 0;
	TF2 = 0;
	MelodyPin = 1;
}

void delay_ms(unsigned int ms){
	int i;
	for(;ms>0;ms--)
		for(i = 0; i < 123;i++){}
}

//Ham nay choi mot not nhac voi tan so 'Pitch' va do tre giua cac not nhac la 'interval'
void Melodyplay(const int Pitch, unsigned int interval){
	if(Pitch!=0)
		PlayNote(Pitch >> 8, Pitch);
	delay_ms(interval);
	EndNote();
}
//Ham nay dinh nghia mot doan nhac su dung cac not nhac duoc dinh nghia truoc
//voi thoi gian tempo va do tre giua cac not nhac da duoc xac dinh
void LooneyToons(){ //d=4,o=5,b=140
	MelodyTempo(140);
	Melodyplay(pitch_C6, qtrN); //c6
	Melodyplay(pitch_F6, eighthN); //8f6
	Melodyplay(pitch_E6, eighthN); //8e6
	Melodyplay(pitch_D6, eighthN); //8d6
	Melodyplay(pitch_C6, eighthN); //8c6
	Melodyplay(pitch_A5, qtrN); //a.
	Melodyplay(pitch_A5, eighthN); //a.
	Melodyplay(pitch_C6, eighthN); //8c6
	Melodyplay(pitch_F6, eighthN); //8f6
	Melodyplay(pitch_E6, eighthN); //8e6
	Melodyplay(pitch_D6, eighthN); //8d6
	Melodyplay(pitch_Eb6, eighthN); //8d#6
	Melodyplay(pitch_E6, qtrN); //e.6
	
}
// ham nay lam thay doi trang thai bien MelodyPin de tao ra am thanh
void T2ISR(void) interrupt 5 {
	MelodyPin = ~MelodyPin;
	TF2 = 0;
}

