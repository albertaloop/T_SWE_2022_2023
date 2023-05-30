#include "estimator.h"
#include <stdio.h>
#include <iostream>
using namespace std;

void test_50 () {

    float x[3] = {0.0f, 0.0f, 1.0f};
    float P[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    // estimator contains float array x[3] initialized to the same as x[]
    // x[2] and est.x[2] are equivalent to acceleration, which never changes for this test
    estimator est = estimator();
    est.x[2] = 1;
    // Calculate 50 points using kinematic equations and compare against the esimator
    // which uses the same kinematic equations and inital conditions.
    for(int i=0; i < 50; i++) {
      // printf("iteration %d\n", i);
      x[0] = x[0] + (x[1] * delta) + (x[2] * half_delta_sqrd);
      x[1] = x[1] + (x[2] * delta);
      P[0] = P[0] + (P[2] * delta) + (P[1] * delta) + (P[3] * delta * delta);
      P[1] = P[1] + (P[3] * delta);
      P[2] = P[2] + (P[3] * delta);
      // printf("Predict P[0] = %.6f\n", P[0]);
      // printf("Predict P[1] = %.6f\n", P[1]);
      // printf("Predict P[2] = %.6f\n", P[2]);
      // printf("Predict P[3] = %.6f\n", P[3]);
      est.predict();
      float y = (rand() % 3);
      est.update(x[0] + y);

      float S = 1 / (P[0] + 1.0f);
      // printf("y = %.6f\n", y);


      // printf("S = %.6f\n", S);
      
      float xerr_div_S = P[0] * S;
      // printf("xerr_div_S = %.6f\n", xerr_div_S);

      float xvcov_div_S = P[1] * S;
      float xerr_xvcov_div_S = (P[0]*P[1]) * S;
      float xvcov_vxcov_div_S = (P[1]*P[2]) * S;
      x[0] = x[0] + (xerr_div_S) * y;
      x[1] = x[1] + (xvcov_div_S) * y;
      P[0] = P[0] * (1 - xerr_div_S);
      P[1] = P[1] * (1 - xerr_div_S);
      P[2] = P[2] - (xerr_xvcov_div_S);
      P[3] = P[3] - (xvcov_vxcov_div_S);
      // printf("Update P[0] = %.6f\n", P[0]);
      // printf("Update P[1] = %.6f\n", P[1]);
      // printf("Update P[2] = %.6f\n", P[2]);
      // printf("Update P[3] = %.6f\n", P[3]);


      // https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
      // printf("x[0] = %.6f, est.x[0] = %.6f\n", x[0], est.x[0]);
      // printf("x[1] = %.6f, est.x[1] = %.6f\n", x[1], est.x[1]);
    }
    printf("After 50 iterations, x[0] = %.6f, x[1] = %.6f, est.x[0] = %.6f, est.x[1] = %.6f\n", x[0], x[1], est.x[0], est.x[1]);
}

int main() {
    test_50();

    return 0;
}