#include "BamocarInterface.h"
#include "MessageMaster.h"

#include <FlexCAN_T4.h>
#include <Arduino.h>
#include "ChRt.h"

BamocarInterface::BamocarInterface(MessageMaster &msgMaster)
{
    this->msgMaster = &msgMaster;
    this->setTransmissionEnabled(false);
    this->setupRegisters();
}

void BamocarInterface::setupRegisters()
{
    for (uint8_t i=0; i<(this->nRegsInUse); i++) {
        this->registerValues[i] = 0;
    }
}

/* Run this method before every parameter change. Initializes the
 Bamocar's transmission if not previously enabled*/
void BamocarInterface::preChangeParameter()
{
    bool transmissionEnabled = this->getTransmissionEnabled();
    if (!transmissionEnabled) {
        this->initializeTransmission();
    }
}

/* This method runs the message routine to initialize the Bamocar's
 transmission*/
void BamocarInterface::initializeTransmission()
{
    this->msgMaster->transmissionEnableBTB();
    chThdYield();
    this->msgMaster->transmissionDisable();
    this->msgMaster->transmissionEnableHW();
    chThdYield();
    this->msgMaster->transmitNoDisable();

    this->setTransmissionEnabled(true);
}

/* This method solely disables the Bamocar's transmission*/
void BamocarInterface::endTransmission() {
    this->msgMaster->transmissionDisable();
}

/* Setter for internal flag representing whether the transmission
 enabling message routine has been run*/
void BamocarInterface::setTransmissionEnabled(bool setting)
{
    this->transmissionEnabled = setting;
}

/* Getter for internal flag representing whether the transmission
 enabling message routine has been run*/
bool BamocarInterface::getTransmissionEnabled()
{
    return this->transmissionEnabled;
}

/* Base Register Setter Code.
   Takes register ID and a 4-digit hex value.

   EXAMPLE: setRegisterBase(0x01, 0x1FE4)*/
void BamocarInterface::setRegisterBase(uint8_t regID, uint16_t value)
{
    CAN_message_t outMsg;
    outMsg.id = BamocarCanID::Receive; // this is INTENTIONALLY the receive ID. DON'T CHANGE.
    outMsg.len = this->msgMaster->bamocarReceiveLen;
    outMsg.buf[0] = regID;
    outMsg.buf[1] = value % 0x100;
    outMsg.buf[2] = value / 0x100;

    this->msgMaster->sendMsgBamocar(outMsg);
}

/* Sets Nominal Motor Frequency. Input frequency in Hz.*/
void BamocarInterface::setNominalMotorFrequency(uint16_t freq)
{
    this->preChangeParameter();
    this->setRegisterBase(RegIDs::NominalMotorFrequency, freq);
}

/* Sets Motor Voltage. Input Voltage in V.*/
void BamocarInterface::setMotorNominalVoltage(uint16_t voltage)
{
    this->preChangeParameter();
    this->setRegisterBase(RegIDs::MotorNominalVoltage, voltage);
}

/* Sets Pre-Magnetization Time.
    Input period in ms.*/
void BamocarInterface::setDCTimePreMag(uint16_t time)
{
    this->preChangeParameter();
    this->setRegisterBase(RegIDs::DCTimePreMag, time);
}

/* Sets DC Voltage for Pre-Magnetization.
    Input Voltage as % of max voltage. */
void BamocarInterface::setDCVoltage(uint16_t percentage)
{
    this->preChangeParameter();

    // convert percentage to bamocar format
    uint16_t converted = (percentage * 0x7FFF) / 100;
    this->setRegisterBase(RegIDs::DCVoltages, converted);
}

/* Sets Filter.
    Input value from 0-10.
    Low filter values: motor noise may occur
    High filter values: low-frequency vibrations may occur */
void BamocarInterface::setFilter(uint16_t filter)
{
    this->preChangeParameter();
    this->setRegisterBase(RegIDs::Filter, filter);
}

/* Sets Mains Supply Voltage. Input Voltage in V.
    AC and three-phase current voltage AC (30~to 480V~).
    Battery voltage or dc mains (12V= to 560V=). */
void BamocarInterface::setMainsSupplyVoltage(uint16_t voltage)
{
    this->preChangeParameter();
    this->setRegisterBase(RegIDs::MainsSupplyVoltage, voltage);
}

/* Sets Battery Supply Voltage. Input Voltage in V. */
void BamocarInterface::setBatteryVoltage(uint16_t voltage)
{
    this->preChangeParameter();
    this->setRegisterBase(RegIDs::BatteryVoltage, voltage);
}

/* Sets DC Voltage for Pre-Magnetization.
    Input Voltage as % of max voltage. */
void BamocarInterface::setPowerFactor(float PF)
{
    this->preChangeParameter();

    // convert percentage to bamocar format
    uint16_t converted = (PF * 0x7FFF);
    this->setRegisterBase(RegIDs::PowerFactor, converted);
}