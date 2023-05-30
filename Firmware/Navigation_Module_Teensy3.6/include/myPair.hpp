
#ifndef _MYPAIR_H_
#define _MYPAIR_H_

template <class T>
class mypair {
    T values [2];
  public:
    mypair (T first, T second)
    {
      values[0]=first; values[1]=second;
    }
}; 

#endif