#include "DSP2834x_Device.h"
#include "ZXY_app.h"
#pragma DATA_SECTION(ECat_UP_Buf,"ECATUPDATA");
#pragma DATA_SECTION(AD_UP_Buf,"ADUPDATA");
Uint16 AD_Download_Buf[AD_DOWN_DATA_LENGTH];
Uint16 AD_UP_Buf[AD_UP_DATA_LENGTH];	//sym：上传缓冲数组
Uint16 ECat_UP_Buf[14];	//sym：上传缓冲数组
Uint16 ProDelayCNT;
Uint16 ProJudgeEn;
Uint16 ECat_int_clr;
Uint16 PWM_SM_InsVoltage;	//模块PWM输出指令电压
Uint16 PWM_SM_No;			//PWM逆变模块桥臂编号
Uint16 PWM_TestA;		//LYB TEST 2013.10.18
Uint16 PWM_TestB;		//LYB TEST 2013.10.18
Uint16 PWM_TestC;		//LYB TEST 2013.10.18
Uint16 PWM_TestUdc;		//LYB TEST 2013.10.18
Uint16 PWM_TestCMD;		//LYB TEST 2013.10.18
Uint16 PWM_Test_Leg;		//LYB TEST 2013.10.18
Uint16 ModuleCommTest[8];
Uint16 FlgModuleCommTest;
Uint16 SynFlaging_flag; //PWM同步清零已完成标记，1为已完成
struct MC_SM_S MC_SM;
struct EcatUpData ECAT_UPLOAD;
struct SM_STATUS_DATA SM_STATUS;
struct DI_CHECK_ALL DI_CHECK;
struct MC_SM_S MC_SM_DMA;
int N=0;
int In=1;
//int erro=0;
//int max=0;
int old;
Uint16 EcatSatusRead(void)
{
    Uint16 ret = 100;
    ret  = *(ECAT_START_ADDR+0x800);
    return ret;
}

Uint16 ReadEcatAddr(void)
{
    Uint16 ret = 100;
    ret  = *(ECAT_START_ADDR+0x09);
    return ret;
}

void EcatInit(void)
{
	Uint16 i=0;
	Uint16 ret=0;
	Uint16 *pRead;
	pRead =  ECAT_START_ADDR+ECAT_DOWNLOAD_DATA_OFFSET;
	for(i=0; i<=ECAT_UP_DATA_LENGTH; i++)
	{
		ret = *(pRead++);
		ret = ret;
	}
	ret=0;
    pRead = ECAT_START_ADDR+ECAT_UP_DATA_OFFSET;
	for(i=0; i<=ECAT_DOWN_DATA_LENGTH; i++)
    {
		*(pRead++) = ret;
		ret = ret;
	}
}

void EcatDataInit(void) 
{
	Uint16 i=0;
	//Uint16 ret;
	Uint16 *pRead;
	pRead = ECAT_START_ADDR + ECAT_DOWNLOAD_DATA_OFFSET;
	for(i=0;i<=ECAT_DOWN_DATA_LENGTH;i++) 
		//ret = *(pRead++);
		*(pRead++) = 0;
	pRead = ECAT_START_ADDR + ECAT_UP_DATA_OFFSET;
	for(i=0;i<=ECAT_UP_DATA_LENGTH;i++) 
	//	ret = *(pRead++);
		*(pRead++) = 0;
}

void EcatRead(Uint16 *pEtherCatReBuf)
{
	Uint16 i=0;
	Uint16 ret=0;
	Uint16 *pRead;
	//Uint16 CNT_test=0;
	pRead =  ECAT_START_ADDR+ECAT_DOWNLOAD_DATA_OFFSET;
	for(i = 0; i <ECAT_DOWN_DATA_LENGTH; i++)  //从0读到46
	{
		ret = *(pRead++);
		*(pEtherCatReBuf++) = ret;
	}
}

