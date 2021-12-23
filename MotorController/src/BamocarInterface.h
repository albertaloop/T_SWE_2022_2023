#ifndef _BMCR_H_
#define _BMCR_H_

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "MessageMaster.h"

enum RegIDs
{
    NominalMotorFrequency = 0x05,
    MotorNominalVoltage = 0x06,
    DCTimePreMag = 0x07,
    DCVoltages = 0x08,
    PowerFactor = 0x0E,
    CurrentActualVal = 0x20,
    IMaxInUse = 0x24,
    ICMDInUse = 0x25,
    MotorTemp = 0x49,
    PowerStageTemp = 0x4A,
    AirTemp = 0x4B,
    CurrentDerateTemp = 0x4C,
    CurrentActualI1 = 0x54,
    CurrentActualI2 = 0x55,
    CurrentActualI3 = 0x56,
    FilteredActualI3 = 0x5F,
    Filter = 0x60,
    MainsSupplyVoltage = 0x64, //the extra s is not a typo
    BatteryVoltage = 0x66
};

enum RegInternalIndices
{
  NominalMotorFrequency = 0,
  MotorNominalVoltage = 1,
  DCTimePreMag = 2,
  DCVoltages = 3,
  PowerFactor = 4,
  CurrentActualVal = 5,
  IMaxInUse = 6,
  ICMDInUse = 7,
  MotorTemp = 8,
  PowerStageTemp = 9,
  AirTemp = 10,
  CurrentDerateTemp = 11,
  CurrentActualI1 = 12,
  CurrentActualI2 = 13,
  CurrentActualI3 = 14,
  FilteredActualI3 = 15,
  Filter = 16,
  MainsSupplyVoltage = 17, // the extra s is not a typo
  BatteryVoltage = 18
};

/*
  BamocarInterface is meant solely for being able to quickly, easily
  change Bamocar parameters. Interfaces with CANbus using MessageMaster.
  
  NOTE: This class will not send CANbus messages for anything other than certain 
  parameter changes.

  NOTE 1: This class's get functionality does not pull data from the Bamocar. It
  pulls from values stored in the data structure.
*/
class BamocarInterface {
  private:
    MessageMaster *msgMaster;

    const uint8_t nRegsInUse = 18;
    bool transmissionEnabled; // representing whether the transmission enabling routine has been run
    uint8_t registerValues[18];
    float powerFactor;

  public:
    BamocarInterface(MessageMaster &msgMaster);
    void setupRegisters();


    void preChangeParameter();
    void initializeTransmission();
    void endTransmission();

    // void startupMotor();

    //-----------
    // void accelerationBaseRoutine();
    // void accelerationHigh();
    // void accelerationLow();
    // void accelerateToSpeed(uint8_t desiredSpeed);

    bool getTransmissionEnabled();

    void setTransmissionEnabled(bool setting);

    void setRegisterBase(uint8_t regID, uint16_t value);
    void setNominalMotorFrequency(uint16_t freq);
    void setMotorNominalVoltage(uint16_t voltage);
    void setDCTimePreMag(uint16_t time);
    void setDCVoltage(uint16_t percentage);
    void setPowerFactor(float PF);
    void setCurrentActual(uint16_t percentage); // to-do: find unit
    void setIMaxInUse(uint16_t amperes);
    void setCurrentActualI1(uint16_t amperes); // to-do: find unit
    void setCurrentActualI2(uint16_t amperes); // to-do: find unit
    void setCurrentActualI3(uint16_t amperes); // to-do: find unit
    void setMainsSupplyVoltage(uint16_t voltage);
    void setBatteryVoltage(uint16_t voltage);
};

#endif