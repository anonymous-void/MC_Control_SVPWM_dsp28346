/*
 * SYM_Ecat.c
 *
 *  Created on: 2016Äê9ÔÂ20ÈÕ
 *      Author: voidman
 */

#include "DSP2834x_Device.h"
#include "ZXY_app.h"
#include "SYM_app.h"
#include "math.h"

//void sym_offline_codec_decode_test()
//{
//	sym_Ecat_DATA_Codec(); // Codec the COM info from specific source to go_SYM_ECAT_DOWN_DATA
////	sym_ECat_DATA_Write(); // Write suppose to be here, but commented for offline test
//	sym_Ecat_DATA_Decode(); // Decode info from go_SYM_ECAT_DOWN_DATA to go_SYM_ECAT_DOWN_DATA_DECODED
//}

void sym_COM_test_data_init(gcl_vector_seq * ob_FAKE_bothSide_vector_sequence)
{
/* Description: Used for test COM system
 * Vector Type: 1~3, represent I, II, II, 0 is rsvd
 * Vector Num : 0~6
 * Duty: 0.0 ~ 1.0
*/
	ob_FAKE_bothSide_vector_sequence[0].vector_type_in =  1;
	ob_FAKE_bothSide_vector_sequence[0].vector_type_out = 1;
	ob_FAKE_bothSide_vector_sequence[0].vector_num_in =   2;
	ob_FAKE_bothSide_vector_sequence[0].vector_num_out =  2;
	ob_FAKE_bothSide_vector_sequence[0].duty =            0.1;

	ob_FAKE_bothSide_vector_sequence[1].vector_type_in =  3;
	ob_FAKE_bothSide_vector_sequence[1].vector_type_out = 3;
	ob_FAKE_bothSide_vector_sequence[1].vector_num_in =   1;
	ob_FAKE_bothSide_vector_sequence[1].vector_num_out =  1;
	ob_FAKE_bothSide_vector_sequence[1].duty =            0.2;

	ob_FAKE_bothSide_vector_sequence[2].vector_type_in =  2;
	ob_FAKE_bothSide_vector_sequence[2].vector_type_out = 2;
	ob_FAKE_bothSide_vector_sequence[2].vector_num_in =   3;
	ob_FAKE_bothSide_vector_sequence[2].vector_num_out =  3;
	ob_FAKE_bothSide_vector_sequence[2].duty =            0.3;

	ob_FAKE_bothSide_vector_sequence[3].vector_type_in =  1;
	ob_FAKE_bothSide_vector_sequence[3].vector_type_out = 1;
	ob_FAKE_bothSide_vector_sequence[3].vector_num_in =   2;
	ob_FAKE_bothSide_vector_sequence[3].vector_num_out =  2;
	ob_FAKE_bothSide_vector_sequence[3].duty =            0.4;

	ob_FAKE_bothSide_vector_sequence[4].vector_type_in =  3;
	ob_FAKE_bothSide_vector_sequence[4].vector_type_out = 3;
	ob_FAKE_bothSide_vector_sequence[4].vector_num_in =   1;
	ob_FAKE_bothSide_vector_sequence[4].vector_num_out =  1;
	ob_FAKE_bothSide_vector_sequence[4].duty =            0.5;
}

