#include "transforms.h"
#include "myPair.hpp"
#include "matrix_math.h"

float R = 6371000.0f;
float RcosPhi;
float Pi = 3.141593;


//https://stackoverflow.com/questions/16266809/convert-from-latitude-longitude-to-x-y/16271669#16271669

mypair<float> equirectangular(float lat, float lon) {
    lat = lat * (Pi / 180);
    lon = lon * (Pi / 180);
    float x = lon * RcosPhi;
    float y = R * lat;
    mypair<float> pair(x,y);
    return pair;
}

void setPhi_naught(float phi_naught) {
    float cosPhi = arm_cos_f32(phi_naught);
    RcosPhi = R * cosPhi;
}

