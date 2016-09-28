#include "DSP2834x_Device.h"
#include "DSP2834x_Examples.h"
#include "math.h"
#include <stdio.h>
#include <string.h>
/**************PWM***************/
#define PowerModule_Index 0xC202
#define PWM_TBTPR 3750 //5000Hz开关频率(三角波)，系统时钟150MHz,系统时钟2分频,
                       //PWM时钟4分频，PWM时钟周期寄存器值为150000000/(5000*2*4)=3750
void InitEPwm1(void);
void InitEPwm2(void);
void InitEPwm3(void);
void InitEPwm4(void);
void SM_PWM_Init(void);
void SM1SwitchOn(void);
void SM1SwitchOff(void);
void SM2SwitchOff(void);
void SM3SwitchOn(void);
void SM3SwitchOff(void);
void SM4SwitchOn(void);
void SM4SwitchOff(void);
void SM1PWM_Out(Uint16 CMP_Value);
void SM2PWM_Out(Uint16 CMP_Value);
void SM3PWM_Out(Uint16 CMP_Value);
void SM4PWM_Out(Uint16 CMP_Value);
void pqcDmaInit(void);

/*********************************/
/**************GPIO***************/
#define GPIO_FUN_00    0
#define GPIO_FUN_01    1
#define GPIO_FUN_10    2
#define GPIO_FUN_11    3
#define GPIO_R_UP      0
#define GPIO_R_DOWN    1
#define GPIO_OUT_UP    1
#define GPIO_OUT_DOWN  0
#define GPIO_DIR_OUT   1
#define GPIO_DIR_IN    0
void SMGpioSet(Uint16 ioNum, Uint16 fun, Uint16 upOrDown, Uint16 driect, Uint16 qsel);
void SMGpioDataSet(Uint16 ioNum, Uint16 highOrlOw);
Uint16 SMGpioDataRead(Uint16 ioNum);
void MOSFETBlock(void);
void ThyristorBlock(void);
void ThyristorPassBy(void);
void Mosfet_Drv_Reset(void);
void LED_ALL_ON(void);
void LED_ALL_OFF(void);
void SM_GPIO_Init(void);

// Function declaration
void ADRead(Uint16 *pEtherCatReBuf);
void ADWrite(Uint16 in_arg);
void InitXintf(void);

/**********************************/
/**************TIMER***************/
void SM_Timer_Init(void);
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer2_isr(void);
/**********************************/
/**************ExINT***************/
void SM_ExInt3_Init(void);
interrupt void Xint3_isr(void);
/*********************************/
/**************EtherCAT***********/
#define AD_START_ADDR (Uint16 *)0x100000
#define AD_DOWNLOAD_DATA_OFFSET 0x00 //0x980
#define AD_UP_DATA_OFFSET 0x0C0 //0x0C0
#define AD_DOWN_DATA_LENGTH 6	//sym：dsp从上位机接收的数据为14个字节12
#define AD_UP_DATA_LENGTH 4	//sym：dsp从上位机接收的数据为14个字节12
extern Uint16 AD_Download_Buf[AD_DOWN_DATA_LENGTH];
#define ECAT_START_ADDR (Uint16 *)0x100000
#define ECAT_UP_DATA_OFFSET 0x900 //0x900
#define ECAT_DOWNLOAD_DATA_OFFSET 0x980 //0x980
#define ECAT_STATUS_OFFSET 0x800
#define ECAT_UP_DATA_LENGTH 42//252	//sym：28335向上位机上传的数据为8个字节
#define ECAT_DOWN_DATA_LENGTH 144	//sym：dsp从上位机接收的数据为14个字节//8
extern Uint16 ECat_UP_Buf[14];	//sym：上传缓冲数组
//extern Uint16 ECat_Download_Buf[ECAT_DOWN_DATA_LENGTH];	//sym：下载缓冲数组
extern Uint16 ProDelayCNT;
extern Uint16 ProJudgeEn;
extern Uint16 ECat_int_clr;
extern Uint16 PWM_SM_InsVoltage;	//模块PWM输出指令电压
extern Uint16 PWM_SM_No;			//PWM逆变模块桥臂编号
extern Uint16 PWM_TestA;		//LYB TEST 2013.10.18
extern Uint16 PWM_TestB;		//LYB TEST 2013.10.18
extern Uint16 PWM_TestC;		//LYB TEST 2013.10.18
extern Uint16 PWM_TestUdc;		//LYB TEST 2013.10.18
extern Uint16 PWM_TestCMD;		//LYB TEST 2013.10.18
extern Uint16 PWM_Test_Leg;		//LYB TEST 2013.10.18
extern Uint16 ModuleCommTest[8];
extern Uint16 FlgModuleCommTest;
extern Uint16 SynFlaging_flag; //PWM同步清零已完成标记，1为已完成



