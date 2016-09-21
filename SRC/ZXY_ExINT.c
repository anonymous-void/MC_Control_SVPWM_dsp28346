#include "DSP2834x_Device.h"
#include "ZXY_app.h"
//This interrupt is the KEY of the whole program!!!!!!!!!!
interrupt void Xint3_isr(void)	//外部中断3，对应GPIO36，Pin145
{	
//	offline_codec_decode_test();
	sym_online_codec_decode_test();
// SYM: Comment for offline test
//	Ecat_DATA_Get();
//	ADRead(AD_Download_Buf);
//	ADWrite();
//	while(DmaRegs.CH1.CONTROL.bit.TRANSFERSTS);
//	SMGpioDataSet(4,GPIO_OUT_DOWN);
//	ECatWrite();
//	SMGpioDataSet(4,GPIO_OUT_UP);

	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

void SM_ExInt3_Init(void)
{
	EALLOW; 
	PieVectTable.XINT3 = &Xint3_isr;
	PieCtrlRegs.PIEIER12.bit.INTx1 = 1;
	IER |= M_INT12;
	EDIS;
	*(Uint16 *)0x100102 = 0xFF00;//0x204-0x205 AL IRQ Event Mask 
	*(Uint16 *)0x100103 = 0x00FF;//0x206-0x207 AL IRQ Event Mask
	// GPIO0 and GPIO1 are inputs         
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO36= 0;         // GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO36 = 0;          // input
	GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3;  //LYB 2013.06.24  值由原来的3改为1
	GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL =  36;
	EDIS;
    // Configure XINT1
    // 0-Falling  1-Rising edge
    // Falling edge interrupt 2-Falling edge 3-Falling or Rising edge
    // Rising edge interrupt
	XIntruptRegs.XINT3CR.bit.POLARITY = 1;      // Rising edge interrupt
	// Enable XINT1 and XINT2   
	XIntruptRegs.XINT3CR.bit.ENABLE = 1;
}
