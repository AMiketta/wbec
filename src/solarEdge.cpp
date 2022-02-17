#include <SPI.h>
#include <globalConfig.h>
#include <Ethernet.h>       // Ethernet library v2 is required
#include <ModbusEthernet.h>
#include <logger.h>
#include <pvAlgo.h>


const byte METER_UID = 240;
const int MODBUS_CONNECT_ERROR = -10;
const int MODBUS_NO_RESPONSE = -11;

const byte FNC_READ_REGS = 0x03;
const byte FNC_WRITE_SINGLE = 0x06;
const byte FNC_ERR_FLAG = 0x80;

enum {
  MODBUS_DELAY,
  BATTERY_DATA,
  INVERTER_DATA,
  METER_DATA
};

//NetClient modbus;
uint8_t requestId = 0;
const uint8_t m  = 8;
const uint16_t REG = 40083;               // Modbus Hreg Offset
const uint16_t REG2 = 40084;  
IPAddress remote(192, 168, 178, 12);  // Address of Modbus Slave device
const int32_t showDelay = 5000;   // Show result every n'th mellisecond

// Enter a MAC address and IP address for your controller below.
byte mac[] = {
  0xEC, 0xFA, 0xBC, 0x58, 0xB8, 0xE0
};
IPAddress ip(192, 168, 178, 68); // The IP address will be dependent on your local network:
ModbusEthernet mb;               // Declare ModbusTCP instance

uint16_t res = 0;
uint32_t showLast = 0;
static uint32_t lastHandleCall       = 0;

void se_setup() {
Serial.begin(115200);     // Open serial communications and wait for port to open
  #if defined(AVR_LEONARDO)
  while (!Serial) {}        // wait for serial port to connect. Needed for Leonardo only
  #endif
  Ethernet.init(5);         // SS pin
  Ethernet.begin(mac, ip);  // start the Ethernet connection
  delay(1000);              // give the Ethernet shield a second to initialize
  mb.client();              // Act as Modbus TCP server
}

void se_loop() {
    if (mb.isConnected(remote)) {   // Check if connection to Modbus Slave is established
        mb.readHreg(remote, REG, &res);  // Initiate Read Hreg from Modbus Slave
        LOG(m, "se_loop: %d", res);
    } else {
        mb.connect(remote);           // Try to connect if not connected
    }
    delay(100);                     // Pulling interval
    mb.task();                      // Common local Modbus task
    if (millis() - showLast > showDelay) { // Display register value every 5 seconds (with default settings)
        showLast = millis();
      //  int inverterAC = regs[0] * pow(10, regs[1]); // ac power * scale
        Serial.println(res);
    }
}

void modbusClearData() {
//   pvChargingPower = 0;
//   pvSOC = 0;
//   meterPower = 0;
//   meterPowerPhaseA = 0;
//   meterPowerPhaseB = 0;
//   meterPowerPhaseC = 0;
//   inverterAC = 0;
//   voltage = 0;
}

boolean requestSymoRTC() {

//   short regs[2];
//   int res = modbusRequest(1, 40222, 2, regs);
//   if (modbusError(res))
//     return false;
//   // SunSpec has seconds from 1.1.2000 UTC, TimeLib uses 'epoch' (seconds from 1.1.1970)
//   setTime(SECS_YR_2000 + (unsigned short) regs[0] * 65536L + (unsigned short) regs[1] + SECS_PER_HOUR); // Europe DST
//   int m = month();
//   if (m > 10 || m < 3) {
//     setTime(now() - SECS_PER_HOUR);
//   } else if (m == 3) {
//     int d = 31 - ((((5 * year()) / 4) + 4) % 7);
//     if (day() < d) {
//       setTime(now() - SECS_PER_HOUR);
//     }
//   } else if (m == 10) {
//     int d = 31 - ((((5 * year()) / 4) + 1) % 7);
//     if (day() >= d) {
//       setTime(now() - SECS_PER_HOUR);
//     }
//   }
//   hourNow = hour();
// #ifdef ARDUINO_ARCH_SAMD
//   rtc.setEpoch(now());
// #endif
  return true;
}

boolean requestInverter() {
//   short regs[2];
//   int res = modbusRequest(1, 40083, 2, regs);
//   if (modbusError(res))
//     return false;
//   int inverterAC = regs[0] * pow(10, regs[1]); // ac power * scale
   return true;
}

boolean requestMeter() {
//   short regs[16];
//   int res = modbusRequest(METER_UID, 40076, 16, regs);
//   if (modbusError(res))
//     return false;
//   voltage = regs[3] * pow(10, regs[8]); // ac voltage F3 * scale
//   float sf = pow(10, regs[15]);
//   meterPower = -regs[11] * sf; // ac power * scale
//   meterPowerPhaseA = regs[12] * sf;
//   meterPowerPhaseB = regs[13] * sf;
//   meterPowerPhaseC = regs[14] * sf;
  return true;
}

boolean requestBattery() {
//   short regs[58];
//   int res = modbusRequest(1, 40257, 58, regs); //MPPT reg + SF offsset
//   if (modbusError(res))
//     return false;
//   short pvProduction = (unsigned short) regs[17] * pow(10, regs[0]); // mppt 1 dc power * scale
//   pvChargingPower = (unsigned short) regs[37] * pow(10, regs[0]); // dc power * scale
//   pvSOC = regs[54] / 100; // storage register addr - mppt register addr + ChaSta offset
//   pvBattCalib = false;
//   switch (regs[57]) {  // charge status
//     case 2:  // EMPTY
//         if (pvProduction < inverterAC) {
//           pvChargingPower = -pvChargingPower;
//         }
//       break;
//     case 7:  // TESTING (CALIBRATION)
//       pvBattCalib = true;
//       break;
//     case 3:  // DISCHARGING
//     case 5:  // FULL
//       pvChargingPower = -pvChargingPower;
//   }
  return true;
}