struct SM_FaultStatus1_BIT
{
	Uint16 SM1_CommFault:1;
	Uint16 SM1_UdcOverVol:1;
	Uint16 SM1_UnderVol:1;
	Uint16 SM1_OverCur:1;
	Uint16 SM1_OverCurSD:1;
	Uint16 SM1_DrvFault:1;
	Uint16 SM1_BriShortCur:1;
	Uint16 SM1_TempOver:1;
	Uint16 SM2_CommFault:1;
	Uint16 SM2_UdcOverVol:1;
	Uint16 SM2_UnderVol:1;
	Uint16 SM2_OverCur:1;
	Uint16 SM2_OverCurSD:1;
	Uint16 SM2_DrvFault:1;
	Uint16 SM2_BriShortCur:1;
	Uint16 SM2_TempOver:1;
};
union SM_FaultStatus1
{
	Uint16 all;
	struct SM_FaultStatus1_BIT bit;
};
struct SM_FaultStatus2_BIT
{
	Uint16 SM3_CommFault:1;
	Uint16 SM3_UdcOverVol:1;
	Uint16 SM3_UnderVol:1;
	Uint16 SM3_OverCur:1;
	Uint16 SM3_OverCurSD:1;
	Uint16 SM3_DrvFault:1;
	Uint16 SM3_BriShortCur:1;
	Uint16 SM3_TempOver:1;
	Uint16 SM4_CommFault:1;
	Uint16 SM4_UdcOverVol:1;
	Uint16 SM4_UnderVol:1;
	Uint16 SM4_OverCur:1;
	Uint16 SM4_OverCurSD:1;
	Uint16 SM4_DrvFault:1;
	Uint16 SM4_BriShortCur:1;
	Uint16 SM4_TempOver:1;
};
union SM_FaultStatus2
{
	Uint16 all;
	struct SM_FaultStatus2_BIT bit;
};
struct EcatDownData
{
	Uint16 SM1Udc;
	Uint16 SM2Udc;
	Uint16 SM3Udc;
	Uint16 SM4Udc;
	int16 SM_Cur;
	union SM_FaultStatus1 SM12_fault;
	union SM_FaultStatus2 SM34_fault;
	Uint16 CommIndex;
};
struct MC_SM_S
{
	struct EcatDownData AU;
	struct EcatDownData AV;
	struct EcatDownData AW;
	struct EcatDownData BU;
	struct EcatDownData BV;
	struct EcatDownData BW;
	struct EcatDownData CU;
	struct EcatDownData CV;
	struct EcatDownData CW;
};

struct SM_DI_STATUS_BIT
{

	Uint16 Drv1_STATUS:1;
	Uint16 Drv2_STATUS:1;
	Uint16 Drv3_STATUS:1;
	Uint16 Drv4_STATUS:1;
	Uint16 SM1_ShortCur:1;
	Uint16 SM2_ShortCur:1;
	Uint16 SM3_ShortCur:1;
	Uint16 SM4_ShortCur:1;
	Uint16 SM1_OverTemp:1;
	Uint16 SM2_OverTemp:1;
	Uint16 SM3_OverTemp:1;
	Uint16 SM4_OverTemp:1;
	Uint16 rsvd:4;
};
union DI_STATUS_ALL
{
	Uint16 all;
	struct SM_DI_STATUS_BIT bit;
};
struct DI_CHECK_ALL
{
	union DI_STATUS_ALL DI_NEW;
	Uint16 Drv1_CNT;
	Uint16 Drv2_CNT;
	Uint16 Drv3_CNT;
	Uint16 Drv4_CNT;
	Uint16 SM1_ShortCur_CNT;
	Uint16 SM2_ShortCur_CNT;
	Uint16 SM3_ShortCur_CNT;
	Uint16 SM4_ShortCur_CNT;
	Uint16 SM1_OverTemp_CNT;
	Uint16 SM2_OverTemp_CNT;
	Uint16 SM3_OverTemp_CNT;
	Uint16 SM4_OverTemp_CNT;
};
struct SM_STATUS_DATA
{
	union DI_STATUS_ALL DI_STATUS;
	float32 SM_Cur_RMS;
	float32 Sm_Cur_Ins;
};

struct SM_CONTROL_CMD_BIT
{

	Uint16 Reset:1;
	Uint16 DeBlock:1;
	Uint16 rsvd:13;
};

union SM_CONTROL_CMD
{
	Uint16 all;
	struct SM_CONTROL_CMD_BIT bit;
};

//矩阵变换器调制参数
struct EcatUpData
{
	union SM_CONTROL_CMD SM_CMD;
	int AU_M;
	int AV_M;
	int AW_M;
	int BU_M;
	int BV_M;
	int BW_M;
	int CU_M;
	int CV_M;
	int CW_M;
	Uint16 OverVoltage;
	Uint16 OverCurrent;
	Uint16 rsvd;

};
extern struct EcatUpData ECAT_UPLOAD;
extern struct SM_STATUS_DATA SM_STATUS;
extern struct DI_CHECK_ALL DI_CHECK;
extern struct MC_SM_S MC_SM_DMA;
extern struct MC_SM_S MC_SM;
extern Uint16 CurSampleEn;

Uint16 EcatSatusRead(void);
Uint16 ReadEcatAddr(void);
void EcatInit(void);
void EcatDataInit(void);
void EcatRead(Uint16 *pEtherCatReBuf);
void ECatWrite(void);
void Ecat_DATA_Get(void);
void Ecat_CMD_Re(void);
/**********************************/
