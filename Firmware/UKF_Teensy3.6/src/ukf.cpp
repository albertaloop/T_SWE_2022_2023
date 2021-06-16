#include <arm_math.h>
#include "ukf.h"

ukf::ukf(){
    arm_mat_init_f32(&this->xyz_pos, 1, 3, this->state_vector[0]);
    arm_mat_init_f32(&this->xyz_vel, 1, 3, this->state_vector[3]);
    arm_mat_init_f32(&this->xyz_accel, 1, 3, this->state_vector[6]);
    arm_mat_init_f32(&this->rpy_att, 1, 3, this->state_vector[9]);
    arm_mat_init_f32(&this->rpy_vel, 1, 3, this->state_vector[12]);

}

int ukf::predict() {

    return 1;
}

int ukf::update() {

    return 1;

}