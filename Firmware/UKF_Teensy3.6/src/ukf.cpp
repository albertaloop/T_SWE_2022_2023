#include "ukf.h"
#include <arm_math.h>

#define Number_of_sigma_points 3

void sigma_points();

ukf::ukf(int alpha, int kappa, int beta, float32_t *P) {

  this->alpha = alpha;
  this->kappa = kappa;
  this->beta = beta;
  this->P.numCols = n;
  this->P.numRows = n;
  this->P.pData = P;

  /*  *To Do *
      Initialize P matrix and Sigma Matrixes
      arm_mat_init_f32(&this->P, 1, 3, );
  */

  /* * To Do *
      Pass predict funciton pointer to this->predict() depending on the filter
     type (pos, vel, angle)
  */

  /* * To Do *
      Calculate Matrix Square root of Covariance Matrix P, store result in
     Matrix Sigma sigma = cholesky(P);
  */

  // Initalize vectors and matrices used in calculations
  //  Examples:
  /* arm_mat_init_f32(&this->x, 1, 3, this->state_vector[0]);
  // arm_mat_init_f32(&this->P, 1, 3, );
  // arm_mat_init_f32(&this->H, 1, 3, );
  // arm_mat_init_f32(&this->F, 1, 3, );
  // arm_mat_init_f32(&this->R, 1, 3, );
  // arm_mat_init_f32(&this->Q, 1, 3, );
  // arm_mat_init_f32(&this->Y, 1, 3, );
  // arm_mat_init_f32(&this->L, 1, 3, );
  // arm_mat_init_f32(&this->x_bar, 1, 3, this->state_vector[0]);
  // arm_mat_init_f32(&this->P_bar, 1, 3, this->state_vector[0]); */
}

// Pass the enumerated type indicating which state vector is being processed.
// The state transition function is selected based on state vector type, and the
// predict step calculations are performed.

//   Position type state vector

int ukf::predict() {

  // Initialize State Transition Matrix
  // arm_matrix_instance_f32 F;

  // Declare sigma point matrix
  arm_matrix_instance_f32 X;
  // Declare Data array for matrix X
  float32_t X_data[2 * n * 3];
  // Compute Sigma points and place result in X_data
  X_data = sigma_points();
  arm_matrix_instance_f32 X;
  // Initialize sigma point matrix X
  arm_mat_init_f32(X, 1, 3, X_data[0]);

  // Compute weights

  // x_bar =

  // P_bar =

  return 1;
}

int ukf::update() { return 1; }

// Van Der Merwes Sigma points algorithm
void sigma_points() {
  int lamda = ((this->alpha) * (this->alpha) * (n + this->kappa)) - n;
  // Declare matrix
  // n is the dimension of the state vector
  float X[2 * n * 3];
  arm_matrix_instance_f32 choleskyd = cholesky(P);
  // Generate columns
  for (int i = 0; i < 3; i++) {
    // Generate Rows
    X[0] = state[i];
    for (int j = i; j < n; j++) {
      X[j] = state[i] + ((n + lamda) * Sigma[i]);
    }
    for (int j = n; j < 2n; j++) {
      X[j] = state[i] + ((n + lamda) * Sigma[i - n]);
    }
  }
}

// Cholesky - Crout algorithm :
// https://en.wikipedia.org/wiki/Cholesky_decomposition
arm_matrix_instance_f32 cholesky(arm_matrix_instance_f32 P) {
  arm_matrix_instance_f32 choleskyd;
  float32_t L[9];
  float32_t A[9];

  float32_t *pData = P.pData;
  for (int i = 0; i < 9; i++) {
    L[i] = *pData++;
  }
  for (int j = 0; j < 9; j++) {
    float sum = 0;

    for (int i = 0; i < 9; i++) {
      choleskyarray[i] = 0;
    }
    for (int k = 0; k < j; k++) {
      sum += L[(j * 3) + k];
    }

    for (int i = 0; i < 9; i++) {
      pData = P.pData;
      pData += i * 3 + j;
      if (i == j) {
        L[(i * 3) + j] = sqrt(*pData - sum);
      }
      L[(i * 3) + j] = (1.0 / L[(j * 3) + j] * (*pData - sum));
    }
  }
  arm_matrix_init_f32(choleskyd, 3, 3, L[0]);
  return (choleskyd);
}
