/*
 * SYM_defines.c
 *
 *  Created on: 2016Äê9ÔÂ20ÈÕ
 *      Author: voidman
 */

#include "SYM_app.h"

// Global variables
gc_SYM_ECAT_CMD_ALL				go_SYM_ECAT_CMD;
gc_SYM_ECAT_DOWN_DATA			go_SYM_ECAT_DOWN_DATA;
gc_SYM_ECAT_DOWN_DATA_DECODED	go_SYM_ECAT_DOWN_DATA_NEED_CODEC;
gc_SYM_ECAT_DOWN_DATA_DECODED   go_SYM_ECAT_DOWN_DATA_DECODED;


// Vector's section lookup table
gcl_vector_ab_prime go_vectorTab_ab_prime[3][7];
// Reference vector object, use for storing incomming reference vector's coordinate(double type)
gcl_vector_ab_prime_loc go_inSide_ref_vector_ab_prime_loc, go_outSide_ref_vector_ab_prime_loc;
// Array of vector objects. Used for storing 3 vector objects of modulation sequence.
gcl_vector_ab_prime go_inSide_vector_sequence[3], go_outSide_vector_sequence[3];
// Switching sequence
gcl_vector_seq go_bothSide_vector_sequence[5]; //Storing a whole sequence of a duty cycle

gc_SYM_THREE_PHASE_VAL go_SYM_3p_in_vol;
gc_SYM_THREE_PHASE_VAL go_SYM_3p_in_cur;

gc_SYM_THREE_PHASE_VAL go_SYM_3p_out_vol;
gc_SYM_THREE_PHASE_VAL go_SYM_3p_out_cur;

//gc_SYM_CLARKE_VAL go_SYM_2p_in_vol;
//gc_SYM_CLARKE_VAL go_SYM_2p_in_cur;
//
//gc_SYM_CLARKE_VAL go_SYM_2p_out_vol;
//gc_SYM_CLARKE_VAL go_SYM_2p_out_cur;
