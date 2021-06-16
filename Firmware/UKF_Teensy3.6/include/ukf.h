#ifndef __UKF__
#define __UKF__

#include <arm_math.h>

class ukf {
    public:
        ukf();   
        arm_matrix_instance_f32 xyz_pos; 
        arm_matrix_instance_f32 xyz_vel;
        arm_matrix_instance_f32 xyz_accel;
        arm_matrix_instance_f32 rpy_att;
        arm_matrix_instance_f32 rpy_vel;

        arm_matrix_instance_f32 Rz_Ry_Rx;
        arm_matrix_instance_f32 Ttheta;
        arm_matrix_instance_f32 sigma_points;

        float32_t state_vector[15];
        int update();
        int predict();

};

#endif