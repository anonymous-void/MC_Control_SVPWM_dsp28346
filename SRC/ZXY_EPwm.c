//sym：???为什么都不适用Shadow寄存器了，还要设置LoadMode为过零载入？？？
/*sym:  更改：
 * 		1. EPwmxRegs.TBCTL.bit.PHSEN =  TB_ENABLE; instead of 'TB_DISABLE'
 * 		2. TBPHS.half.TBPHS 分别为3750的0.25、0.5、0.75
*/
#include "DSP2834x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2834x_Examples.h"   // DSP2833x Examples Include File
#include "DSP2834x_EPwm_defines.h"
#include "ZXY_app.h"

#define	SYM_DB_COUNTER	150

void InitEPwm1(void)
{



 // Setup TBCLK
   EPwm1Regs.TBPRD = PWM_TBTPR;           // Set timer period 801 TBCLKs
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
//   EPwm1Regs.TBPHS.all = 0;
   EPwm1Regs.TBCTR = 0x0000;                      // Clear counter
   
   // Set Compare values
   EPwm1Regs.CMPA.half.CMPA = 500;     // Set compare A value
   EPwm1Regs.CMPB = 500;               // Set Compare B value
   
   // Setup counter mode	//SYSCLKOUT = 30MHz * 10 / 2 = 150MHz  T = 1 / 150MHz = 6.67ns
   // TBCLK = SYSCLKOUT / (HSPCLKDIV * CLKDIV) = 150MHz / (1 * 4) = 37.5MHz  T = 0.02667us = 26.67ns
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; //上下计数：三角波！！！
   EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE; //TB_DISABLE;// Disable phase loading
   EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;	//sym:当TimeBase计数器到0时会在Sync输出端输出一个同步脉冲

   // Setup shadowing
//???为什么都不使用Shadow寄存器了，还要设置LoadMode为过零载入？？？
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	//不使用Shadow寄存器
//   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
 //  EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

   if (((PowerModule_Index & 0x0F00) == 0x0200) || ((PowerModule_Index & 0x0F00) == 0x0400)) //下桥臂
   {
   // Set actions
   	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count,output High,SM output 0
   	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;           // Clear PWM1A on event A, down count, output low, SM ouput Udc

   EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;             // Set PWM1B on event B, up count
   EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;           // Clear PWM1B on event B, down count
   }
   else ///上桥臂	//上下桥臂的动作是完全对称的。
   {
		EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;			  // Set PWM1A on event A, up count,output low, SM ouput Udc
		EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR; 		  // Clear PWM1A on event A, down count, output High,SM output 0

		EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			  // Set PWM1B on event B, up count
		EPwm1Regs.AQCTLB.bit.CBD = AQ_SET; 		  // Clear PWM1B on event B, down count

   }

      // Active Low PWMs - Setup Deadband
   EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
   //EPwm1Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
   EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
   EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
   EPwm1Regs.DBRED = SYM_DB_COUNTER;//400;	//死区的上升沿延时
   EPwm1Regs.DBFED = SYM_DB_COUNTER;//400;	//死区的下降沿延时

/******************************************
   EPwm1Regs.AQCSFRC.bit.CSFA = 0x1;
   EPwm1Regs.AQCSFRC.bit.CSFB = 0x1;
*******************************************/
   EPwm1Regs.AQSFRC.bit.RLDCSF = 0x11; //Temp Test

/*

  // Interrupt where we will change the Deadband
  // EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
  // EPwm1Regs.ETSEL.bit.INTEN = 0;                // Enable INT
  // EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

*/
}


