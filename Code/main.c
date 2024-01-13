#include<reg52.h>
#include"ds1302.h"
#include"buzzer.h"
#include"temp.h"


#define DIG	P0
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit ledhienthi  =P2^0;
sbit ledbamgio   =P2^1;
sbit leddemnguoc =P2^6;
sbit ledbaothuc  = P2^7;


sbit batdau = P3^0;
sbit mode = P3^1;


unsigned char code DIG_CODE[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
unsigned char Num=0;
unsigned char Mode=0;
unsigned int flag = 0;
unsigned int x1;
unsigned int x2;
unsigned int state = 0;
unsigned int dcount = 0;
unsigned int setPlace = 0;
unsigned int setTime = 0;
unsigned int disp[8]={0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};


unsigned int gio, phut, giay;
unsigned int gio1,gio2;
unsigned int phut1, phut2;
unsigned int giay1, giay2;

unsigned int luugio, luuphut, luugiay;
unsigned int hengio, henphut, hengiay;
unsigned int dembaothuc = 0;
unsigned int ms = 1;

void chonKey(unsigned int k);
void hieuchinh();
void hienthibamgio(unsigned int giay, unsigned int phut, unsigned int gio);
void LooneyToons();
void LcdDisplay(int); //Khai bao ham
void Display();
void chonKey(unsigned int k);
void delayMS(unsigned int ms);



void btn_setup ()
{
	IT0 = 1;
	IT1 = 1;
	EA = 1;
	EX0 = 1;
	EX1 = 1;
	ET0 = 1;
}

//Ham nut nhan 
void btn_check(){
	if (batdau == 0)
  {
			if (x1 == 0)
			{
					state ++;
					if (state >= 2){
						state = 0;
					}
					x1 = 1;
			}
   }
   else
        x1 = 0;
	 if (mode == 0)
  {
			if (x2 == 0)
			{
				state = 0;
				gio = phut = giay = 0;
				setPlace = 0;
				Mode ++;
				if (Mode != 4) ms = 10;
				if (Mode > 4){
						Mode = 0;
				}
					x2 = 1;
			}
   }
   else
        x2 = 0;
}

//ham thoi gian thuc
void Realtime(){
	//kiem tra co phai la lan su dung dau tien khong
	if (setTime == 0){
		hienthibamgio(giay,phut,gio);
		if (state == 1) setTime = 1;
	}
	else if (setTime == 1) {
		//goi ham khoi tao Ds1302 de set thoi gian thuc cho Ds1302
		Ds1302Init();
		setTime = 2;
	}else{
		unsigned int temp;
		Display();
		Ds1302ReadTime(); //Goi ham doc thoi gian thuc
			//Lay cac gia tri tuong ung giay, phut, gio luu vao mang disp.
		disp[0] = DIG_CODE[TIME[0]&0x0f];//0000.1111
		disp[1] = DIG_CODE[TIME[0]>>4];
		disp[2] = 0X40;
		disp[3] = DIG_CODE[TIME[1]&0x0f];
		disp[4] = DIG_CODE[TIME[1]>>4];
		disp[5] = 0X40;
		//state la co set 12h/24h, neu state = 0 thi la 24h
		if(state == 0){
			disp[6] = DIG_CODE[TIME[2]&0X0f];
			disp[7] = DIG_CODE[TIME[2]>>4];
		} else {
			if((TIME[2])> 0x12){
				gio1 = TIME[2]&0x0F;
				gio2 = TIME[2] >> 4;
				temp = (gio2*10 + gio1) - 12;
				gio1 = temp % 10;
				gio2 = temp / 10;
				disp[6] = DIG_CODE[gio1];
				disp[7] = DIG_CODE[gio2];
			}
			else{
				disp[6] = DIG_CODE[TIME[2]&0X0f];
				disp[7] = DIG_CODE[TIME[2]>>4];
			}
		}
	}
}

//Ham hien thi dành cho các mode bamgio, demnguoc,  cai dat bao thuc
void hienthibamgio(unsigned int giay, unsigned int phut, unsigned int gio){
	int i;
	giay1 = giay%10;
	giay2 = giay/10;
	
	phut1 = phut % 10;
	phut2 = phut / 10;
	
	gio1 = gio%10;
	gio2 = gio/10;
	for(i =0; i < 25;i++){
	chonKey(0); 	DIG=DIG_CODE[giay1];
			delayMS(2);
	chonKey(1); 	DIG=DIG_CODE[giay2];
			delayMS(2);
	chonKey(2); 	DIG=DIG_CODE[10];
			delayMS(2);
	chonKey(3); 	DIG=DIG_CODE[phut1];
			delayMS(2);
	chonKey(4); 	DIG=DIG_CODE[phut2];
			delayMS(2);
	chonKey(5); 	DIG=DIG_CODE[10];
			delayMS(2);
	chonKey(6); 	DIG=DIG_CODE[gio1];
			delayMS(2);
	chonKey(7); 	DIG=DIG_CODE[gio2];
			delayMS(2);
	}
}
//ham bam gio, khi goi ham bien giay se tu dong dem len
void Bamgio(){
	giay ++;
	hienthibamgio(giay, phut, gio);
	if (giay >= 59) {
		giay = 0;
		phut++;
	}
	if (phut >= 59){
			phut = 0;
			gio++;
	}
}

//ham dem nguoc, khi goi ham bien giay se tu dong giam ve 0, lan luot den phut va gio
void DemNguoc()
{
    if (giay > 0) {
        giay--;
    } else {
        if (phut > 0) {
            phut--;
            giay = 59;
        } else {
            if (gio > 0) {
                gio--;
                phut = 59;
                giay = 59;
            } else {
                state = 0;
            }
        }
    }
    hienthibamgio(giay, phut, gio);
}

//ham hieu chinh tang so tren dong ho, ung voi cac vi tri giay, phut, gio
//ham tang theo 1 chieu dem len, khi giay,phut den 59 se quay ve 0. Va gio den 23 tang tiep se ve 0
void hieuchinh()
{
	if (setPlace == 0)
	{
		giay++;
		if (giay >= 59 && phut <= 59){
			giay = 0;
			phut++;
		}
	}
	if (setPlace == 1)
	{
		phut++;
		if (phut >= 59 && gio <= 23)
		{
			phut = 0;
			gio++;
		}
	}
	if (setPlace == 2){
		gio++;
		if (gio > 23){
			gio = 0;
		}
	}
}
 /*
ham ghi gia tri vao Ds1302
1. ta se chuyen doi cac gia tri da duoc hieuchinh thanh ma hex
2. Mo bao ve, goi ham ghi vao cac gia tri hex da hieuchinh vao dia chi thanh ghi gio, phut, giay
3. Dong bao ve
*/


void Ds1302Init() {
	// Chuyen doi so hex
    int gio_hex = gio / 10 * 16 + gio % 10;     
    int phut_hex = phut / 10 * 16 + phut % 10;  
    int giay_hex = giay / 10 * 16 + giay % 10;  

    Ds1302Write(0x8E, 0x00);  // Mo bao ve
    Ds1302Write(0x84, gio_hex);   
    Ds1302Write(0x82, phut_hex);  
    Ds1302Write(0x80, giay_hex);  
    Ds1302Write(0x8E, 0x80);  // Dong bao ve
}
//ham bao thuc
/*
1. Cac bien hengio, henphut da duoc luu trong ModeBamgio
2. Goi ham thoi gian thuc Ds1302ReadTime() de so sang voi cac bien hengio, henphut
3. khi khi cac bien thoi gian thuc bang voi bien hen gio thì ráng flag = 1, goi ham phat buzzer 3 lan
*/
void ModeBaoThuc(void)
{
	unsigned int m1, m2, h1, h2;
	unsigned int m,h;
	if (Mode == 3)
{
		hienthibamgio(hengiay,henphut,hengio);
}
	Ds1302ReadTime();
	m1 = TIME[1]&0x0f;
	m2 = TIME[1]>>4;
	m = m2 * 10 + m1;
	h1 = TIME[2]&0x0F;
	h2 = TIME[2] >> 4;
	h = h2*10 + h1;
	if (m == henphut && h == hengio ) flag = 1;
	else flag = 0;
	if (flag == 1) {
		dembaothuc++;
		LooneyToons();
		if (dembaothuc==3){
		henphut=100;
		dembaothuc=0;
		}
	}
}
//ham ngat INT1 dung de tang bien gio,phut,giay
void ISR_INT1 (void) interrupt 2
{
	if( Mode == 1){
		giay = 0;
		phut = 0;
		gio = 0;
	}
	else{
		hieuchinh();
	}
}

//ham ngat ngoai INT0 dung de tang vi tri bien gio,phut,giay
void ISR_INT0 (void) interrupt 0
{
	if (Mode != 4)
	{
		setPlace ++;
		if (setPlace >= 3){
			setPlace = 0;
		}
	}
}
//Ham luu cac mode
void ModeBamGio(){
	hienthibamgio(giay, phut, gio);
	if (state == 1 && Mode == 1){
			Bamgio();
	}
	if (state == 1 && Mode == 2){
			DemNguoc();		
		if (gio == 0 && phut == 0 && giay == 0) flag = 1;
		if (flag == 1)
		{
			LooneyToons();
			flag = 0;
		}	
	}
	if (state == 1 && Mode == 3){
		ModeBaoThuc();
	}
	if (state == 0 && Mode != 0){
		if (Mode == 3){
			hengio = 0;
			henphut = 0;
			hengiay = 0;
				hengio = gio;
				henphut = phut;
				hengiay = giay;
		}
		else{
			luugiay = giay;
			luuphut = phut;
			luugio = gio;
			hienthibamgio(luugiay, luuphut, luugio);
		}
	}
}


void main()
{
	
	ledhienthi = 0; //sang
	ledbamgio = 1; //tat
	leddemnguoc = 1;
	ledbaothuc = 1;
	Mode = 0;
	btn_setup ();
	
	gio=0;
	phut= 0;
	giay= 0;
	batdau = 1;
	mode = 1;
	luugio = 0;
	luuphut = 0;
	luugiay = 0;
	
	while(1)
	{	
		switch(Mode) {
				case 0: ledhienthi =0; ledbamgio = 1; leddemnguoc = 1; ledbaothuc = 1; Realtime(); ModeBaoThuc();
				break;
				case 1:	ledhienthi =1; ledbamgio = 0;	leddemnguoc = 1; ledbaothuc = 1; ModeBamGio();	
				break;
				case 2:	ledhienthi =1; ledbamgio = 1; leddemnguoc = 0; ledbaothuc = 1; ModeBamGio(); 
				break;
				case 3: ledhienthi = 1; ledbamgio = 1; leddemnguoc = 1; ledbaothuc = 0; ModeBamGio(); 
				break;
				case 4: ledhienthi =0; ledbamgio = 1; leddemnguoc = 1; ledbaothuc = 1; LcdDisplay(Ds18b20ReadTemp()); 
				break;
		} 
	}
}


void Display() {
	 int i;
   for(i = 0; i < 8; i++) {
        DIG = disp[i];  
				chonKey(i);
        delayMS(1);  
        DIG = 0x00;  
   }
}
//Ham nay dung de tinh toan va hien thi gia tri nhiet do
void LcdDisplay(int temp) 	 
{
    
  unsigned char datas[] = {0, 0, 0, 0, 0}; 
	float tp;  
	if(temp< 0)	//Neu niet do am		
  	{
        disp[7] = 0x40; //Hien thi dau "-"

		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
    // Tính toan va tra ve gia tri nhiet do
 
  	}
 	else
  	{	
        disp[7] = 0;	// Neu nhiet do duong thì hien thi 0	
		tp=temp;
		temp=tp*0.0625*100+0.5;	

	}
  	disp[0] = 0x39;
		disp[1] = 0x63;
	//	disp[2] = 0;
		disp[3] = DIG_CODE[temp % 100 / 10];
    disp[2] = DIG_CODE[temp % 10];
    disp[4] = DIG_CODE[temp % 1000 / 100] | 0x80; // Them dau '.' vao gia tri
		disp[5] = DIG_CODE[temp % 10000 / 1000];
		disp[6] = DIG_CODE[temp / 10000];
		Display(); 
}

void chonKey(unsigned int k){
	switch (k)
    {
    case (0):
        LSC = 0; LSB = 0; LSA = 0; break;
    case (1):
        LSC = 0; LSB = 0; LSA = 1; break;
    case (2):
        LSC = 0; LSB = 1; LSA = 0; break;
    case (3):
        LSC = 0; LSB = 1; LSA = 1; break;        
    case (4):
        LSC = 1; LSB = 0; LSA = 0; break;        
    case (5):
        LSC = 1; LSB = 0; LSA = 1; break;       
    case (6):
        LSC = 1; LSB = 1; LSA = 0; break;       
    case (7):
        LSC = 1; LSB = 1; LSA = 1; break;        
    }
}

void delayMS (unsigned int t)
{
	TMOD = 0X01;	// timer 0, mode 1 :16bit
	TH0  = 0XFC; 	//8bit trong so cao
	TL0  = 0X18; 	//8bit trong do thap
	IE   = 0x87;	// bat ngat
	dcount =0;
	TR0  = 1;			// start timer 0
	//FFFF+1 - FC18 =1ms
	while (dcount < t);
	ET0   = 0;			// tat tat ca ngat
	TR0  =0;			//clear flag
	// khi TF ==1 gan lai gia ri
}


void ISR_timer0() interrupt 1
{
	TH0 = 0xFC;
	TL0 = 0x18;
	dcount++;
	btn_check();
}
