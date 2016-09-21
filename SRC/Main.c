#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "ZXY_app.h"

void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2834x_SysCtrl.c file.
   InitSysCtrl();
   //MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   InitXintf();
   pqcDmaInit();
// Step 2. Initalize GPIO:
// This example function is found in the DSP2834x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
   // InitGpio(); Skipped for this example
   //SMGpioSet(0,0,0,1,0);
   //SMGpioDataSet(0,1);
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2834x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2834x_DefaultIsr.c.
// This function is found in DSP2834x_PieVect.c.
   InitPieVectTable();
   SMGpioSet(4,GPIO_FUN_00,GPIO_R_UP,GPIO_DIR_OUT,3);
//   SM_Timer_Init();
   SM_ExInt3_Init();
   //DataInit();
   EcatDataInit();
   EcatInit();
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2834x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM
// Step 5. User specific code:
   while(1)
   {
	  ;
   }
}
