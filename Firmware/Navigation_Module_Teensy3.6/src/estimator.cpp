#include "estimator.h"

// void arm_mat_init_f32(
//     arm_matrix_instance_f32 * S,
//     uint16_t nRows,
//     uint16_t nColumns,
//     float * pData);

estimator::estimator() {
    arm_mat_init_f32(&this->F, 2, 2, estimator::Fdata);
    arm_mat_init_f32(&this->G, 2, 1, estimator::Gdata);
}

void estimator::arm_mat_init_f32(
  arm_matrix_instance_f32 * S,
  uint16_t nRows,
  uint16_t nColumns,
  float * pData)
{
  /* Assign Number of Rows */
  S->numRows = nRows;

  /* Assign Number of Columns */
  S->numCols = nColumns;

  /* Assign Data pointer */
  S->pData = pData;
}


int estimator::arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
        arm_matrix_instance_f32 * pDst)
{
  float32_t *pIn1 = pSrcA->pData;                /* Input data matrix pointer A */
  float32_t *pIn2 = pSrcB->pData;                /* Input data matrix pointer B */
  float32_t *pInA = pSrcA->pData;                /* Input data matrix pointer A */
  float32_t *pInB = pSrcB->pData;                /* Input data matrix pointer B */
  float32_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  float32_t *px;                                 /* Temporary output data matrix pointer */
  float32_t sum;                                 /* Accumulator */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  unsigned int col, i = 0U, row = numRowsA, colCnt;  /* Loop counters */
  int status = 0;                             /* Status of matrix multiplication */

  {
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of row being processed */
      px = pOut + i;

      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pIn2 = pSrcB->pData;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0.0f;

        /* Initialize pointer pIn1 to point to starting address of column being processed */
        pIn1 = pInA;
 /* Initialize cntCnt with number of columns */
        colCnt = numColsA;

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform the multiply-accumulates */
          sum += *pIn1++ * *pIn2;
          pIn2 += numColsB;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Store result in destination buffer */
        *px++ = sum;

        /* Decrement column loop counter */
        col--;

        /* Update pointer pIn2 to point to starting address of next column */
        pIn2 = pInB + (numColsB - col);

      } while (col > 0U);

      /* Update pointer pInA to point to starting address of next row */
      i = i + numColsB;
      pInA = pInA + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Set status as ARM_MATH_SUCCESS */
    status = 1;
  }

  /* Return to application */
  return (status);
}

int estimator::arm_mat_scale_f32(
  const arm_matrix_instance_f32 * pSrc,
        float32_t                 scale,
        arm_matrix_instance_f32 * pDst)
{
  float32_t *pIn = pSrc->pData;                  /* Input data matrix pointer */
  float32_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  unsigned int numSamples;                           /* Total number of elements in the matrix */
  unsigned int blkCnt;                               /* Loop counters */
  int status = 0;
  {
    /* Total number of samples in input matrix */
    numSamples = (unsigned int) pSrc->numRows * pSrc->numCols;
    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;
        while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * scale */

      /* Scale and store result in destination buffer. */
      *pOut++ = (*pIn++) * scale;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Set status as ARM_MATH_SUCCESS */
    status = 1;
  }

  /* Return to application */
  return (status);
}

int estimator::arm_mat_add_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
        arm_matrix_instance_f32 * pDst)
{
  float32_t *pInA = pSrcA->pData;                /* input data matrix pointer A */
  float32_t *pInB = pSrcB->pData;                /* input data matrix pointer B */
  float32_t *pOut = pDst->pData;                 /* output data matrix pointer */

  unsigned int numSamples;                           /* total number of elements in the matrix */
  unsigned int blkCnt;                               /* loop counters */
  int status = 0;                             /* status of matrix addition */
   {
    /* Total number of samples in input matrix */
    numSamples = (unsigned int) pSrcA->numRows * pSrcA->numCols;
    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;
        while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) + B(m,n) */

      /* Add and store result in destination buffer. */
      *pOut++ = *pInA++ + *pInB++;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Set status as ARM_MATH_SUCCESS */
    status = 1;
  }

  /* Return to application */
  return (status);
}

void estimator::predict() {
    // Declare Matrix Data
    float temp2data[3];
    float temp3data[3];
    // Declare Matrixes
    arm_matrix_instance_f32 temp1;
    arm_matrix_instance_f32 temp2;
    arm_matrix_instance_f32 temp3;
    // Initialize Matrixes
    arm_mat_init_f32(&temp1, 2, 1, this->x_bar);
    arm_mat_init_f32(&temp2, 2, 1, temp2data);
    arm_mat_init_f32(&temp3, 2, 1, temp3data);

    // F * x_bar
    arm_mat_mult_f32(&F, &temp1, &temp2);
    // G * a
    arm_mat_scale_f32(&G, x_bar[2], &temp1);
    // F * x_bar + G * a
    arm_mat_add_f32(&temp2, &temp1, &temp3);

    for (int i = 0; i < 2; i++) {
        x_bar[i] = temp3data[i];
    }
}