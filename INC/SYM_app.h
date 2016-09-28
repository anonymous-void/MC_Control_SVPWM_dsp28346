/*
 * SYM_app.h
 *
 *  Created on: 2016Äê9ÔÂ20ÈÕ
 *      Author: voidman
 */
#include "DSP2834x_Device.h"

#ifndef INC_SYM_APP_H_
#define INC_SYM_APP_H_

// Macros for single float and half float conversion
#define  INT16_TYPE          short
#define UINT16_TYPE unsigned short
#define  INT32_TYPE          long
#define UINT32_TYPE unsigned long

#define  INF  99
#define  gm_SWITHCING_FREQ  10000
#define  gm_SWITHCING_PRD	(1000000/gm_SWITHCING_FREQ)

#define PI			3.14159265358979323846
/* Start
 * SYM - 2016-09-13 My communication protocol for M3C SVPWM
*/
typedef struct gs_SYM_ECAT_CMD_BIT
{
    uint16_t Reset:1;
    uint16_t deblock:1;
    uint16_t rsvd:14;
} gc_SYM_ECAT_CMD_BIT;

typedef union gu_SYM_ECAT_CMD_ALL
{
    uint16_t all;
    struct gs_SYM_ECAT_CMD_BIT bit;
} gc_SYM_ECAT_CMD_ALL;

typedef struct gs_SYM_ECAT_TREE_BIT
{
    uint16_t tree_l:7;
    uint16_t tree_h:7;
    uint16_t rsvd:2;
} gc_SYM_ECAT_TREE_BIT;

typedef union gu_SYM_ECAT_TREE_ALL
{
    uint16_t all;
    gc_SYM_ECAT_TREE_BIT bit;
} gc_SYM_ECAT_TREE_ALL;

typedef struct gs_SYM_ECAT_VECTOR_BIT
{
    uint16_t vector_l:5;
    uint16_t vector_m:5;
    uint16_t vector_h:5;
    uint16_t rsvd:1;
} gc_SYM_ECAT_VECTOR_BIT;

typedef union gu_SYM_ECAT_VECTOR_ALL
{
    uint16_t all;
    gc_SYM_ECAT_VECTOR_BIT bit;
} gc_SYM_ECAT_VECTOR_ALL;

typedef struct gs_SYM_ECAT_DOWN_DATA
{
    union gu_SYM_ECAT_CMD_ALL SM_CMD;
    uint16_t DUTY1;
    uint16_t DUTY2;
    uint16_t DUTY3;
    uint16_t DUTY4;
    gc_SYM_ECAT_TREE_ALL TREE_SEC_1;
    gc_SYM_ECAT_TREE_ALL TREE_SEC_2;
    gc_SYM_ECAT_TREE_ALL TREE_SEC_3;
    gc_SYM_ECAT_VECTOR_ALL VECTOR_SEC_1;
    gc_SYM_ECAT_VECTOR_ALL VECTOR_SEC_2;
    gc_SYM_ECAT_VECTOR_ALL VECTOR_SEC_3;
    gc_SYM_ECAT_VECTOR_ALL VECTOR_SEC_4;
    uint16_t UdcOverVol;
    uint16_t OverCur;
} gc_SYM_ECAT_DOWN_DATA;

typedef struct gs_SYM_ECAT_DOWN_DATA_VECTOR
{
    uint16_t vtype; // Used for store vector type, vtype = 0~2, represents I, II, II
    uint16_t vnum; // Storing vector num, vnum = 0~6, represent V0 ~ V6
    uint16_t vraw; // Storing undecoded code.
} gc_SYM_ECAT_DOWN_DATA_VECTOR;

typedef struct gs_SYM_ECAT_DOWN_DATA_DECODED
{
    float duty[5]; // The fifth duty does not really matter, cuz it could be calced by former 4 duties.
    uint16_t  tree[5];
    gc_SYM_ECAT_DOWN_DATA_VECTOR	vector_input[5];
    gc_SYM_ECAT_DOWN_DATA_VECTOR	vector_output[5];
} gc_SYM_ECAT_DOWN_DATA_DECODED;

// These classes are for sequence finding
typedef struct gst_VECTOR_AB_PRIME
{
    int vector_type; // 1 for VI, 2 for VII, 3 for VIII
    int vector_num;  // 0:V0, 1:V1, ..., 6:V6
    int alpha;  // Location in ab' coordinate, alpha'
    int beta;   // .........................., beta'
    float32 duty; // duty cycle
} gcl_vector_ab_prime;

typedef struct gst_VECTOR_AB_PRIME_LOC
{
    float32 a_ref, b_ref;
    int a0_prime, a1_prime, a2_prime, a3_prime;
    int b0_prime, b1_prime, b2_prime, b3_prime;
    int flag_lower_or_upper;
} gcl_vector_ab_prime_loc;


