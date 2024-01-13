#include "ds1302.h"

uchar TIME[3] = {0, 0, 0x12};

/*******************************************************************************
* Ham           : Ds1302Write
* Chuc nang      : Gui lenh va du lieu toi DS1302 (Dia chi + Du lieu)
* Tham so dau vao : addr, dat
* Tham so dau ra  : Khong
*******************************************************************************/

void Ds1302Write(uchar addr, uchar dat)
{    
	uchar n;

    EA = 0;
	RST = 0;
	_nop_();

	SCLK = 0;  // Dat SCLK ve muc thap
	_nop_();
	RST = 1;  // Sau do dat RST(CE) ve muc cao
	_nop_();

	for (n = 0; n < 8; n++)  // Bat dau gui dia chi lenh tu bit thap nhat
	{
		DSIO = addr & 0x01;
		addr >>= 1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n = 0; n < 8; n++)  // Gui 8 bit du lieu
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;  // Ket thuc viec gui du lieu
	_nop_();
    EA = 1;
}

/*******************************************************************************
* Ham           : Ds1302Read
* Chuc nang      : Doc du lieu tu mot dia chi cu the cua DS1302
* Tham so dau vao : addr
* Tham so dau ra  : dat
*******************************************************************************/

uchar Ds1302Read(uchar addr)
{   
    uchar n, dat, dat1;
    
    EA = 0;
	RST = 0;
	_nop_();

	SCLK = 0;
	_nop_();
	RST = 1;
	_nop_();

	for (n = 0; n < 8; n++)  // Bat dau gui dia chi lenh tu bit thap nhat
	{
		DSIO = addr & 0x01;
		addr >>= 1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}
	_nop_();
	for (n = 0; n < 8; n++)  // Doc 8 bit du lieu
	{
		dat1 = DSIO;
		dat = (dat >> 1) | (dat1 << 7);
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}

	RST = 0;
	_nop_();
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
    EA = 1;
	return dat;	
}

/*******************************************************************************
* Ham           : Ds1302ReadTime
* Chuc nang      : Doc thoi gian tu DS1302
* Tham so dau vao : Khong
* Tham so dau ra  : Khong
*******************************************************************************/

void Ds1302ReadTime()
{
	TIME[0] = Ds1302Read(0x81);
	TIME[1] = Ds1302Read(0x83);
	TIME[2] = Ds1302Read(0x85);
}
