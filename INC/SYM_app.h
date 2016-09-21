/*
 * SYM_app.h
 *
 *  Created on: 2016��9��20��
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


extern gc_SYM_ECAT_CMD_ALL				go_SYM_ECAT_CMD;
extern gc_SYM_ECAT_DOWN_DATA			go_SYM_ECAT_DOWN_DATA;
extern gc_SYM_ECAT_DOWN_DATA_DECODED	go_SYM_ECAT_DOWN_DATA_NEED_CODEC;
extern gc_SYM_ECAT_DOWN_DATA_DECODED    go_SYM_ECAT_DOWN_DATA_DECODED;

extern int gi_Vector[3][7][3];
extern int gi_Tree[81][3][3];

extern int gi_seq_cnt;
extern int gi_test_vect[8];


// Function Declaration
void sym_Ecat_DATA_Write(void);
void sym_Ecat_DATA_Codec(void);
void sym_Ecat_DATA_Decode_vector(void); // Called by sym_Ecat_DATA_Decode()
void sym_Ecat_DATA_Decode(void);
int singles2halfp(void *target, void *source, int numel);
int halfp2singles(void *target, void *source, int numel);
#endif /* INC_SYM_APP_H_ */
