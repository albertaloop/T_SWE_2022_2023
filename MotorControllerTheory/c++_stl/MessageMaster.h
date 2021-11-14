#include <Arduino.h>
#include <vector>
using namespace std;

/*
  Contains CANbus messages.
*/
class MessageMaster {
  public:
    vector<vector<uint8_t>> buildSndMessages();
    vector<vector<uint8_t>> buildRcvMessages();
};