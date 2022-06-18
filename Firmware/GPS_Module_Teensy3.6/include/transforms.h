
#ifndef _TRANSFORMS_H_
#define _TRANSFORMS_H_
#include "myPair.hpp"
class transforms {

    public:
        mypair<float> equirectangular(float lat, float lon);
        float euclidean(mypair<float> origin, mypair<float> pair);
        void setPhi_naught(float phi_naught);
    
    private:
        float R = 6371000.0f;
        float RcosPhi;
        float Pi = 3.141593;
};

#endif