#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <utility> 
#include <cassert>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

using namespace std;
// Circualr buffer is like a 1st add 1st out stack.
template<typename T, int Size> 
class CircularBuffer
{
    public:
        // Check if the circular buffer is full or not.
        bool isFull(){
            return full;
        }
        // Check if the circualr buffer is empty or not.
        bool isEmpty(){
            bool temp = false;
            if((head == tail)&&(full != true)){
                temp = true;
            }
            return temp;       
        }
        // "Push to stack", add elements to the buffer.
        bool Write(T element){
            if(isFull()){
                return false;
            }
            else{
                buffer[head] = element;
                head=(head+1) % size;
                numElement++;
                if(head == tail){
                    full = true;
                }
                return true;
            }
        }
        // Cout how many elements are in the circular buffer currently.
        int numElements(){
            return numElement;
        }
        T Peek(){
            return buffer[tail];
        }
        // "Pop out elements", remove the 1st added element from the circular buffer.
        T Read(){
            T read = this->Peek();
            tail=(tail+1) % size;
            full = false;
            numElement--;
            return read;
            
        }
        // Check the total capacity of the circular buffer or the remaining capacity.
        size_t Capacity(){
            return size;
        }
        // Check the available size of the circular buffer.
        size_t RemainingSize(){
            int RemSize = size;
            if(!full){
                if(head >= tail){
                RemSize = size - (head - tail);
                }
                else{
                    RemSize = size-(size + head - tail);
                }   
            }
            return RemSize;
        }
        // Reset the buffer to empty state.
        void Reset(){
            head = tail;
            full = false;
            numElement = 0;
        }
    private:
        int head = 0;
        int tail = 0;
        int size = Size;
        int numElement = 0;
        bool full = false;
        T buffer[Size];

};

#endif