void InitEPwm2(void)
{

   
   // Setup TBCLK
	 EPwm2Regs.TBPRD = PWM_TBTPR;		   // Set timer period 801 TBCLKs
	 EPwm2Regs.TBPHS.half.TBPHS = PWM_TBTPR*1/2;		// Phase is 0
//	 EPwm2Regs.TBPHS.all = 938;
	 EPwm2Regs.TBCTR = 0x0000;						// Clear counter
	 
	 // Set Compare values
	 EPwm2Regs.CMPA.half.CMPA = 500;	 // Set compare A value
	 EPwm2Regs.CMPB = 500;				 // Set Compare B value
	 
	 // Setup counter mode
	 EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	 EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE; // TB_DISABLE;		// Disable phase loading
	 EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN;
	 EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	 EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Clock ratio to SYSCLKOUT
	 EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV4;
	 EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
   
	 // Setup shadowing
	 EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	 EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// Load on Zero
	 EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	 
   
   
	 // Set actions
   if (((PowerModule_Index & 0x0F00) == 0x0200) || ((PowerModule_Index & 0x0F00) == 0x0400)) //下桥臂
   	{
   // Set actions
   	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count,output High,SM output 0
   	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;           // Clear PWM1A on event A, down count, output low, SM ouput Udc
   
	 EPwm2Regs.AQCTLB.bit.CBU = AQ_SET; 			// Set PWM1B on event B, up count
	 EPwm2Regs.AQCTLB.bit.CBD = AQ_CLEAR;			// Clear PWM1B on event B, down count
   	}
	   else ///上桥臂
	   	{
		   EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;			  // Set PWM1A on event A, up count,output low, SM ouput Udc 
		   EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR; 		  // Clear PWM1A on event A, down count, output High,SM output 0
   
		   EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;			  // Set PWM1B on event B, up count
		   EPwm2Regs.AQCTLB.bit.CBD = AQ_SET; 		  // Clear PWM1B on event B, down count
   
	   	} 
   
   
		// Active Low PWMs - Setup Deadband
	 EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	 //EPwm2Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	 EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	 EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	 EPwm2Regs.DBRED = SYM_DB_COUNTER;//400;
	 EPwm2Regs.DBFED = SYM_DB_COUNTER;//400;
   
/******************************************
	 EPwm2Regs.AQCSFRC.bit.CSFA = 0x1;
	 EPwm2Regs.AQCSFRC.bit.CSFB = 0x1;
******************************************/
	 EPwm2Regs.AQSFRC.bit.RLDCSF = 0x11; //Temp Test

   

   // Interrupt where we will modify the deadband
  // EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Zero event
  // EPwm2Regs.ETSEL.bit.INTEN = 0;                 // Enable INT
  // EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;            // Generate INT on 3rd event

}

void InitEPwm3(void)
{

   
   // Setup TBCLK
	 EPwm3Regs.TBPRD = PWM_TBTPR;		   // Set timer period 801 TBCLKs
	 EPwm3Regs.TBPHS.half.TBPHS = PWM_TBTPR*1;	// Phase is 180
	 EPwm3Regs.TBCTR = 0x0000;						// Clear counter
	 
	 // Set Compare values
	 EPwm3Regs.CMPA.half.CMPA = 500;	 // Set compare A value
	 EPwm3Regs.CMPB = 500;				 // Set Compare B value
	 
	 // Setup counter mode
	 EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	 EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE; // TB_DISABLE;		// Disable phase loading
	 EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
	 EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	 EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Clock ratio to SYSCLKOUT
	 EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV4;
	 EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
   
	 // Setup shadowing
	 EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	 EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// Load on Zero
	 EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	 
   
   
	 // Set actions
	 if (((PowerModule_Index & 0x0F00) == 0x0200) || ((PowerModule_Index & 0x0F00) == 0x0400)) //下桥臂
	  {
	 // Set actions
	  EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;			 // Set PWM1A on event A, up count,output High,SM output 0
	  EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;			 // Clear PWM1A on event A, down count, output low, SM ouput Udc
   
	 EPwm3Regs.AQCTLB.bit.CBU = AQ_SET; 			// Set PWM1B on event B, up count
	 EPwm3Regs.AQCTLB.bit.CBD = AQ_CLEAR;			// Clear PWM1B on event B, down count
	  }
		 else ///上桥臂
		  {
			 EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;			// Set PWM1A on event A, up count,output low, SM ouput Udc 
			 EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR; 		// Clear PWM1A on event A, down count, output High,SM output 0
			 
			 EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;			// Set PWM1B on event B, up count
			 EPwm3Regs.AQCTLB.bit.CBD = AQ_SET; 		// Clear PWM1B on event B, down count
	 
		  }

   
   
   
   
		// Active Low PWMs - Setup Deadband
	 EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	 //EPwm3Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	 EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	 EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	 EPwm3Regs.DBRED = SYM_DB_COUNTER;//400;
	 EPwm3Regs.DBFED = SYM_DB_COUNTER;//400;
   
/******************************************
	 EPwm3Regs.AQCSFRC.bit.CSFA = 0x1;
	 EPwm3Regs.AQCSFRC.bit.CSFB = 0x1;
******************************************/
	 EPwm3Regs.AQSFRC.bit.RLDCSF = 0x11; //Temp Test


   // Interrupt where we will change the deadband
  // EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       // Select INT on Zero event
  // EPwm3Regs.ETSEL.bit.INTEN = 0;                  // Enable INT
   // EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;             // Generate INT on 3rd event

}

