#include "DSP2834x_Device.h"
#include "ZXY_app.h"


void SM_Timer_Init(void)
{
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.TINT0 =  &cpu_timer0_isr;
   PieVectTable.TINT2 =  &cpu_timer2_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

   InitCpuTimers();   // For this example, only initialize the Cpu Timers


/*For Backup*///   ConfigCpuTimer(&CpuTimer0, 150, 156*5);//5�����ж�һ��
   ConfigCpuTimer(&CpuTimer0, 150, 156*5);//5�����ж�һ��
//sym: Used for generate Sin(x)
//   ConfigCpuTimer(&CpuTimer1, 150, 2);	// 2us interrupt
//   ConfigCpuTimer(&CpuTimer2,150,1000);///1ms�ж�һ��

// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
// below settings must also be updated.

   CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
//sym: Used for generate Sin(x)
//   CpuTimer1Regs.TCR.all = 0x4001;
   CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0	
// Enable CPU int1 which is connected to CPU-Timer 0
   IER |= M_INT1;
//   IER |= M_INT13;
   IER |= M_INT14;
// Enable TINT0 in the PIE: Group 1 interrupt 7
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

}


interrupt void cpu_timer0_isr(void)  //////5���ⲿ�жϵ�ʱ�����һ�ζ�ʱ��0�ж�//////////////
{  
	ServiceDog();
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer2_isr(void)  ///////1ms����һ�ζ�ʱ���ж�//////////////////////
{  
	/*EcatSatusRead();
	Ecat_DATA_Get();
    Ecat_CMD_Re();

    ECatWrite();*/
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = 0x1000;
}