void sym_online_codec_decode_test()
{
	// Step 1: Calc 3-phase value
	const float32 f_in = 15, f_out = 50;
	const float32 A = 8.0/7.0;
	const float32 step = 0.0001;

	go_SYM_3p_in_vol.a_val = (float32)(A * sin(2 * PI * f_in * t));
	go_SYM_3p_in_vol.b_val = (float32)(A * sin(2 * PI * f_in * t - 2*PI/3.0));
	go_SYM_3p_in_vol.c_val = (float32)(A * sin(2 * PI * f_in * t + 2*PI/3.0));

	go_SYM_3p_out_vol.a_val = (float32)(A*sin(2 * PI * f_out * t));
	go_SYM_3p_out_vol.b_val = (float32)(A*sin(2 * PI * f_out * t - 2*PI/3.0));
	go_SYM_3p_out_vol.c_val = (float32)(A*sin(2 * PI * f_out * t + 2*PI/3.0));

	// Step 2: Calc the duty, vector type, vector num, etc for each side
	gf_SYM_get_vecotor_location_in_ab_prime(&go_SYM_3p_in_vol, &go_inSide_ref_vector_ab_prime_loc);
	gf_SYM_get_vector_sequence(&go_inSide_ref_vector_ab_prime_loc, go_vectorTab_ab_prime, go_inSide_vector_sequence);

	gf_SYM_get_vecotor_location_in_ab_prime(&go_SYM_3p_out_vol, &go_outSide_ref_vector_ab_prime_loc);
	gf_SYM_get_vector_sequence(&go_outSide_ref_vector_ab_prime_loc, go_vectorTab_ab_prime, go_outSide_vector_sequence);

	// Step 3: Combine the two sides sequences into 1 sequence according to their duty cycles
	gf_SYM_sequence_combine(go_inSide_vector_sequence, go_outSide_vector_sequence, go_bothSide_vector_sequence);

	// Step 4: Copy the sequence data from __get_vector__ format into __ECAT_DOWNDATA_DECODED__ format
	gf_SYM_data_bridge(go_bothSide_vector_sequence, &go_SYM_ECAT_DOWN_DATA_NEED_CODEC);
	// SYM: For COM test.
//	gf_SYM_data_bridge(go_FAKE_bothSide_vector_sequence, &go_SYM_ECAT_DOWN_DATA_NEED_CODEC);

	// Step 5: Transform all info from float format into Uint16 format for transmission.
	sym_Ecat_DATA_Codec(&go_SYM_ECAT_DOWN_DATA_NEED_CODEC, &go_SYM_ECAT_DOWN_DATA, 1000, 1000);


	Ecat_DATA_Get();
	ADRead(AD_Download_Buf);
//	ADWrite((Uint16)(go_outSide_vector_sequence[2].duty * 4095));
	ADWrite(ad_test);
	if (ad_test == 0)
		ad_test = 4095;
	else
		ad_test = 0;

	while(DmaRegs.CH1.CONTROL.bit.TRANSFERSTS);

	// Step 6: Start transmission.
	SMGpioDataSet(4,GPIO_OUT_DOWN);
//		ECatWrite();//
	sym_ECat_DATA_Write(&go_SYM_ECAT_DOWN_DATA);
	SMGpioDataSet(4,GPIO_OUT_UP);

//	if (go_bothSide_vector_sequence[0].duty < 0 || go_bothSide_vector_sequence[1].duty < 0||\
//			go_bothSide_vector_sequence[2].duty < 0 || go_bothSide_vector_sequence[3].duty < 0 ||\
//			go_bothSide_vector_sequence[4].duty < 0)
//		flag = 1;

	// Step 7: Simulation time update
{
	t = t + step;
	if (t >= 0.2)
		t = 0;
}

}

void sym_ECat_DATA_Write(gc_SYM_ECAT_DOWN_DATA * ob_ECAT_DOWN_DATA)
{/* Description: this func write the command data to a specific address for transmission.
 *  Input args: ob_ECAT_DOWN_DATA, all members are Uint16
 * */
    ECat_UP_Buf[0] =  ob_ECAT_DOWN_DATA->SM_CMD.all;
    ECat_UP_Buf[1] =  ob_ECAT_DOWN_DATA->DUTY1;
    ECat_UP_Buf[2] =  ob_ECAT_DOWN_DATA->DUTY2;
    ECat_UP_Buf[3] =  ob_ECAT_DOWN_DATA->DUTY3;
    ECat_UP_Buf[4] =  ob_ECAT_DOWN_DATA->DUTY4;
    ECat_UP_Buf[5] =  ob_ECAT_DOWN_DATA->TREE_SEC_1.all;
    ECat_UP_Buf[6] =  ob_ECAT_DOWN_DATA->TREE_SEC_2.all;
    ECat_UP_Buf[7] =  ob_ECAT_DOWN_DATA->TREE_SEC_3.all;
    ECat_UP_Buf[8] =  ob_ECAT_DOWN_DATA->VECTOR_SEC_1.all;
    ECat_UP_Buf[9] =  ob_ECAT_DOWN_DATA->VECTOR_SEC_2.all;
    ECat_UP_Buf[10] = ob_ECAT_DOWN_DATA->VECTOR_SEC_3.all;
    ECat_UP_Buf[11] = ob_ECAT_DOWN_DATA->VECTOR_SEC_4.all;
    ECat_UP_Buf[12] = ob_ECAT_DOWN_DATA->UdcOverVol;
    ECat_UP_Buf[13] = ob_ECAT_DOWN_DATA->OverCur;
}