void InitEPwm4(void)
{

   
   // Setup TBCLK
	 EPwm4Regs.TBPRD = PWM_TBTPR;		   // Set timer period 801 TBCLKs
	 EPwm4Regs.TBPHS.half.TBPHS = PWM_TBTPR*1/2;	// Phase is 0
	 EPwm4Regs.TBCTR = 0x0000;						// Clear counter
	 
	 // Set Compare values
	 EPwm4Regs.CMPA.half.CMPA = 500;	 // Set compare A value
	 EPwm4Regs.CMPB = 500;				 // Set Compare B value
	 
	 // Setup counter mode
	 EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	 EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE; // TB_DISABLE;		// Disable phase loading
	 EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;
	 EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	 EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Clock ratio to SYSCLKOUT
	 EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV4;
	 EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
   
	 // Setup shadowing
	 EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	 EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	 EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// Load on Zero
	 EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	 
   
   
	 // Set actions
   if (((PowerModule_Index & 0x0F00) == 0x0200) || ((PowerModule_Index & 0x0F00) == 0x0400)) //下桥臂
   	{
   // Set actions
   	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM1A on event A, up count,output High,SM output 0
   	EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;           // Clear PWM1A on event A, down count, output low, SM ouput Udc
   
	 EPwm4Regs.AQCTLB.bit.CBU = AQ_SET; 			// Set PWM1B on event B, up count
	 EPwm4Regs.AQCTLB.bit.CBD = AQ_CLEAR;			// Clear PWM1B on event B, down count
   	}
	   else ///上桥臂
	   	{
		   EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;			  // Set PWM1A on event A, up count,output low, SM ouput Udc 
		   EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR; 		  // Clear PWM1A on event A, down count, output High,SM output 0
   
		   EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;			  // Set PWM1B on event B, up count
		   EPwm4Regs.AQCTLB.bit.CBD = AQ_SET; 		  // Clear PWM1B on event B, down count
   
	   	} 
   
   
		// Active Low PWMs - Setup Deadband
	 EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	 //EPwm4Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;
	 EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	 EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	 EPwm4Regs.DBRED = SYM_DB_COUNTER;//400;
	 EPwm4Regs.DBFED = SYM_DB_COUNTER;//400;
   
/******************************************
	 EPwm4Regs.AQCSFRC.bit.CSFA = 0x1;
	 EPwm4Regs.AQCSFRC.bit.CSFB = 0x1;
******************************************/
	 EPwm4Regs.AQSFRC.bit.RLDCSF = 0x11;	//Temp Test

  

   // Interrupt where we will change the deadband
  // EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       // Select INT on Zero event
  // EPwm4Regs.ETSEL.bit.INTEN = 0;                  // Enable INT
   // EPwm4Regs.ETPS.bit.INTPRD = ET_3RD;             // Generate INT on 3rd event

}

void SM_PWM_Init(void)
{
	EALLOW;
	
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	InitEPwm1( );
	InitEPwm2( );
	InitEPwm3( );
	InitEPwm4( );

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;
}

void SM1SwitchOn(void)
{//sym: Action-qualifier Continuous Software Force Register
//CSFA & CSFB
	// 00: Forcing disabled--has no effect
	// 01: Forces a continuous LOW   on output port A or B
	// 10: Forces a continuous HIGHT on output port A or B
	// 11: Software forcing is Disabled---has no effect

	EALLOW;	
	//sym：下面这两条语句用于选择是子模块上面的MOSFET导通，还是
	//下面的MOSFET导通
   EPwm1Regs.AQCSFRC.bit.CSFA = 1;//01B
   EPwm1Regs.AQCSFRC.bit.CSFB = 2;//10B
	EDIS;	
	
}