void ECatWrite(void)
{
	    int *pJY;
		Uint16 temp=0,j;
		static int angel=0;
		float A;
		float B;
		static float TEST1=0;
		static float TEST2=0;
		static float TEST3=0;

		angel++;
		if(angel>200)angel=0;

		A = 1.0;
		B = (((float)angel)*2*3.1415)/200.0;
		TEST1 = A * sin(B);
		TEST1 =(int)(1875+1500*TEST1);
		TEST2 = A * sin(B-2*3.1415/3);
		TEST2 =(int)(1875+1500*TEST2);
		TEST3 = A * sin(B+2*3.1415/3);
		TEST3 =(int)(1875+1500*TEST3);

		ECAT_UPLOAD.AU_M = TEST1;
		ECAT_UPLOAD.AV_M = TEST2;
		ECAT_UPLOAD.AW_M = TEST3;
		ECAT_UPLOAD.OverVoltage = 1000;
		ECAT_UPLOAD.OverCurrent = 200;
		ECAT_UPLOAD.rsvd = angel+100;

		ECat_UP_Buf[0] =ECAT_UPLOAD.SM_CMD.all;
		ECat_UP_Buf[1] =ECAT_UPLOAD.AU_M;
		ECat_UP_Buf[2] =ECAT_UPLOAD.AV_M;
		ECat_UP_Buf[3] =ECAT_UPLOAD.AW_M;
		ECat_UP_Buf[4] =ECAT_UPLOAD.BU_M;
		ECat_UP_Buf[5] =ECAT_UPLOAD.BV_M;
		ECat_UP_Buf[6] =ECAT_UPLOAD.BW_M;
		ECat_UP_Buf[7] =ECAT_UPLOAD.CU_M;//(unsigned int)100*((float)((unsigned int)(32768 + (signed int)AD_Download_Buf[0]))/65535);
		ECat_UP_Buf[8] =ECAT_UPLOAD.CV_M;
		ECat_UP_Buf[9] =ECAT_UPLOAD.CW_M;
		ECat_UP_Buf[10] = ECAT_UPLOAD.OverVoltage;
		ECat_UP_Buf[11] = ECAT_UPLOAD.OverCurrent;
		ECat_UP_Buf[12] = ECAT_UPLOAD.rsvd;

		pJY = &ECAT_UPLOAD.SM_CMD.all;
		for(j = 0; j <13; j++)  //
		{
			temp += *(pJY++);

		}
		ECat_UP_Buf[13] = temp;
}

void Ecat_DATA_Get(void)
{
	MemCopy(&MC_SM_DMA.AU.SM1Udc,&MC_SM_DMA.CW.CommIndex,&MC_SM.AU.SM1Udc);
	EALLOW;
	DmaRegs.CH1.CONTROL.bit.PERINTFRC=1;
	EDIS;
}



void ADRead(Uint16 *pEtherCatReBuf)
{
	Uint16 i=0;
	Uint16 ret=0;
	Uint16 *pRead;
	//Uint16 CNT_test=0;
	pRead =  AD_START_ADDR+AD_DOWNLOAD_DATA_OFFSET;
	for(i = 0; i <AD_DOWN_DATA_LENGTH; i++)  //从0读到46
	{
		ret = *(pRead++);
		*(pEtherCatReBuf++) = ret;
	}
}
void ADWrite(Uint16 in_arg)
{
	Uint16 *pWrite;
	Uint16 i=0;
	Uint16 ret=0;
	if (in_arg > 4096)
		in_arg = 4096;
	else if (in_arg < 0)
		in_arg = 0;

	AD_UP_Buf[0] = in_arg;
	AD_UP_Buf[1] = in_arg;
	AD_UP_Buf[2] = in_arg;
	AD_UP_Buf[3] = in_arg;
	pWrite = AD_START_ADDR+AD_UP_DATA_OFFSET;
	for(i=0; i<AD_UP_DATA_LENGTH; i++)
	{
		ret =AD_UP_Buf[i];
		*(pWrite++) = ret;
	}
}
