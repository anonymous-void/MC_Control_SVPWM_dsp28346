/*
 * SYM_SVPWM.c
 *
 *  Created on: 2016Äê9ÔÂ27ÈÕ
 *      Author: voidman
 */
#include "DSP2834x_Device.h"
#include "ZXY_app.h"
#include "SYM_app.h"


int gf_SYM_vectorTab_ab_prime_init(gcl_vector_ab_prime (*ob_ab_prime)[7])
{/* Description: Transform hexagon ab vector to rectangle ab-prime vector
 * */
    int trans_ab_prime[2][3] = {{1, 0, -1}, {-1, 1, 0}};
    int vector_abc[3][7][3] = {
            {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}},
            {{0, 0, 0}, {2, 0, 1}, {2, 1, 0}, {1, 2, 0}, {0, 2, 1}, {0, 1, 2}, {1, 0, 2}},
            {{0, 0, 0}, {2, 0, 0}, {2, 2, 0}, {0, 2, 0}, {0, 2, 2}, {0, 0, 2}, {2, 0, 2}}
    };
    int type_num = 0, vector_num = 0, element = 0;
    for (type_num = 0; type_num < 3; type_num++)
    {
        for (vector_num = 0; vector_num < 7; vector_num++)
        {
            for (element = 0; element < 3; element++)
            {
                ob_ab_prime[type_num][vector_num].alpha = \
                trans_ab_prime[0][0] * vector_abc[type_num][vector_num][0] + \
                trans_ab_prime[0][1] * vector_abc[type_num][vector_num][1] + \
                trans_ab_prime[0][2] * vector_abc[type_num][vector_num][2];

                ob_ab_prime[type_num][vector_num].beta = \
                trans_ab_prime[1][0] * vector_abc[type_num][vector_num][0] + \
                trans_ab_prime[1][1] * vector_abc[type_num][vector_num][1] + \
                trans_ab_prime[1][2] * vector_abc[type_num][vector_num][2];

                ob_ab_prime[type_num][vector_num].vector_type = type_num + 1;
                ob_ab_prime[type_num][vector_num].vector_num = vector_num;
            }
        }
    }
    return 0;
}

int gf_SYM_get_vector_type(int ob_alpha, int ob_beta, gcl_vector_ab_prime (*ob_vector_table)[7])
{/* Description: According to an ab-prime vector's alpha component and beta component, return its
 * vector type, I, II, III represented by 1, 2, 3.
 * */
    int vector_type = 0, vector_num = 0;
    for (vector_type = 0; vector_type < 3; vector_type++)
    {
        for (vector_num = 0; vector_num < 7; vector_num ++)
        {
            if (ob_alpha == ob_vector_table[vector_type][vector_num].alpha && \
                ob_beta == ob_vector_table[vector_type][vector_num].beta)
            {
                return ob_vector_table[vector_type][vector_num].vector_type;
            }
        }
    }
    return -1; // Error
}

int gf_SYM_get_vector_num(int ob_alpha, int ob_beta, gcl_vector_ab_prime (*ob_vector_table)[7])
{/* Description: According to an ab-prime vector's alpha component and beta component, return its
 * vector num, 0~6.
 * */
    int vector_type = 0, vector_num = 0;
    for (vector_type = 0; vector_type < 3; vector_type++)
    {
        for (vector_num = 0; vector_num < 7; vector_num ++)
        {
            if (ob_alpha == ob_vector_table[vector_type][vector_num].alpha && \
                ob_beta == ob_vector_table[vector_type][vector_num].beta)
            {
                return ob_vector_table[vector_type][vector_num].vector_num;
            }
        }
    }
    return -1; // Error
}

int gf_SYM_get_vector_quadrant(int ob_alpha, int ob_beta)
{/* Description: According an ab-prime vector's alpha and beta components, return the quadrant where the vector lies in.
 * 1~4 represent for
 *           |
 *     2     |      1
 *           |
 * -----------------------
 *           |
 *     3     |      4
 *           |
 * */
    int quadrant4ret = -1;
    if (ob_alpha > -0.5 && ob_beta > -0.5)
        quadrant4ret = 1;
    else if (ob_alpha < -0.5 && ob_beta > -0.5)
        quadrant4ret = 2;
    else if (ob_alpha < -0.5 && ob_beta < -0.5)
        quadrant4ret = 3;
    else
        quadrant4ret = 4;

    return quadrant4ret;
}