void sym_Ecat_DATA_Codec(gc_SYM_ECAT_DOWN_DATA_DECODED * ob_ECAT_DOWN_DATA_NEED_CODEC,\
                        gc_SYM_ECAT_DOWN_DATA *ob_ECAT_DOWN_DATA, Uint16 UdcOverVol, Uint16 OverCur)
{/* Description: Codec the specific vals to go_SYM_ECAT_DOWN_DATA
*/
	ob_ECAT_DOWN_DATA->UdcOverVol = 1000;
	ob_ECAT_DOWN_DATA->OverCur = 1000;

    singles2halfp(&ob_ECAT_DOWN_DATA->DUTY1, &ob_ECAT_DOWN_DATA_NEED_CODEC->duty[0], 1);
    singles2halfp(&ob_ECAT_DOWN_DATA->DUTY2, &ob_ECAT_DOWN_DATA_NEED_CODEC->duty[1], 1);
    singles2halfp(&ob_ECAT_DOWN_DATA->DUTY3, &ob_ECAT_DOWN_DATA_NEED_CODEC->duty[2], 1);
    singles2halfp(&ob_ECAT_DOWN_DATA->DUTY4, &ob_ECAT_DOWN_DATA_NEED_CODEC->duty[3], 1);

    ob_ECAT_DOWN_DATA->TREE_SEC_1.bit.tree_l = ob_ECAT_DOWN_DATA_NEED_CODEC->tree[0];
    ob_ECAT_DOWN_DATA->TREE_SEC_1.bit.tree_h = ob_ECAT_DOWN_DATA_NEED_CODEC->tree[1];
    ob_ECAT_DOWN_DATA->TREE_SEC_2.bit.tree_l = ob_ECAT_DOWN_DATA_NEED_CODEC->tree[2];
    ob_ECAT_DOWN_DATA->TREE_SEC_2.bit.tree_h = ob_ECAT_DOWN_DATA_NEED_CODEC->tree[3];
    ob_ECAT_DOWN_DATA->TREE_SEC_3.bit.tree_l = ob_ECAT_DOWN_DATA_NEED_CODEC->tree[4];

    ob_ECAT_DOWN_DATA->VECTOR_SEC_1.bit.vector_l = \
			sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[0].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[0].vnum);
    ob_ECAT_DOWN_DATA->VECTOR_SEC_1.bit.vector_m = \
			sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[1].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[1].vnum);
    ob_ECAT_DOWN_DATA->VECTOR_SEC_1.bit.vector_h = \
			sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[2].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[2].vnum);

    ob_ECAT_DOWN_DATA->VECTOR_SEC_2.bit.vector_l = \
				sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[3].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[3].vnum);
    ob_ECAT_DOWN_DATA->VECTOR_SEC_2.bit.vector_m = \
				sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[4].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_input[4].vnum);


    ob_ECAT_DOWN_DATA->VECTOR_SEC_3.bit.vector_l = \
				sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[0].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[0].vnum);
    ob_ECAT_DOWN_DATA->VECTOR_SEC_3.bit.vector_m = \
					sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[1].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[1].vnum);
    ob_ECAT_DOWN_DATA->VECTOR_SEC_3.bit.vector_h = \
					sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[2].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[2].vnum);

    ob_ECAT_DOWN_DATA->VECTOR_SEC_4.bit.vector_l = \
					sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[3].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[3].vnum);
    ob_ECAT_DOWN_DATA->VECTOR_SEC_4.bit.vector_m = \
					sym_Ecat_DATA_Codec_VectCodec(ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[4].vtype, ob_ECAT_DOWN_DATA_NEED_CODEC->vector_output[4].vnum);


//    ob_ECAT_DOWN_DATA->SM_CMD.bit.Reset = 0;
//    ob_ECAT_DOWN_DATA->SM_CMD.bit.deblock = 1;

}


