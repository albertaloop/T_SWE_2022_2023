#include <arm_math.h>
#include "ukf.h"

ukf::ukf(){
    arm_mat_init_f32(&this->x, 1, 3, this->state_vector[0]);
    arm_mat_init_f32(&this->P, 1, 3, );
    arm_mat_init_f32(&this->H, 1, 3, );
    arm_mat_init_f32(&this->F, 1, 3, );
    arm_mat_init_f32(&this->R, 1, 3, );
    arm_mat_init_f32(&this->Q, 1, 3, );
    arm_mat_init_f32(&this->X, 1, 3, );
    arm_mat_init_f32(&this->Y, 1, 3, );
    arm_mat_init_f32(&this->L, 1, 3, );
    arm_mat_init_f32(&this->x_bar, 1, 3, this->state_vector[0]);
    arm_mat_init_f32(&this->P_bar, 1, 3, this->state_vector[0]);



}

int ukf::predict() {
    // Compute Sigma points
    for (int i = 0; i < ; i++) {
        
    }
    // Compute weights

    // x_bar =

    // P_bar =

    return 1;
}

int ukf::update() {

    return 1;

}