int gf_SYM_get_vector_sequence(gcl_vector_ab_prime_loc *ob_vector_ab_prime_loc, \
                            gcl_vector_ab_prime (*ob_vector_table)[7], gcl_vector_ab_prime *ob_vector_sequence) {
    int quadrant = gf_SYM_get_vector_quadrant(ob_vector_ab_prime_loc->a0_prime, ob_vector_ab_prime_loc->b0_prime);
    double d0_or_3 = -1, d1 = -1, d2 = -1;
    if (ob_vector_ab_prime_loc->flag_lower_or_upper == 0)
    {
        d0_or_3 = ( (1 + ob_vector_ab_prime_loc->a0_prime + ob_vector_ab_prime_loc->b0_prime) - (ob_vector_ab_prime_loc->a_ref + ob_vector_ab_prime_loc->b_ref) );
        d1 = ( ob_vector_ab_prime_loc->a_ref - ob_vector_ab_prime_loc->a0_prime );
        d2 = ( ob_vector_ab_prime_loc->b_ref - ob_vector_ab_prime_loc->b0_prime );
    }
    else
    {
        d0_or_3 = ( (ob_vector_ab_prime_loc->a_ref + ob_vector_ab_prime_loc->b_ref) - (ob_vector_ab_prime_loc->a0_prime + ob_vector_ab_prime_loc->b0_prime + 1) );
        d1 = (ob_vector_ab_prime_loc->b3_prime - ob_vector_ab_prime_loc->b_ref);
        d2 = (ob_vector_ab_prime_loc->a3_prime - ob_vector_ab_prime_loc->a_ref);
    }

    switch (quadrant)
    {
        case 1:
        {
            if (ob_vector_ab_prime_loc->flag_lower_or_upper == 0) {
                // lower section, v0prime/v1prime/v2prime active
                ob_vector_sequence[0].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a0_prime,
                                                                           ob_vector_ab_prime_loc->b0_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[0].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a0_prime,
                                                                         ob_vector_ab_prime_loc->b0_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[0].alpha = ob_vector_ab_prime_loc->a0_prime;
                ob_vector_sequence[0].beta = ob_vector_ab_prime_loc->b0_prime;
                ob_vector_sequence[0].duty = d0_or_3;

            }
            else {
                // upper section, v3prime/v1prime/v2prime active
                ob_vector_sequence[0].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a3_prime,
                                                                           ob_vector_ab_prime_loc->b3_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[0].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a3_prime,
                                                                         ob_vector_ab_prime_loc->b3_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[0].alpha = ob_vector_ab_prime_loc->a3_prime;
                ob_vector_sequence[0].beta = ob_vector_ab_prime_loc->b3_prime;
                ob_vector_sequence[0].duty = d0_or_3;
            }
            ob_vector_sequence[1].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a1_prime,
                                                                       ob_vector_ab_prime_loc->b1_prime,
                                                                       ob_vector_table);
            ob_vector_sequence[1].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a1_prime,
                                                                     ob_vector_ab_prime_loc->b1_prime, ob_vector_table);
            ob_vector_sequence[1].alpha = ob_vector_ab_prime_loc->a1_prime;
            ob_vector_sequence[1].beta = ob_vector_ab_prime_loc->b1_prime;
            ob_vector_sequence[1].duty = d1;

            ob_vector_sequence[2].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a2_prime,
                                                                       ob_vector_ab_prime_loc->b2_prime,
                                                                       ob_vector_table);
            ob_vector_sequence[2].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a2_prime,
                                                                     ob_vector_ab_prime_loc->b2_prime, ob_vector_table);
            ob_vector_sequence[2].alpha = ob_vector_ab_prime_loc->a2_prime;
            ob_vector_sequence[2].beta = ob_vector_ab_prime_loc->b2_prime;
            ob_vector_sequence[2].duty = d2;
            break;
        }

        case 2:
        {
            ob_vector_sequence[0].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a1_prime,
                                                                       ob_vector_ab_prime_loc->b1_prime,
                                                                       ob_vector_table);
            ob_vector_sequence[0].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a1_prime,
                                                                     ob_vector_ab_prime_loc->b1_prime, ob_vector_table);
            ob_vector_sequence[0].alpha = ob_vector_ab_prime_loc->a1_prime;
            ob_vector_sequence[0].beta = ob_vector_ab_prime_loc->b1_prime;
            ob_vector_sequence[0].duty = d1;

            if (ob_vector_ab_prime_loc->flag_lower_or_upper == 0) {
                // lower section, v0prime/v1prime/v2prime active
                ob_vector_sequence[1].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a2_prime,
                                                                           ob_vector_ab_prime_loc->b2_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[1].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a2_prime,
                                                                         ob_vector_ab_prime_loc->b2_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[1].alpha = ob_vector_ab_prime_loc->a2_prime;
                ob_vector_sequence[1].beta = ob_vector_ab_prime_loc->b2_prime;
                ob_vector_sequence[1].duty = d2;

                ob_vector_sequence[2].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a0_prime,
                                                                           ob_vector_ab_prime_loc->b0_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[2].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a0_prime,
                                                                         ob_vector_ab_prime_loc->b0_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[2].alpha = ob_vector_ab_prime_loc->a0_prime;
                ob_vector_sequence[2].beta = ob_vector_ab_prime_loc->b0_prime;
                ob_vector_sequence[2].duty = d0_or_3;
            }
            else {
                // upper section, v3prime/v1prime/v2prime active
                ob_vector_sequence[1].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a3_prime,
                                                                           ob_vector_ab_prime_loc->b3_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[1].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a3_prime,
                                                                         ob_vector_ab_prime_loc->b3_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[1].alpha = ob_vector_ab_prime_loc->a3_prime;
                ob_vector_sequence[1].beta = ob_vector_ab_prime_loc->b3_prime;
                ob_vector_sequence[1].duty = d0_or_3;

                ob_vector_sequence[2].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a2_prime,
                                                                           ob_vector_ab_prime_loc->b2_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[2].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a2_prime,
                                                                         ob_vector_ab_prime_loc->b2_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[2].alpha = ob_vector_ab_prime_loc->a2_prime;
                ob_vector_sequence[2].beta = ob_vector_ab_prime_loc->b2_prime;
                ob_vector_sequence[2].duty = d2;
            }
            break;
        }

        case 3:
        {
            if (ob_vector_ab_prime_loc->flag_lower_or_upper == 0) {
                // lower section, v0prime/v1prime/v2prime active
                ob_vector_sequence[0].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a0_prime,
                                                                           ob_vector_ab_prime_loc->b0_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[0].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a0_prime,
                                                                         ob_vector_ab_prime_loc->b0_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[0].alpha = ob_vector_ab_prime_loc->a0_prime;
                ob_vector_sequence[0].beta = ob_vector_ab_prime_loc->b0_prime;
                ob_vector_sequence[0].duty = d0_or_3;

            }
            else {
                // upper section, v3prime/v1prime/v2prime active
                ob_vector_sequence[0].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a3_prime,
                                                                           ob_vector_ab_prime_loc->b3_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[0].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a3_prime,
                                                                         ob_vector_ab_prime_loc->b3_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[0].alpha = ob_vector_ab_prime_loc->a3_prime;
                ob_vector_sequence[0].beta = ob_vector_ab_prime_loc->b3_prime;
                ob_vector_sequence[0].duty = d0_or_3;
            }
            ob_vector_sequence[1].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a2_prime,
                                                                       ob_vector_ab_prime_loc->b2_prime,
                                                                       ob_vector_table);
            ob_vector_sequence[1].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a2_prime,
                                                                     ob_vector_ab_prime_loc->b2_prime, ob_vector_table);
            ob_vector_sequence[1].alpha = ob_vector_ab_prime_loc->a2_prime;
            ob_vector_sequence[1].beta = ob_vector_ab_prime_loc->b2_prime;
            ob_vector_sequence[1].duty = d2;

            ob_vector_sequence[2].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a1_prime,
                                                                       ob_vector_ab_prime_loc->b1_prime,
                                                                       ob_vector_table);
            ob_vector_sequence[2].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a1_prime,
                                                                     ob_vector_ab_prime_loc->b1_prime, ob_vector_table);
            ob_vector_sequence[2].alpha = ob_vector_ab_prime_loc->a1_prime;
            ob_vector_sequence[2].beta = ob_vector_ab_prime_loc->b1_prime;
            ob_vector_sequence[2].duty = d1;
            break;
        }

        case 4:
        {
            ob_vector_sequence[0].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a2_prime,
                                                                       ob_vector_ab_prime_loc->b2_prime,
                                                                       ob_vector_table);
            ob_vector_sequence[0].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a2_prime,
                                                                     ob_vector_ab_prime_loc->b2_prime, ob_vector_table);
            ob_vector_sequence[0].alpha = ob_vector_ab_prime_loc->a2_prime;
            ob_vector_sequence[0].beta = ob_vector_ab_prime_loc->b2_prime;
            ob_vector_sequence[0].duty = d2;

            if (ob_vector_ab_prime_loc->flag_lower_or_upper == 0) {
                // lower section, v0prime/v1prime/v2prime active
                ob_vector_sequence[1].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a0_prime,
                                                                           ob_vector_ab_prime_loc->b0_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[1].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a0_prime,
                                                                         ob_vector_ab_prime_loc->b0_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[1].alpha = ob_vector_ab_prime_loc->a0_prime;
                ob_vector_sequence[1].beta = ob_vector_ab_prime_loc->b0_prime;
                ob_vector_sequence[1].duty = d0_or_3;

                ob_vector_sequence[2].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a1_prime,
                                                                           ob_vector_ab_prime_loc->b1_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[2].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a1_prime,
                                                                         ob_vector_ab_prime_loc->b1_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[2].alpha = ob_vector_ab_prime_loc->a1_prime;
                ob_vector_sequence[2].beta = ob_vector_ab_prime_loc->b1_prime;
                ob_vector_sequence[2].duty = d1;
            }
            else {
                // upper section, v3prime/v1prime/v2prime active
                ob_vector_sequence[1].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a1_prime,
                                                                           ob_vector_ab_prime_loc->b1_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[1].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a1_prime,
                                                                         ob_vector_ab_prime_loc->b1_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[1].alpha = ob_vector_ab_prime_loc->a1_prime;
                ob_vector_sequence[1].beta = ob_vector_ab_prime_loc->b1_prime;
                ob_vector_sequence[1].duty = d1;

                ob_vector_sequence[2].vector_type = gf_SYM_get_vector_type(ob_vector_ab_prime_loc->a3_prime,
                                                                           ob_vector_ab_prime_loc->b3_prime,
                                                                           ob_vector_table);
                ob_vector_sequence[2].vector_num = gf_SYM_get_vector_num(ob_vector_ab_prime_loc->a3_prime,
                                                                         ob_vector_ab_prime_loc->b3_prime,
                                                                         ob_vector_table);
                ob_vector_sequence[2].alpha = ob_vector_ab_prime_loc->a3_prime;
                ob_vector_sequence[2].beta = ob_vector_ab_prime_loc->b3_prime;
                ob_vector_sequence[2].duty = d0_or_3;
            }
            break;
        }

        default:
            break;
    }
    return 0;
}