uint16_t sym_Ecat_DATA_Codec_VectCodec(int vtype, int vnum)
{/* Description: Given a specific vector's type and num,
				this func return vector code, whose low 5 bits take effect
*/
	uint16_t ret = 0;
	ret = ( ( (uint16_t)0x03 & (uint16_t)vtype )<<3 ) + ( (uint16_t)0x07 & (uint16_t)vnum); // 2bit of vtype, 3bit of vnum
	return ret;
}

int singles2halfp(void *target, void *source, int numel)
{
    UINT16_TYPE *hp = (UINT16_TYPE *) target; // Type pun output as an unsigned 16-bit int
    UINT32_TYPE *xp = (UINT32_TYPE *) source; // Type pun input as an unsigned 32-bit int
    UINT16_TYPE    hs, he, hm;
    UINT32_TYPE x, xs, xe, xm;
    int hes;
/*
 * SYM: 2016-09-20
 *     Comment for forcing ignore the IEEE-754 checking, cause DSP is not intrinsic 32bit, it use 2 16bits emulation.
 * */
//    static int next;  // Little Endian adjustment
//    static int checkieee = 1;  // Flag to check for IEEE754, Endian, and word size
//    double one = 1.0; // Used for checking IEEE754 floating point format
//    UINT32_TYPE *ip; // Used for checking IEEE754 floating point format

//    if( checkieee ) { // 1st call, so check for IEEE754, Endian, and word size
//        ip = (UINT32_TYPE *) &one;
//        if( *ip ) { // If Big Endian, then no adjustment
//            next = 0;
//        } else { // If Little Endian, then adjustment will be necessary
//            next = 1;
//            ip++;
//        }
//        if( *ip != 0x3FF00000u ) { // Check for exact IEEE 754 bit pattern of 1.0
//            return 1;  // Floating point bit pattern is not IEEE 754
//        }
//        if( sizeof(INT16_TYPE) != 2 || sizeof(INT32_TYPE) != 4 ) {
//            return 1;  // short is not 16-bits, or long is not 32-bits.
//        }
//        checkieee = 0; // Everything checks out OK
//    }
/*
 * SYM: 2016-09-20
 *     Comment for forcing ignore the IEEE-754 checking, cause DSP is not intrinsic 32bit, it use 2 16bits emulation.
 * */

    if( source == NULL || target == NULL ) { // Nothing to convert (e.g., imag part of pure real)
        return 0;
    }

    while( numel-- ) {
        x = *xp++;
        if( (x & 0x7FFFFFFFu) == 0 ) {  // Signed zero
            *hp++ = (UINT16_TYPE) (x >> 16);  // Return the signed zero
        } else { // Not zero
            xs = x & 0x80000000u;  // Pick off sign bit
            xe = x & 0x7F800000u;  // Pick off exponent bits
            xm = x & 0x007FFFFFu;  // Pick off mantissa bits
            if( xe == 0 ) {  // Denormal will underflow, return a signed zero
                *hp++ = (UINT16_TYPE) (xs >> 16);
            } else if( xe == 0x7F800000u ) {  // Inf or NaN (all the exponent bits are set)
                if( xm == 0 ) { // If mantissa is zero ...
                    *hp++ = (UINT16_TYPE) ((xs >> 16) | 0x7C00u); // Signed Inf
                } else {
                    *hp++ = (UINT16_TYPE) 0xFE00u; // NaN, only 1st mantissa bit set
                }
            } else { // Normalized number
                hs = (UINT16_TYPE) (xs >> 16); // Sign bit
                hes = ((int)(xe >> 23)) - 127 + 15; // Exponent unbias the single, then bias the halfp
                if( hes >= 0x1F ) {  // Overflow
                    *hp++ = (UINT16_TYPE) ((xs >> 16) | 0x7C00u); // Signed Inf
                } else if( hes <= 0 ) {  // Underflow
                    if( (14 - hes) > 24 ) {  // Mantissa shifted all the way off & no rounding possibility
                        hm = (UINT16_TYPE) 0u;  // Set mantissa to zero
                    } else {
                        xm |= 0x00800000u;  // Add the hidden leading bit
                        hm = (UINT16_TYPE) (xm >> (14 - hes)); // Mantissa
                        if( (xm >> (13 - hes)) & 0x00000001u ) // Check for rounding
                            hm += (UINT16_TYPE) 1u; // Round, might overflow into exp bit, but this is OK
                    }
                    *hp++ = (hs | hm); // Combine sign bit and mantissa bits, biased exponent is zero
                } else {
                    he = (UINT16_TYPE) (hes << 10); // Exponent
                    hm = (UINT16_TYPE) (xm >> 13); // Mantissa
                    if( xm & 0x00001000u ) // Check for rounding
                        *hp++ = (hs | he | hm) + (UINT16_TYPE) 1u; // Round, might overflow to inf, this is OK
                    else
                        *hp++ = (hs | he | hm);  // No rounding
                }
            }
        }
    }
    return 0;
}

