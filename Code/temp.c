#include"temp.h"


void Delay1ms(unsigned int y)
{
	unsigned int x;
	for(y;y>0;y--)
		for(x=110;x>0;x--);
}


unsigned char Ds18b20Init()
{
	unsigned int i;
    EA = 0;
	DSPORT=0;			 // Gan DSPORT = 0
	i=70;	
	while(i--);//Vong lap tao do tre tuong duong 70 lan lap
	DSPORT=1;			// Gan DSPORT = 1
	i=0;          // Gan i = 0
	while(DSPORT)	//Vong lap kiem tra DSPORT
	{
		i++;
		if(i>5000)//
			return 0;//Ham tra ve gia tri 0 khi DSPORT van bang 1
	}
	return 1;  //Ham tra ve gia tri 1 khi DSPORT bang 0
}



void Ds18b20WriteByte(unsigned char dat)  //Khai bao ham Ds18b20WriteByte voi nhan mot tham so la dat
{
	unsigned int i,j;
    EA = 0;
	for(j=0;j<8;j++) //Vong lap 8 lan de thuc hien ghi tung bitt cua byte du lieu
	{
		DSPORT=0;			//Gan DSPORT bang 0
		i++;          //Tang bien i, tao do tre
		DSPORT=dat&0x01; //Ghi bitt thap nhat cua cua 'dat' vao 'DSPORT'
		i=6;          //Gan i bang 6
		while(i--); //Vong lap 6 lan, tao do tre
		DSPORT=1;	//Gan DSPORT bang 1
		dat>>=1;  //Dich phai bien dat
	}
    EA = 1;
}



unsigned char Ds18b20ReadByte()
{
	unsigned char byte,bi;
	unsigned int i,j;
    EA = 0;	
	for(j=8;j>0;j--)
	{
		DSPORT=0;//Gan DSPORT bang 0
		i++;     //Tang bien i, tao do tre
		DSPORT=1;//Gan DSPORT bang 1
		i++;
		i++;
		bi=DSPORT;	 //Gan bien bi = DSPORT
		byte=(byte>>1)|(bi<<7);	//Dich phai gia tri cua byte va gan bitt thap nhat cua bi vao bitt cao nhat cua byte				  
		i=4;		
		while(i--);// Tao do tre truoc khi doc bitt tiep theo
	}
    EA = 1;				
	return byte;// Tra ve gia tri cua bien byte
}


void  Ds18b20ChangTemp()
{
	Ds18b20Init(); //Goi ham Ds18b20Init()
	Delay1ms(1);  //Tao delay 1ms
	Ds18b20WriteByte(0xcc);		//Gui lenh '0xCC' de chuyen cam bien DS18B20 sang che do lua chon ROM (Read-ROM) 
	Ds18b20WriteByte(0x44);	    //Gui lenh '0x44' de yei cau cam bien thuc hien do va cap nhat nhiet do
	Delay1ms(10);	
   
}


void  Ds18b20ReadTempCom()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //Gui lenh '0xCC' de chuyen cam bien DS18B20 sang che do lua chon ROM (Read-ROM)
	Ds18b20WriteByte(0xbe);	 //Gui lenh'0xBE' de yeu cau cam bien thuc hien do va truyen nhiet do
}


int Ds18b20ReadTemp()
{
	int temp=0;
	unsigned char tmh,tml;
	Ds18b20ChangTemp();			 	//de chuan bi cam bien de thuc hien do va cap nhat nhiet do moi
	Ds18b20ReadTempCom();			//de chuan bi cam bien de thuc hien qua trinh doc nhiet do
	tml=Ds18b20ReadByte();		//Doc byte thap cua gia tri nhiet do tu cam bien va luu vao bien 'tml'
	tmh=Ds18b20ReadByte();		//Doc byte cao cua gia tri nhiet do tu cam bien va luu vao bien 'tml'
	temp=tmh;
	temp<<=8;
	temp|=tml;// Ket hop byte thap va byte cao de tao thanh gia tri nhiet do hoan chinh
	return temp;// Tra ve gia tri nhiet do da doc duoc, duoc luu trong bien 'temp'
}