int gf_SYM_get_vecotor_location_in_ab_prime(gc_SYM_THREE_PHASE_VAL *abc, gcl_vector_ab_prime_loc *ob_vector_ab_prime_loc)
{// update reference vectors' loaction(alpha beta prime coordinate), and recalculate the related location
    static const float32 trans_mat[2][3] = { {1, 0, -1}, {-1, 1, 0}};
    // abc -> ab prime transformation
    ob_vector_ab_prime_loc->a_ref = trans_mat[0][0] * abc->a_val + trans_mat[0][1] * abc->b_val + trans_mat[0][2] * abc->c_val;
    ob_vector_ab_prime_loc->b_ref = trans_mat[1][0] * abc->a_val + trans_mat[1][1] * abc->b_val + trans_mat[1][2] * abc->c_val;

    ob_vector_ab_prime_loc->a0_prime = (int)floor(ob_vector_ab_prime_loc->a_ref);
    ob_vector_ab_prime_loc->b0_prime = (int)floor(ob_vector_ab_prime_loc->b_ref);

    ob_vector_ab_prime_loc->a1_prime = ob_vector_ab_prime_loc->a0_prime + 1;
    ob_vector_ab_prime_loc->a2_prime = ob_vector_ab_prime_loc->a0_prime;
    ob_vector_ab_prime_loc->a3_prime = ob_vector_ab_prime_loc->a0_prime + 1;

    ob_vector_ab_prime_loc->b1_prime = ob_vector_ab_prime_loc->b0_prime;
    ob_vector_ab_prime_loc->b2_prime = ob_vector_ab_prime_loc->b0_prime + 1;
    ob_vector_ab_prime_loc->b3_prime = ob_vector_ab_prime_loc->b0_prime + 1;

    if (ob_vector_ab_prime_loc->a0_prime + ob_vector_ab_prime_loc->b0_prime >= ob_vector_ab_prime_loc->a_ref + ob_vector_ab_prime_loc->b_ref - 1)
        ob_vector_ab_prime_loc->flag_lower_or_upper = 0; // lower section
    else
        ob_vector_ab_prime_loc->flag_lower_or_upper = 1; // upper section

    return 0;
}



