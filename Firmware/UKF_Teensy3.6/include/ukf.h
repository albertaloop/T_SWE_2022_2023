#ifndef __UKF__
#define __UKF__

#include <arm_math.h>

#define State_dim 3
#define n State_dim

class ukf {
public:
  ukf(int alpha, int kappa, int beta, float32_t *P);

  // State parameters to be tracked
  float32_t state[n];

  // Sigma point parameters
  int alpha;
  int beta;
  int kappa;

  arm_matrix_instance_f32 P;

  // Upper case Sigma, calculated using "matrix square root"
  arm_matrix_instance_f32 Sigma;

  // Examples:
  // arm_matrix_instance_f32 rpy_att;
  // arm_matrix_instance_f32 rpy_vel;
  // arm_matrix_instance_f32 Rz_Ry_Rx;
  // arm_matrix_instance_f32 Ttheta;

  int predict();

  int update();
};

#endif
