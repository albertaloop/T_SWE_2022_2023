#include <stdio.h>
#include <string>
#include <iostream>
#include "matrix_math.h"
using namespace std;



int main() {
    FILE *fp; // declaration of file pointer
    char con[100]; // variable to read the content
    double rads[13];
    fp =fopen("radians.txt","r");// opening of file
    if (!fp)// checking for error
    return 1;
    int i = 0;
    while (fgets(con,100, fp)!=NULL) {// reading file content
      rads[i] = std::stof(con);
      printf("%s", con);
      i++;
    }
    fclose(fp); // closing file

    double cosines[13];
    for (int i = 0; i < 13; i++) {
      cosines[i] = arm_cos_f32(rads[i]);
      printf("Radians input = %.11f\n",rads[i]);
      printf("Cosine output = %.11f\n", cosines[i]);
    }

    fp = fopen("cosines.txt", "w");   
    if(fp) {//will be null if failed to open
      for(i = 0; i < 13; i++) {
        fprintf(fp, "%.11f\n", cosines[i]);
      }
    }
    fclose(fp);


    return 0;
}