int gf_SYM_sequence_combine(gcl_vector_ab_prime *ob_inSide_vector_sequence,
                            gcl_vector_ab_prime *ob_outSide_vector_sequence, \
                            gcl_vector_seq *ob_bothSide_vector_sequence)
{/* Description: This function combine the input side duty cycle sequence and output side duty cycle sequence,
 *  yield a sequence in gcl_vector_seq class object with input and output sides' vector number, type and duty cycle.
 *  Input args: ob_inSide_vector_sequence[3] and ob_outSide_vector_sequence[3], only vector_type, vector_num and duty are used
 *              other member of the object are ignored.
 *  Output args: ob_bothSide_vector_sequence[5]
 * */
    gcl_vector_ab_prime new_inSide_vector_sequence[3], new_outSide_vector_sequence[3]; // This object store duty cycle incremently
    gcl_vector_ab_prime *p_in = new_inSide_vector_sequence, *p_out = new_outSide_vector_sequence;
    gcl_vector_seq *cp = ob_bothSide_vector_sequence;
    int p_in_cnt = 0, p_out_cnt = 0;

    new_inSide_vector_sequence[0] = ob_inSide_vector_sequence[0];
    new_inSide_vector_sequence[1] = ob_inSide_vector_sequence[1];
    new_inSide_vector_sequence[2] = ob_inSide_vector_sequence[2];

    new_outSide_vector_sequence[0] = ob_outSide_vector_sequence[0];
    new_outSide_vector_sequence[1] = ob_outSide_vector_sequence[1];
    new_outSide_vector_sequence[2] = ob_outSide_vector_sequence[2];
    //-----------------------------------------------------------
    new_inSide_vector_sequence[1].duty = ob_inSide_vector_sequence[0].duty + ob_inSide_vector_sequence[1].duty;
    new_inSide_vector_sequence[2].duty = 1;

    new_outSide_vector_sequence[1].duty = ob_outSide_vector_sequence[0].duty + ob_outSide_vector_sequence[1].duty;
    new_outSide_vector_sequence[2].duty = 1;
    //-----------------------------------------------------------
    cp[0].duty = 1.0; cp[1].duty = 1.0; cp[2].duty = 1.0; cp[3].duty = 1.0; cp[4].duty = 1.0;
    while ((p_in_cnt + p_out_cnt) < 5)
    {
        if (p_in[p_in_cnt].duty < p_out[p_out_cnt].duty)
        {
            cp[p_in_cnt + p_out_cnt].vector_type_in = p_in[p_in_cnt].vector_type;
            cp[p_in_cnt + p_out_cnt].vector_type_out = p_out[p_out_cnt].vector_type;
            cp[p_in_cnt + p_out_cnt].vector_num_in = p_in[p_in_cnt].vector_num;
            cp[p_in_cnt + p_out_cnt].vector_num_out = p_out[p_out_cnt].vector_num;
            cp[p_in_cnt + p_out_cnt].duty = p_in[p_in_cnt].duty;
            p_in_cnt ++;
            if (p_in_cnt >= 3 || p_in[p_in_cnt - 1].duty >= 1)
                break;

        } else {
            cp[p_in_cnt + p_out_cnt].vector_type_in = p_in[p_in_cnt].vector_type;
            cp[p_in_cnt + p_out_cnt].vector_type_out = p_out[p_out_cnt].vector_type;
            cp[p_in_cnt + p_out_cnt].vector_num_in = p_in[p_in_cnt].vector_num;
            cp[p_in_cnt + p_out_cnt].vector_num_out = p_out[p_out_cnt].vector_num;
            cp[p_in_cnt + p_out_cnt].duty = p_out[p_out_cnt].duty;
            p_out_cnt ++;
            if (p_out_cnt >= 3 || p_out[p_out_cnt - 1].duty >= 1)
                break;
        }
    }
    ob_bothSide_vector_sequence[4].duty = ob_bothSide_vector_sequence[4].duty - ob_bothSide_vector_sequence[3].duty;
    ob_bothSide_vector_sequence[3].duty = ob_bothSide_vector_sequence[3].duty - ob_bothSide_vector_sequence[2].duty;
    ob_bothSide_vector_sequence[2].duty = ob_bothSide_vector_sequence[2].duty - ob_bothSide_vector_sequence[1].duty;
    ob_bothSide_vector_sequence[1].duty = ob_bothSide_vector_sequence[1].duty - ob_bothSide_vector_sequence[0].duty;
    return 0;
}