int halfp2singles(void *target, void *source, int numel)
{
    UINT16_TYPE *hp = (UINT16_TYPE *) source; // Type pun input as an unsigned 16-bit int
    UINT32_TYPE *xp = (UINT32_TYPE *) target; // Type pun output as an unsigned 32-bit int
    UINT16_TYPE h, hs, he, hm;
    UINT32_TYPE xs, xe, xm;
    INT32_TYPE xes;
    int e;

/*
 * SYM: 2016-09-20
 *     Comment for forcing ignore the IEEE-754 checking, cause DSP is not intrinsic 32bit, it use 2 16bits emulation.
 * */
//    static int next;  // Little Endian adjustment
//    static int checkieee = 1;  // Flag to check for IEEE754, Endian, and word size
//    double one = 1.0; // Used for checking IEEE754 floating point format
//    UINT32_TYPE *ip; // Used for checking IEEE754 floating point format

//    if( checkieee ) { // 1st call, so check for IEEE754, Endian, and word size
//        ip = (UINT32_TYPE *) &one;
//        if( *ip ) { // If Big Endian, then no adjustment
//            next = 0;
//        } else { // If Little Endian, then adjustment will be necessary
//            next = 1;
//            ip++;
//        }
//        if( *ip != 0x3FF00000u ) { // Check for exact IEEE 754 bit pattern of 1.0
//            return 1;  // Floating point bit pattern is not IEEE 754
//        }
//        if( sizeof(INT16_TYPE) != 2 || sizeof(INT32_TYPE) != 4 ) {
//            return 1;  // short is not 16-bits, or long is not 32-bits.
//        }
//        checkieee = 0; // Everything checks out OK
//    }
/*
 * SYM: 2016-09-20
 *     Comment for forcing ignore the IEEE-754 checking, cause DSP is not intrinsic 32bit, it use 2 16bits emulation.
 * */

    if( source == NULL || target == NULL ) // Nothing to convert (e.g., imag part of pure real)
        return 0;

    while( numel-- ) {
        h = *hp++;
        if( (h & 0x7FFFu) == 0 ) {  // Signed zero
            *xp++ = ((UINT32_TYPE) h) << 16;  // Return the signed zero
        } else { // Not zero
            hs = h & 0x8000u;  // Pick off sign bit
            he = h & 0x7C00u;  // Pick off exponent bits
            hm = h & 0x03FFu;  // Pick off mantissa bits
            if( he == 0 ) {  // Denormal will convert to normalized
                e = -1; // The following loop figures out how much extra to adjust the exponent
                do {
                    e++;
                    hm <<= 1;
                } while( (hm & 0x0400u) == 0 ); // Shift until leading bit overflows into exponent bit
                xs = ((UINT32_TYPE) hs) << 16; // Sign bit
                xes = ((INT32_TYPE) (he >> 10)) - 15 + 127 - e; // Exponent unbias the halfp, then bias the single
                xe = (UINT32_TYPE) (xes << 23); // Exponent
                xm = ((UINT32_TYPE) (hm & 0x03FFu)) << 13; // Mantissa
                *xp++ = (xs | xe | xm); // Combine sign bit, exponent bits, and mantissa bits
            } else if( he == 0x7C00u ) {  // Inf or NaN (all the exponent bits are set)
                if( hm == 0 ) { // If mantissa is zero ...
                    *xp++ = (((UINT32_TYPE) hs) << 16) | ((UINT32_TYPE) 0x7F800000u); // Signed Inf
                } else {
                    *xp++ = (UINT32_TYPE) 0xFFC00000u; // NaN, only 1st mantissa bit set
                }
            } else { // Normalized number
                xs = ((UINT32_TYPE) hs) << 16; // Sign bit
                xes = ((INT32_TYPE) (he >> 10)) - 15 + 127; // Exponent unbias the halfp, then bias the single
                xe = (UINT32_TYPE) (xes << 23); // Exponent
                xm = ((UINT32_TYPE) hm) << 13; // Mantissa
                *xp++ = (xs | xe | xm); // Combine sign bit, exponent bits, and mantissa bits
            }
        }
    }
    return 0;
}