void SM1SwitchOff(void)
{
   EALLOW;
   EPwm1Regs.AQCSFRC.bit.CSFA = 2;
   EPwm1Regs.AQCSFRC.bit.CSFB = 1;
   EDIS;

	
	
}

void SM2SwitchOn(void)
{
	EALLOW;
	EPwm2Regs.AQCSFRC.bit.CSFA = 1;
    EPwm2Regs.AQCSFRC.bit.CSFB = 2;
	EDIS;	
}

void SM2SwitchOff(void)
{
   EALLOW;
   EPwm2Regs.AQCSFRC.bit.CSFA = 2;
   EPwm2Regs.AQCSFRC.bit.CSFB = 1;
   EDIS;	
	
}

void SM3SwitchOn(void)
{
	EALLOW;
	EPwm3Regs.AQCSFRC.bit.CSFA = 1;
    EPwm3Regs.AQCSFRC.bit.CSFB = 2;
	EDIS;
	
	
}

void SM3SwitchOff(void)
{
	EALLOW;
	EPwm3Regs.AQCSFRC.bit.CSFA = 2;
    EPwm3Regs.AQCSFRC.bit.CSFB = 1;
	EDIS;	
	
}

void SM4SwitchOn(void)
{
	EALLOW;
	EPwm4Regs.AQCSFRC.bit.CSFA = 1;
    EPwm4Regs.AQCSFRC.bit.CSFB = 2;
	EDIS;
	
}

void SM4SwitchOff(void)
{
	EALLOW;
	EPwm4Regs.AQCSFRC.bit.CSFA = 2;
    EPwm4Regs.AQCSFRC.bit.CSFB = 1;
	EDIS;
	
}

void SM1PWM_Out(Uint16 CMP_Value)
{// SMxPWM_Out()函数的作用就是更新CMP寄存器的值
//  同时，把AQCSFRC给disable了
	EALLOW;

//	EPwm1Regs.TBCTR = 0x0000;					   // Clear counter LYB 2013.10.18 test
	
	// Set Compare values
	EPwm1Regs.CMPA.half.CMPA = CMP_Value; 	// Set compare A value
	EPwm1Regs.CMPB = CMP_Value;				// Set Compare B value
	EPwm1Regs.AQCSFRC.bit.CSFA = 0;	//Software Force Disabled!
    EPwm1Regs.AQCSFRC.bit.CSFB = 0;

	EDIS;

}

void SM2PWM_Out(Uint16 CMP_Value)
{
	EALLOW;

//	EPwm2Regs.TBCTR = 0x0000;					   // Clear counter
	
	// Set Compare values
	EPwm2Regs.CMPA.half.CMPA = CMP_Value; 	// Set compare A value
	EPwm2Regs.CMPB = CMP_Value;				// Set Compare B value
	EPwm2Regs.AQCSFRC.bit.CSFA = 0;
    EPwm2Regs.AQCSFRC.bit.CSFB = 0;

	EDIS;

}

void SM3PWM_Out(Uint16 CMP_Value)
{
	EALLOW;

//	EPwm2Regs.TBCTR = 0x0000;					   // Clear counter
	
	// Set Compare values
	EPwm3Regs.CMPA.half.CMPA = CMP_Value; 	// Set compare A value
	EPwm3Regs.CMPB = CMP_Value;				// Set Compare B value
	EPwm3Regs.AQCSFRC.bit.CSFA = 0;
    EPwm3Regs.AQCSFRC.bit.CSFB = 0;

	EDIS;

}


void SM4PWM_Out(Uint16 CMP_Value)
{
	EALLOW;

//	EPwm4Regs.TBCTR = 0x0000;					   // Clear counter
	
	// Set Compare values
	EPwm4Regs.CMPA.half.CMPA = CMP_Value; 	// Set compare A value
	EPwm4Regs.CMPB = CMP_Value;				// Set Compare B value
	EPwm4Regs.AQCSFRC.bit.CSFA = 0;
    EPwm4Regs.AQCSFRC.bit.CSFB = 0;

	EDIS;

}


