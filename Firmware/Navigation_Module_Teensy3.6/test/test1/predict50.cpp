#include <unity.h>
#include "estimator.h"
#include <stdio.h>

void __attribute__((optimize("O0"))) test_predict_50 () {

    float x[3] = {0, 0, 1};
    // estimator contains float array x[3] initialized to the same as x[]
    // x[2] and est.x[2] are equivalent to acceleration, which never changes for this test
    estimator est = estimator();

    // Calculate 50 points using kinematic equations and compare against the esimator
    // which uses the same kinematic equations and inital conditions.
    for(int i=0; i < 50; i++) {
      x[0] = x[0] + (x[1] * delta) + (x[2] * half_delta_sqrd * 0.5);
      x[1] = x[1] + (x[2] * delta);
      est.predict();

      printf("hi");


      if (x[0] != est.x[0]) {
        printf("x[0] = %.6f, est.x[0] = %.6f", x[0], est.x[0]);
        TEST_FAIL_MESSAGE("Position mismatch");
      }
      if (x[1] != est.x[1]) {
        printf("x[1] = %.6f, est.x[1] = %.6f", x[1], est.x[1]);
        // TEST_FAIL_MESSAGE("Velocity mismatch");
      }
    }
    printf("After 50 iterations, x[0] = %.6f, x[1] = %.6f, est.x[0] = %.6f, est.x[1] = %.6f\n", x[0], x[1], est.x[0], est.x[1]);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_predict_50);
    UNITY_END();
    return 0;
}