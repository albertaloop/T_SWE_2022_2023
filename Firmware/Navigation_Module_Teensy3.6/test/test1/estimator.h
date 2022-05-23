#ifndef __EST__
#define __EST__

#define delta                   1
#define half_delta_sqrd         0.25   //((0.5 * delta) * (0.5 * delta))

#include "matrix_math.h"

class estimator {
    public:
        float x[3] = {0, 0, 0};
        float x_bar[2];
        estimator();
        void predict();
        void update(float m);
        
    private:
        float Fdata[4] = {1, delta, 0, 1};
        float F_Tdata[4] = {1, 0, delta, 1};
        float Gdata[2] = {half_delta_sqrd, delta};
        float Hdata[2] = {1, 0};
        float Pdata[4] = {1, 0, 1, 0};
        float Idata[4] = {1, 0, 1, 0};
        /* Neglect process noise Q */
        // float Qdata[4] = { , , , };


        float y_bar;

        arm_matrix_instance_f32 H;
        arm_matrix_instance_f32 H_T;

        arm_matrix_instance_f32 F;
        arm_matrix_instance_f32 F_T;
        arm_matrix_instance_f32 G;
        arm_matrix_instance_f32 P;
        arm_matrix_instance_f32 P_bar;
        arm_matrix_instance_f32 I;
        float R = 1;
        /* Neglect process noise Q */
        // arm_matrix_instance_f32 Q;

        
};

#endif