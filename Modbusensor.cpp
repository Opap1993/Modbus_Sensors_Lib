#include "Modbusensor.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2, A3);

void Modbusensor::Modbusbgn() {
    mySerial.begin(9600);
    SerialUSB.begin(9600);
}

float Modbusensor::readModbusValue(uint8_t regAddr) {
    uint8_t data[8] = { 0x12, 0x03, 0x00, regAddr, 0x00, 0x01, 0x00, 0x00 };
    uint8_t recv[8] = {0};

    uint16_t crc = calculateCRC(data, 6);
    data[6] = crc & 0xFF;
    data[7] = crc >> 8;

    mySerial.write(data, 8);

    unsigned long start = millis();
    int count = 0;

    while (millis() - start < 1000) {
        if (mySerial.available()) {
            recv[count++] = mySerial.read();
            if (count >= 8) break;
        }
    }

    if (count < 7) return -1;  // Error: incomplete response

    if (calculateCRC(recv, 6) != ((recv[7] << 8) | recv[6])) return -1; // CRC check failed

    uint16_t val = (recv[3] << 8) | recv[4];

    return (regAddr == 0x02) ? val : (float)val / 100; // EC is not divided by 100
}

float Modbusensor::Getmodbustemp() {
    return readModbusValue(0x00); // Read temperature
}

float Modbusensor::Getmodbushum() {
    return readModbusValue(0x01); // Read humidity
}

float Modbusensor::Getmodbusec() {
    return readModbusValue(0x02); // Read EC
}

uint16_t Modbusensor::calculateCRC(uint8_t *data, uint8_t len) {
    uint16_t crc = 0xFFFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