int gf_SYM_data_bridge(gcl_vector_seq *ob_bothSide_vector_sequence, gc_SYM_ECAT_DOWN_DATA_DECODED *ob_ECAT_DATA_NEED_CODEC)
{/* Description: This func copy vector sequence data[5] to  gc_SYM_ECAT_DOWN_DATA_DECODED object, which is waited for
 *               codec to Uint16 format for transmission.
 * */
    ob_ECAT_DATA_NEED_CODEC->tree[0] = 0; // SYM: without voltage balance ctrl, tree can be specified randomly
    ob_ECAT_DATA_NEED_CODEC->tree[1] = 0;
    ob_ECAT_DATA_NEED_CODEC->tree[2] = 0;
    ob_ECAT_DATA_NEED_CODEC->tree[3] = 0;
    ob_ECAT_DATA_NEED_CODEC->tree[4] = 0;


    ob_ECAT_DATA_NEED_CODEC->duty[0] = ob_bothSide_vector_sequence[0].duty;
    ob_ECAT_DATA_NEED_CODEC->duty[1] = ob_bothSide_vector_sequence[1].duty;
    ob_ECAT_DATA_NEED_CODEC->duty[2] = ob_bothSide_vector_sequence[2].duty;
    ob_ECAT_DATA_NEED_CODEC->duty[3] = ob_bothSide_vector_sequence[3].duty;
    ob_ECAT_DATA_NEED_CODEC->duty[4] = ob_bothSide_vector_sequence[4].duty;


    ob_ECAT_DATA_NEED_CODEC->vector_input[0].vtype = ob_bothSide_vector_sequence[0].vector_type_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[1].vtype = ob_bothSide_vector_sequence[1].vector_type_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[2].vtype = ob_bothSide_vector_sequence[2].vector_type_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[3].vtype = ob_bothSide_vector_sequence[3].vector_type_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[4].vtype = ob_bothSide_vector_sequence[4].vector_type_in;

    ob_ECAT_DATA_NEED_CODEC->vector_input[0].vnum = ob_bothSide_vector_sequence[0].vector_num_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[1].vnum = ob_bothSide_vector_sequence[1].vector_num_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[2].vnum = ob_bothSide_vector_sequence[2].vector_num_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[3].vnum = ob_bothSide_vector_sequence[3].vector_num_in;
    ob_ECAT_DATA_NEED_CODEC->vector_input[4].vnum = ob_bothSide_vector_sequence[4].vector_num_in;


    ob_ECAT_DATA_NEED_CODEC->vector_output[0].vtype = ob_bothSide_vector_sequence[0].vector_type_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[1].vtype = ob_bothSide_vector_sequence[1].vector_type_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[2].vtype = ob_bothSide_vector_sequence[2].vector_type_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[3].vtype = ob_bothSide_vector_sequence[3].vector_type_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[4].vtype = ob_bothSide_vector_sequence[4].vector_type_out;

    ob_ECAT_DATA_NEED_CODEC->vector_output[0].vnum = ob_bothSide_vector_sequence[0].vector_num_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[1].vnum = ob_bothSide_vector_sequence[1].vector_num_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[2].vnum = ob_bothSide_vector_sequence[2].vector_num_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[3].vnum = ob_bothSide_vector_sequence[3].vector_num_out;
    ob_ECAT_DATA_NEED_CODEC->vector_output[4].vnum = ob_bothSide_vector_sequence[4].vector_num_out;
    return 0;
}

