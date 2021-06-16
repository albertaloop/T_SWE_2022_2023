#include ukf.h

ukf::ukf(float32_t * pData){
    arm_mat_init_f32(arm_matrix_instance_f32 * S, 1, 15, pData)
}

ukf::predict() {

}

ukf::update() {

}