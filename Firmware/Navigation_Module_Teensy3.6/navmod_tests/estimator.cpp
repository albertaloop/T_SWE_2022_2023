#include "estimator.h"
#include "matrix_math.h"
#include <stdio.h>


//https://en.wikipedia.org/wiki/Kalman_filter

// Declare Matrixes
arm_matrix_instance_f32 temp_x;
arm_matrix_instance_f32 temp_xbar;
arm_matrix_instance_f32 temp1x1_1;
arm_matrix_instance_f32 temp1x2_1;
arm_matrix_instance_f32 temp2x1_1;
arm_matrix_instance_f32 temp2x1_2;
arm_matrix_instance_f32 temp2x1_3;
arm_matrix_instance_f32 temp2x2_1;
arm_matrix_instance_f32 temp2x2_2;

// Declare Matrix Data
float temp1x1_1data[1];
float temp1x2_1data[2];
float temp2x1_1data[2];
float temp2x1_2data[2];
float temp2x1_3data[2];
float temp2x2_1data[4];
float temp2x2_2data[4];

estimator::estimator() {
    arm_mat_init_f32(&this->F, 2, 2, estimator::Fdata);
    arm_mat_init_f32(&this->F_T, 2, 2, estimator::F_Tdata);
    arm_mat_init_f32(&this->G, 2, 1, estimator::Gdata);
    arm_mat_init_f32(&this->H, 1, 2, estimator::Hdata);
    arm_mat_init_f32(&this->H_T, 2, 1, estimator::Hdata);
    arm_mat_init_f32(&this->P, 2, 2, estimator::Pdata);
    arm_mat_init_f32(&this->I, 2, 2, estimator::Idata);
    arm_mat_init_f32(&this->P_bar, 2, 2, estimator::P_bardata);

    // 1x1 temp matrix
    arm_mat_init_f32(&temp1x1_1, 1, 1, temp1x1_1data);

    // 1x2 temp matrixes
    arm_mat_init_f32(&temp1x2_1, 1, 2, temp1x2_1data);

    // 2x1 temp matrixes
    arm_mat_init_f32(&temp2x1_1, 2, 1, temp2x1_1data);
    arm_mat_init_f32(&temp2x1_2, 2, 1, temp2x1_2data);
    arm_mat_init_f32(&temp2x1_3, 2, 1, temp2x1_3data);

    // 2x2 temp matrixes
    arm_mat_init_f32(&temp2x2_1, 2, 2, temp2x2_1data);
    arm_mat_init_f32(&temp2x2_2, 2, 2, temp2x2_2data);
}

void estimator::predict() {
    arm_mat_init_f32(&temp_x, 2, 1, x);

    /* x_bar = Fx + Ga */
    // F * x_bar
    arm_mat_mult_f32(&this->F, &temp_x, &temp2x1_1);
    // G * a
    arm_mat_scale_f32(&this->G, x[2], &temp2x1_2);
    // F * x_bar + G * a
    arm_mat_add_f32(&temp2x1_1, &temp2x1_2, &temp2x1_3);
    for (int i = 0; i < 2; i++) {
        x_bar[i] = temp2x1_3data[i];
    }

    /* P_bar = FPF^T + Q */
    // F * P
    arm_mat_mult_f32(&this->F, &this->P, &temp2x2_1);
    // F * P * F^T
    arm_mat_mult_f32(&temp2x2_1, &this->F_T, &this->P_bar);
    /* Use this line instead if incorporating process noise Q */
    // arm_mat_mult_f32(&temp2x2_1, &this->F_T, &temp2x2_2);
    // FPF^T + Q
    /* Neglect Process Noise Q */
    // arm_mat_add_f32(&temp2x2_2, &this->Q, &this->P_bar);
}

void estimator::update(float z) {
    arm_mat_init_f32(&temp_xbar, 2, 1, x_bar);
    
    /* y = z - Hx_bar */
    // H * x_bar
    arm_mat_mult_f32(&this->H, &temp_xbar, &temp1x1_1);
    // y = z - H * x_bar
    float y = z - temp1x1_1data[0];

    /* S = HP_barH^T + R */
    // H * P_bar
    arm_mat_mult_f32(&this->H, &this->P_bar, &temp1x2_1);
    // H * P_bar * H^T
    arm_mat_mult_f32(&temp1x2_1, &this->H_T, &temp1x1_1);
    // S = HP_barH^T + R
    float S = temp1x1_1data[0] + this->R; 


    /* K = P_barH^TS^-1 */
    // S^-1
    S = 1/S;
    // P_bar * H^T
    arm_mat_mult_f32(&this->P_bar, &this->H_T, &temp2x1_1);
    // K = P_bar * H^T * S^-1
    arm_mat_scale_f32(&temp2x1_1, S, &temp2x1_2);

    /* x = x_bar + Ky */
    // K * y
    arm_mat_scale_f32(&temp2x1_2, y, &temp2x1_1);
    // x = x_bar + Ky
    arm_mat_add_f32(&temp_xbar, &temp2x1_1, &temp2x1_3);
    for(int i = 0; i < 2; i++) {
        x[i] = temp2x1_3data[i];
    }

    /* P = (I - KH)P_bar */
    // K * H
    arm_mat_mult_f32(&temp2x1_2, &this->H, &temp2x1_1);
    // I - K * H
    arm_mat_sub_f32(&this->I, &temp2x1_1, &temp2x2_2);
    // P = (I-KH) * P_bar
    arm_mat_mult_f32(&temp2x2_2, &this->P_bar, &this->P);
}

float estimator::getPosPbar() {
    return P_bardata[0];
}

float estimator::getPosVelCov() {
    return P_bardata[2];
}
