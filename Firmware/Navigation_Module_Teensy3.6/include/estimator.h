#ifndef __EST__
#define __EST__

#define delta                   1
#define half_delta_sqrd         0.25   //((0.5 * delta) * (0.5 * delta))

// #include <stdint.h>
typedef unsigned short uint16_t;
// typedef unsigned int uint32_t;

typedef float float32_t;

// typedef enum
//   {
//     ARM_MATH_SUCCESS        =  0,        /**< No error */
//     ARM_MATH_ARGUMENT_ERROR = -1,        /**< One or more arguments are incorrect */
//     ARM_MATH_LENGTH_ERROR   = -2,        /**< Length of data buffer is incorrect */
//     ARM_MATH_SIZE_MISMATCH  = -3,        /**< Size of matrices is not compatible with the operation */
//     ARM_MATH_NANINF         = -4,        /**< Not-a-number (NaN) or infinity is generated */
//     ARM_MATH_SINGULAR       = -5,        /**< Input matrix is singular and cannot be inverted */
//     ARM_MATH_TEST_FAILURE   = -6         /**< Test Failed */
// } arm_status;



typedef struct
{
    uint16_t numRows;     /**< number of rows of the matrix.     */
    uint16_t numCols;     /**< number of columns of the matrix.  */
    float *pData;     /**< points to the data of the matrix. */
} arm_matrix_instance_f32;



class estimator {
    public:
        float x_bar[3] = {0, 0, 0};

        estimator();
        void predict();

        void arm_mat_init_f32(
            arm_matrix_instance_f32 * S,
            uint16_t nRows,
            uint16_t nColumns,
            float * pData);
        int arm_mat_mult_f32(
            const arm_matrix_instance_f32 * pSrcA,
            const arm_matrix_instance_f32 * pSrcB,
            arm_matrix_instance_f32 * pDst);
        int arm_mat_add_f32(
            const arm_matrix_instance_f32 * pSrcA,
            const arm_matrix_instance_f32 * pSrcB,
            arm_matrix_instance_f32 * pDst);
        int arm_mat_scale_f32(
            const arm_matrix_instance_f32 * pSrc,
            float32_t                 scale,
            arm_matrix_instance_f32 * pDst);
        
    private:
        float Fdata[4] = {1, delta, 0, 1};
        float Gdata[2] = {half_delta_sqrd, delta};
        arm_matrix_instance_f32 F;
        arm_matrix_instance_f32 G;
        
};

#endif