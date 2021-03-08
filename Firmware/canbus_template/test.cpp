#include "circular_buffer.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    int num;
    CircularBuffer<int, 5> buffer;
    while(true){
        int num;
        cin >> num;
        buffer.Write(num);
        buffer.Write(9);
        cout << "Read: ";
        cout << buffer.Read() << endl;
        cout << "numElements: ";
        cout << buffer.numElements() << endl;
        cout << "Capacity: ";
        cout << buffer.Capacity() << endl;
        cout << "Remaining size: " << buffer.RemainingSize() << endl;
    }
    buffer.Reset();
    return 0;
}