#include "DSP2834x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2834x_Examples.h"   // DSP2833x Examples Include File
#include "ZXY_app.h"


#pragma DATA_SECTION(ECat_Download_Buf,"ECATDOWNDATA");

volatile Uint16 ECat_Download_Buf[ECAT_DOWN_DATA_LENGTH];
volatile Uint16 *DMADest;
volatile Uint16 *DMASource;


void pqcDmaInit(void)
{
   volatile Uint16 temp=0;

// Initialize DMA
	DMAInitialize();


    DMADest = &MC_SM_DMA.AU.SM1Udc;
	DMASource = &ECat_Download_Buf[0];
   	DMACH1AddrConfig(DMADest, DMASource);
	DMACH1BurstConfig(0,1,1);
	DMACH1TransferConfig(ECAT_DOWN_DATA_LENGTH,1,1);
	DMACH1WrapConfig(0xFFFF,0,0xFFFF,0);
	DMACH1ModeConfig(DMA_SEQ1INT,PERINT_ENABLE,ONESHOT_ENABLE,CONT_ENABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_DISABLE);
	StartDMACH1();





}


