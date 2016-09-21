/*
 * SYM_Control.c
 *
 *  Created on: 2016Äê9ÔÂ21ÈÕ
 *      Author: voidman
 */

#include "DSP2834x_Device.h"
#include "ZXY_app.h"
#include "SYM_app.h"

void sym_offline_codec_decode_test()
{
	sym_Ecat_DATA_Codec(); // Codec the COM info from specific source to go_SYM_ECAT_DOWN_DATA
//	sym_ECat_DATA_Write(); // Write suppose to be here, but commented for offline test
	sym_Ecat_DATA_Decode(); // Decode info from go_SYM_ECAT_DOWN_DATA to go_SYM_ECAT_DOWN_DATA_DECODED
}

void sym_online_codec_decode_test()
{
		Ecat_DATA_Get();
		ADRead(AD_Download_Buf);
		ADWrite();
		while(DmaRegs.CH1.CONTROL.bit.TRANSFERSTS);
		sym_Ecat_DATA_Codec();
		SMGpioDataSet(4,GPIO_OUT_DOWN);
//		ECatWrite();//

		sym_ECat_DATA_Write();
		SMGpioDataSet(4,GPIO_OUT_UP);
}
