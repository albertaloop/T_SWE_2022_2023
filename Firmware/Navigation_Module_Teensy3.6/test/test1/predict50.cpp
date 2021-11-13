#include <unity.h>
#include "estimator.h"




void test_predict_50 () {
  float x = {0, 0, 1};
  estimator est = estimator();

  for (int i = 0; i < 50; i++) {
    x[0] = x[0] + x[1] * delta + x[2] * half_delta_sqrd;
    x[i] = x[i] + x[2] * delta;
    estimator.predict();
  }

  }

}

int main() {

    UNITY_BEGIN();
    RUN_TEST(test_predict_50);
    
    UNITY_END();
    return 0;
}