typedef struct gst_VECTOR_SEQ
{
    int vector_type_in;
    int vector_type_out;
    int vector_num_in;
    int vector_num_out;
    float32 duty;
} gcl_vector_seq;

typedef struct gs_SYM_THREE_PHASE_VAL
{// Storing 3-phase value, could be voltage, current, etc.
    float32 a_val;
    float32 b_val;
    float32 c_val;
} gc_SYM_THREE_PHASE_VAL;

typedef struct gs_SYM_CLARKE_VAL
{
    float32 alpha_val;
    float32 beta_val;
} gc_SYM_CLARKE_VAL;


extern gc_SYM_ECAT_CMD_ALL				go_SYM_ECAT_CMD;
extern gc_SYM_ECAT_DOWN_DATA			go_SYM_ECAT_DOWN_DATA;
extern gc_SYM_ECAT_DOWN_DATA_DECODED	go_SYM_ECAT_DOWN_DATA_NEED_CODEC;
extern gc_SYM_ECAT_DOWN_DATA_DECODED    go_SYM_ECAT_DOWN_DATA_DECODED;

extern int gi_Vector[3][7][3];
extern int gi_Tree[81][3][3];

extern int gi_seq_cnt;
extern int gi_test_vect[8];

// Vector's section lookup table
extern gcl_vector_ab_prime go_vectorTab_ab_prime[3][7];
// Reference vector object, use for storing incomming reference vector's coordinate(double type)
extern gcl_vector_ab_prime_loc go_inSide_ref_vector_ab_prime_loc, go_outSide_ref_vector_ab_prime_loc;
// Array of vector objects. Used for storing 3 vector objects of modulation sequence.
extern gcl_vector_ab_prime go_inSide_vector_sequence[3], go_outSide_vector_sequence[3];
// Switching sequence
extern gcl_vector_seq go_bothSide_vector_sequence[5]; //Storing a whole sequence of a duty cycle

extern gc_SYM_THREE_PHASE_VAL go_SYM_3p_in_vol;
extern gc_SYM_THREE_PHASE_VAL go_SYM_3p_in_cur;

extern gc_SYM_THREE_PHASE_VAL go_SYM_3p_out_vol;
extern gc_SYM_THREE_PHASE_VAL go_SYM_3p_out_cur;

extern gc_SYM_CLARKE_VAL go_SYM_2p_in_vol;
extern gc_SYM_CLARKE_VAL go_SYM_2p_in_cur;

extern gc_SYM_CLARKE_VAL go_SYM_2p_out_vol;
extern gc_SYM_CLARKE_VAL go_SYM_2p_out_cur;



// Function Declaration
void sym_ECat_DATA_Write(gc_SYM_ECAT_DOWN_DATA * ob_ECAT_DOWN_DATA);
void sym_Ecat_DATA_Codec(gc_SYM_ECAT_DOWN_DATA_DECODED * ob_ECAT_DOWN_DATA_NEED_CODEC, gc_SYM_ECAT_DOWN_DATA *ob_ECAT_DOWN_DATA);
uint16_t sym_Ecat_DATA_Codec_VectCodec(int vtype, int vnum);
//void sym_Ecat_DATA_Decode_vector(void); // Called by sym_Ecat_DATA_Decode()
//void sym_Ecat_DATA_Decode(void);


int gf_SYM_vectorTab_ab_prime_init(gcl_vector_ab_prime (*ob_ab_prime)[7]);
int gf_SYM_get_vecotor_location_in_ab_prime(gc_SYM_THREE_PHASE_VAL *abc, gcl_vector_ab_prime_loc *ob_vector_ab_prime_loc);
int gf_SYM_get_vector_sequence(gcl_vector_ab_prime_loc *ob_vector_ab_prime_loc, \
                            gcl_vector_ab_prime (*ob_vector_table)[7], gcl_vector_ab_prime *ob_vector_sequence);
int gf_SYM_sequence_combine(gcl_vector_ab_prime *ob_inSide_vector_sequence,
                            gcl_vector_ab_prime *ob_outSide_vector_sequence, \
                            gcl_vector_seq *ob_bothSide_vector_sequence);
int gf_SYM_data_bridge(gcl_vector_seq *ob_bothSide_vector_sequence, gc_SYM_ECAT_DOWN_DATA_DECODED *ob_ECAT_DATA_NEED_CODEC);


int singles2halfp(void *target, void *source, int numel);
int halfp2singles(void *target, void *source, int numel);
#endif /* INC_SYM_APP_H_ */
