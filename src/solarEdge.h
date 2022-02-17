#ifndef SOLAREDGE_H
#define SOLAREDGE_H

#include <Arduino.h>


typedef enum {
	 SINGLE_PHASE_INVERTER = 101,
    SPLIT_PHASE_INVERTER = 102,
    THREE_PHASE_INVERTER = 103,
    SINGLE_PHASE_METER = 201,
    SPLIT_PHASE_METER = 202,
    WYE_THREE_PHASE_METER = 203,
    DELTA_THREE_PHASE_METER = 204,
    BATTERY_BASE = 802,
    LI_BATTERY_BANK = 803,
    LI_BATTERY_STRING = 804,
    LI_BATTERY_MODULE = 805,
    FLOW_BATTERY = 806,
    FLOW_BATTERY_STRING = 807,
    FLOW_BATTERY_MODULE = 808,
    FLOW_BATTERY_STACK = 809
} sunspecDID;

typedef enum {
	I_STATUS_OFF = 1,
    I_STATUS_SLEEPING = 2,
    I_STATUS_STARTING = 3,
    I_STATUS_MPPT = 4,
    I_STATUS_THROTTLED = 5,
    I_STATUS_SHUTTING_DOWN = 6,
    I_STATUS_FAULT = 7,
    I_STATUS_STANDBY = 8
} inverterStatus;

typedef enum {
	B_STATUS_OFF = 1,
    B_STATUS_EMPTY = 2,
    B_STATUS_DISCHARGING = 3,
    B_STATUS_CHARGING = 4,
    B_STATUS_FULL = 5,
    B_STATUS_HOLDING = 6,
    B_STATUS_TESTING = 7
} batteryStatus;

typedef enum {
    RTU = 1,
    TCP = 2
} connectionType;

typedef enum {
    INPUT_Type = 1,
    HOLDING = 2
} registerType;

typedef enum {
    UINT16 = 1,
    UINT32 = 2,
    UINT64 = 3,
    INT16 = 4,
    SCALE = 4,
    ACC32 = 5,
    FLOAT32 = 6,
    SEFLOAT = 7,
    STRING = 9
} registerDataType;

const char version[] = "build "  __DATE__ " " __TIME__;

#define FILE_NEW (O_READ | O_WRITE | O_CREAT)

const byte TONE_PIN = 2;
#ifdef ARDUINO_SAMD_ZERO
const byte MAIN_RELAY_PIN = 3;
const byte SD_SS_PIN = 4;  // Ethernet shield
const byte ZC_EI_PIN = 5;  // on one Grove connector with TRIAC_PIN
const byte TRIAC_PIN = 6;  // TCC0 WO pin for TriacLib
const byte NET_SS_PIN = 10; // Ethernet shield
// const byte PUMP_RELAY_PIN = ??;
#elif defined(PROBADIO)
const byte ZC_EI_PIN = 3; // INT1 pin. on one Grove connector with TRIAC_PIN
const byte TRIAC_PIN = 4; // TIMER1 OC1B
const byte MAIN_RELAY_PIN = 5;
const byte PUMP_RELAY_PIN = 6;
const byte SD_SS_PIN = 10;  // Adafruit SD card adapter directly on pins 10 to 13
//pin 10-13 SPI (Ethernet, SD)
const byte NET_SS_PIN = A5; // is close to SPI header
#endif
const byte BYPASS_RELAY_PIN = 7;
const byte LEDBAR_DATA_PIN = 8;
const byte LEDBAR_CLOCK_PIN = LEDBAR_DATA_PIN + 1; //on one Grove connector
const byte BUTTON_PIN = SCL; // Grove Base shield I2C connector

const byte STATUS_LED_PIN = 99; // status led not used

const int MAX_POWER = 2050;
const int BYPASS_POWER = 2050;
const unsigned long PUMP_STOP_MILLIS = 10 * 60000; // 10 min
const unsigned long VALVE_ROTATION_TIME = 30000; // 30 sec

const int EXT_HEATER_POWER = 1000;

const IPAddress symoAddress(192,168,1,7);

enum struct RegulatorState {
  REST = 'N', // night
  ALARM = 'A',
  MONITORING = 'M', // SoC < xx%
  REGULATING = 'R', // PowerPilot.ino
  OVERHEATED = 'O', // ElSesns.ino heater safety thermostat triggered
  MANUAL_RUN = 'H', // ManualRun.ino - H as 'by hand'
  OPENING_VALVES = 'V'
};

enum struct AlarmCause {
  NOT_IN_ALARM = '-',
  NETWORK = 'N',
  MODBUS = 'M',
  PUMP = 'P'
};

// events array indexes (and size)
enum {
  EVENTS_SAVE_EVENT,
  RESTART_EVENT,
  WATCHDOG_EVENT,
  NETWORK_EVENT,
  PUMP_EVENT,
  MODBUS_EVENT,
  OVERHEATED_EVENT,
  BALBOA_PAUSE_EVENT,
  MANUAL_RUN_EVENT,
  VALVES_BACK_EVENT,
  SUSPEND_CALIBRATION_EVENT,
  BATTSETT_LIMIT_EVENT,
  POWERPILOT_PLAN_EVENT,
  EXT_HEATER_EVENT,
  STATS_SAVE_EVENT,
  EVENTS_SIZE
};

enum {
  BATTERY_PRIORITY,
  HEATING_PRIORITY_AM,
  HEATING_DISABLED_AM,
  HEATING_DISABLED
};

enum {
  EXT_HEATER_DISABLED,
  EXT_HEATER_NORMAL
};

struct Stats {
  long heatingTime = 0; // minutes
  long consumedPower = 0; // watts
};

extern void     se_setup();
extern void     se_loop();
extern void modbusClearData();
extern boolean requestSymoRTC();
extern boolean requestInverter();
extern boolean requestMeter();
extern boolean requestBattery();
extern boolean modbusError(int err);
extern int modbusRequest(byte uid, unsigned int addr, byte len, short *regs);
extern int modbusWriteSingle(unsigned int address, int val);
extern int modbusConnection();
//extern int32_t  pv_getWatt();
//extern void     pv_setWatt(int32_t val);
//extern pvMode_t pv_getMode();
//extern void     pv_setMode(pvMode_t val);

#endif /* SOLAREDGE_H */