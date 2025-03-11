#ifndef MODBUSENSOR_H
#define MODBUSENSOR_H

#include "Arduino.h"
#include "SoftwareSerial.h"

class Modbusensor {
public:
    void Modbusbgn();         // Initializes Modbus sensor communication
    float Getmodbustemp();    // Reads temperature from the sensor
    float Getmodbushum();     // Reads humidity from the sensor
    float Getmodbusec();      // Reads electrical conductivity (EC) from the sensor

private:
    float readModbusValue(uint8_t regAddr); // Helper function for reading Modbus data
    uint16_t calculateCRC(uint8_t *data, uint8_t len); // CRC calculation
};

#endif
