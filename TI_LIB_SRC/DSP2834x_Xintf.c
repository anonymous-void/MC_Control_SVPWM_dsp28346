// TI File $Revision: /main/5 $
// Checkin $Date: August 28, 2008   16:54:21 $
//###########################################################################
//
// FILE:   DSP2834x_Xintf.c
//
// TITLE:   DSP2834x Device External Interface Init & Support Functions.
//
// DESCRIPTION:
//
//          Example initialization function for the external interface (XINTF).
//          This example configures the XINTF to its default state.  For an
//          example of how this function being used refer to the
//          examples/run_from_xintf project.
//
//###########################################################################
// $TI Release: 2834x Header Files V1.12 $
// $Release Date: March 2011 $
//###########################################################################

#include "DSP2834x_Device.h"     // DSP2834x Headerfile Include File
#include "DSP2834x_Examples.h"   // DSP2834x Examples Include File

//---------------------------------------------------------------------------
// InitXINTF:
//---------------------------------------------------------------------------
// This function initializes the External Interface the default reset state.
//
// Do not modify the timings of the XINTF while running from the XINTF.  Doing
// so can yield unpredictable results


void InitXintf(void)
{
    // This shows how to write to the XINTF registers.  The
    // values used here are the default state after reset.
    // Different hardware will require a different configuration.

    // For an example of an XINTF configuration used with the
    // F28345 eZdsp, refer to the examples/run_from_xintf project.

    // Any changes to XINTF timing should only be made by code
    // running outside of the XINTF.

    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = SYSCLKOUT/4
    EALLOW;
	// XTIMCLK = SYSCLKOUT/2
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;//1
    // No write buffering
    XintfRegs.XINTCNF2.bit.WRBUFF = 0;
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 1;
    // XCLKOUT = XTIMCLK/2
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;
    // XCLKOUT = XTIMCLK/4 (SYSCLKOUT/8)
    XintfRegs.XINTCNF2.bit.BY4CLKMODE = 0;


	//Setup Zine Timing
	//Note: These timing are assuming SYSCLKOUT is 300Mhz

    // Zone 0------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead (Read) must always be 1 or greater
    // Lead and Trail (Write) must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING0.bit.XWRLEAD = 3;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING0.bit.XRDLEAD = 3;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING0.bit.X2TIMING = 0;//1

    // Zone will sample XREADY signal
    XintfRegs.XTIMING0.bit.USEREADY = 0;//1
    XintfRegs.XTIMING0.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING0.bit.XSIZE = 3;

    // Zone 6------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead (Read) must always be 1 or greater
    // Lead and Trail (Write) must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING6.bit.XWRLEAD = 3;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING6.bit.XRDLEAD =  3;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING6.bit.X2TIMING = 0;//1

    // Zone will sample XREADY signal
    XintfRegs.XTIMING6.bit.USEREADY = 0;//1
    XintfRegs.XTIMING6.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;


    // Zone 7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead (Read) must always be 1 or greater
    // Lead and Trail (Write) must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING7.bit.XWRLEAD = 3;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING7.bit.XRDLEAD = 3;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING7.bit.X2TIMING = 0;//1

    // Zone will sample XREADY signal
    XintfRegs.XTIMING7.bit.USEREADY = 0;//1
    XintfRegs.XTIMING7.bit.READYMODE = 1;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING7.bit.XSIZE = 3;

    // Bank switching
    // Assume Zone 7 is slow, so add additional BCYC cycles
    // when ever switching from Zone 7 to another Zone.
    // This will help avoid bus contention.
    XintfRegs.XBANK.bit.BANK = 7;
    XintfRegs.XBANK.bit.BCYC = 7;
    EDIS;
   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.

   InitXintf16Gpio();
// InitXintf32Gpio();

   asm(" RPT #7 || NOP");

}

void InitXintf32Gpio()
{
	 // To configure the GPIOs for XINTF, set the GPIO MUX setting equal to:
	 //	- 2 for C2834x/C2824x devices
	 // Always refer to the device data manual and XINTF User's Guide for
	 // correct GPIO MUX settings

     EALLOW;
     GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 2;  // XD31
     GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 2;  // XD30
     GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 2;  // XD29
     GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 2;  // XD28
     GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 2;  // XD27
     GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 2;  // XD26
     GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 2;  // XD25
     GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 2;  // XD24
     GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 2;  // XD23
     GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 2;  // XD22
     GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 2;  // XD21
     GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 2;  // XD20
     GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 2;  // XD19
     GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 2;  // XD18
     GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 2;  // XD17
     GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 2;  // XD16

     GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 2;  // XD31 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO49 = 2;  // XD30 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 2;  // XD29 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 2;  // XD28 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO52 = 2;  // XD27 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 2;  // XD26 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 2;  // XD25 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 2;  // XD24 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 2;  // XD23 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 2;  // XD22 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 2;  // XD21 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 2;  // XD20 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 2;  // XD19 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 2;  // XD18 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 2;  // XD17 asynchronous input
     GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 2;  // XD16 asynchronous input


     InitXintf16Gpio();
}

void InitXintf16Gpio()
{
     EALLOW;
     GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 2;  // XD15
     GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 2;  // XD14
     GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 2;  // XD13
     GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 2;  // XD12
     GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 2;  // XD11
     GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 2;  // XD10
     GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 2;  // XD19
     GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 2;  // XD8
     GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 2;  // XD7
     GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 2;  // XD6
     GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 2;  // XD5
     GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 2;  // XD4
     GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 2;  // XD3
     GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 2;  // XD2
     GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 2;  // XD1
     GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 2;  // XD0

     GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 2;  // XA0 (Note: XWE1n is a seperate pin and not muxed to XA0 in C2834x/C2824x)
     GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 2;  // XA1
     GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 2;  // XA2
     GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 2;  // XA3
     GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 2;  // XA4
     GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 2;  // XA5
     GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 2;  // XA6
     GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 2;  // XA7

     GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 2;  // XA8
     GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 2;  // XA9
     GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 2;  // XA10
     GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 2;  // XA11
     GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 2;  // XA12
     GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 2;  // XA13
     GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 2;  // XA14
     GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 2;  // XA15
     GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 2;  // XA16
     GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 2;  // XA17
     GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 2;  // XA18
     GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 2;  // XA19

     GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 2;  // XREADY
	 GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 2;  // XRNW
     GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 2;  // XWE0

     GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 2;  // XZCS0
     GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 2;  // XZCS7
     GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 2;  // XZCS6
     EDIS;
}

//===========================================================================
// No more.
//===========================================================================
