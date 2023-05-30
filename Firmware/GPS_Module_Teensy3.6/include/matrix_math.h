
#ifndef _MATRIX_MATH_H_
#define _MATRIX_MATH_H_

typedef unsigned short uint16_t;

typedef float float32_t;

typedef struct
{
    uint16_t numRows;     /**< number of rows of the matrix.     */
    uint16_t numCols;     /**< number of columns of the matrix.  */
    float *pData;     /**< points to the data of the matrix. */
} arm_matrix_instance_f32;

void arm_mat_init_f32(
            arm_matrix_instance_f32 * S,
            uint16_t nRows,
            uint16_t nColumns,
            float * pData);

void arm_mat_mult_f32(
    const arm_matrix_instance_f32 * pSrcA,
    const arm_matrix_instance_f32 * pSrcB,
    arm_matrix_instance_f32 * pDst);

void arm_mat_add_f32(
    const arm_matrix_instance_f32 * pSrcA,
    const arm_matrix_instance_f32 * pSrcB,
    arm_matrix_instance_f32 * pDst);

void arm_mat_scale_f32(
    const arm_matrix_instance_f32 * pSrc,
    float32_t                 scale,
    arm_matrix_instance_f32 * pDst);

void arm_mat_sub_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst);
  
void arm_mat_trans_f32(
    const arm_matrix_instance_f32 * pSrc,
    arm_matrix_instance_f32 * pDst);

float32_t arm_cos_f32(
    float32_t x);

void arm_mat_inverse_f32(
    const arm_matrix_instance_f32 * pSrc,
    arm_matrix_instance_f32 * pDst);

#endif