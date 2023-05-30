#include "transforms.h"
#include "myPair.hpp"
#include "matrix_math.h"
#include <cmath>

float temp1;
float temp2;

//https://stackoverflow.com/questions/16266809/convert-from-latitude-longitude-to-x-y/16271669#16271669
mypair<float> transforms::equirectangular(float lon, float lat) {
    // Convert from degrees to radians
    lat = lat * (Pi / 180);
    lon = lon * (Pi / 180);
    // Compute x and y coordinates
    float x = lon * this->RcosPhi;
    float y = R * lat;
    mypair<float> pair(x,y);
    return pair;
}

void transforms::setPhi_naught(float phi_naught) {
    // Convert from degrees to radians
    phi_naught = phi_naught * (Pi / 180);
    // Use phi naught to solve for RcosPhi
    float cosPhi = arm_cos_f32(phi_naught);
    RcosPhi = R * cosPhi;
}

// https://en.wikipedia.org/wiki/Euclidean_distance
float transforms::euclidean(mypair<float> origin, mypair<float> pair) {
    // Find distance from p (origin) to q (pair)
    // (q1 - p1)^2
    temp1 = (float)pair.getFirst() - (float)origin.getFirst();
    temp1 = temp1 * temp1;
    // (q2 - p2)^2
    temp2 = (float)pair.getSecond() - (float)origin.getSecond();
    temp2 = temp2 * temp2;
    // ((q1 - p1)^2 + (q2 - p2)^2)^(1/2)
    return  (sqrt(temp1 + temp2));
}