// SYM: Decode function series are just for offline test.
//void sym_Ecat_DATA_Decode_vector(void)
//{
//    int i = 0;
//    uint16_t mask_type = 0x0018, mask_num = 0x0007;
//    for (i = 0; i < 5; ++i) {
//        go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[i].vtype = (mask_type & go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[i].vraw)>>3;
//        go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[i].vnum = mask_num & go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[i].vraw;
//
//        go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[i].vtype = (mask_type & go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[i].vraw)>>3;
//        go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[i].vnum = mask_num & go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[i].vraw;
//    }
//}
//
//void sym_Ecat_DATA_Decode(void)
//{/* Description: Decode command, tree, vector, duty and so on from go_SYM_ECAT_DOWN_DATA
// * */
//	go_SYM_ECAT_CMD.bit.Reset = go_SYM_ECAT_DOWN_DATA.SM_CMD.bit.Reset; // Original Command code copyed.
//	go_SYM_ECAT_CMD.bit.deblock = go_SYM_ECAT_DOWN_DATA.SM_CMD.bit.deblock;
//
//    halfp2singles(&go_SYM_ECAT_DOWN_DATA_DECODED.duty[0], &go_SYM_ECAT_DOWN_DATA.DUTY1, 1);
//    halfp2singles(&go_SYM_ECAT_DOWN_DATA_DECODED.duty[1], &go_SYM_ECAT_DOWN_DATA.DUTY2, 1);
//    halfp2singles(&go_SYM_ECAT_DOWN_DATA_DECODED.duty[2], &go_SYM_ECAT_DOWN_DATA.DUTY3, 1);
//    halfp2singles(&go_SYM_ECAT_DOWN_DATA_DECODED.duty[3], &go_SYM_ECAT_DOWN_DATA.DUTY4, 1);
//
//    // Input side vector copy;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[0].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_1.bit.vector_l;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[1].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_1.bit.vector_m;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[2].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_1.bit.vector_h;
//
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[3].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_2.bit.vector_l;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_input[4].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_2.bit.vector_m;
//
//    // Output side vector copy
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[0].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_3.bit.vector_l;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[1].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_3.bit.vector_m;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[2].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_3.bit.vector_h;
//
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[3].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_4.bit.vector_l;
//    go_SYM_ECAT_DOWN_DATA_DECODED.vector_output[4].vraw = go_SYM_ECAT_DOWN_DATA.VECTOR_SEC_4.bit.vector_m;
//
//    sym_Ecat_DATA_Decode_vector(); // Both side vector decode
//
//    // Tree decode
//    go_SYM_ECAT_DOWN_DATA_DECODED.tree[0] = go_SYM_ECAT_DOWN_DATA.TREE_SEC_1.bit.tree_l;
//    go_SYM_ECAT_DOWN_DATA_DECODED.tree[1] = go_SYM_ECAT_DOWN_DATA.TREE_SEC_1.bit.tree_h;
//
//    go_SYM_ECAT_DOWN_DATA_DECODED.tree[2] = go_SYM_ECAT_DOWN_DATA.TREE_SEC_2.bit.tree_l;
//    go_SYM_ECAT_DOWN_DATA_DECODED.tree[3] = go_SYM_ECAT_DOWN_DATA.TREE_SEC_2.bit.tree_h;
//
//    go_SYM_ECAT_DOWN_DATA_DECODED.tree[4] = go_SYM_ECAT_DOWN_DATA.TREE_SEC_3.bit.tree_l;
//}
