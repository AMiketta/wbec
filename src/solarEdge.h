#ifndef SOLAREDGE_H
#define SOLAREDGE_H

#include <Arduino.h>
#include <ModbusEthernet.h>

const char version[] = "build "  __DATE__ " " __TIME__;

#define FILE_NEW (O_READ | O_WRITE | O_CREAT)

extern bool isConnected;
extern uint16_t power_inverter;
extern uint16_t power_inverter_scale;
extern uint16_t power_meter;
extern uint16_t power_meter_scale;
extern uint16_t power_house;

extern void     se_setup();
extern void     se_loop();
bool cb(Modbus::ResultCode event, uint16_t transactionId, void *data);
int16_t pow_int16(int16_t base, uint16_t exp);

#endif /* SOLAREDGE_H */