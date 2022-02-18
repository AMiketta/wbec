#include <SPI.h>
#include <globalConfig.h>
#include <Ethernet.h>       // Ethernet library v2 is required
#include <ModbusIP_ESP8266.h>
#include <logger.h>
#include <pvAlgo.h>
#include <globalConfig.h>

IPAddress remote(192, 168, 178, 68);  // Address of Modbus Slave device
ModbusIP mb;               // Declare ModbusTCP instance

bool isConnected = false;
uint16_t ac_current = 0;
uint16_t power_inverter = 0;
uint16_t power_inverter_scale = 0;
uint16_t power_meter = 0; // // power (pos. = 'Einspeisung', neg. = 'Bezug')
uint16_t power_meter_scale = 0;
uint16_t power_house = 0; 
uint32_t showLast = 0;
int status = 0;
//Modbus::cbTransaction cb;
static uint32_t lastHandleCall       = 0;

bool cb(Modbus::ResultCode event, uint16_t transactionId, void *data)
{
  if (event != Modbus::EX_SUCCESS)
    Serial.printf("Modbus result: %02X\n", event);
  if (event == Modbus::EX_TIMEOUT)
  {
#ifdef DEBUG_SOLAREDGE
    Serial.println("Timeout");
#endif
  }
  return true;
}

int16_t pow_int16(int16_t base, uint16_t exp)
{
  int16_t x = 1;
  for (uint16_t i = 0; i < exp; i++) {
    x = x * base;
  }
  return x;
}

void se_setup() {
  delay(1000);              // give the Ethernet shield a second to initialize
  mb.client();              // Act as Modbus TCP server
}

void se_loop() {
    if ((millis() - lastHandleCall < (uint16_t)cfgPvCycleTime * 1000))      // avoid unnecessary frequent calls
    {
		return;
	}
	lastHandleCall = millis();

    isConnected = mb.isConnected(remote);
    if (isConnected) {   // Check if connection to Modbus Slave is established
    
        mb.readHreg(remote, REG_I_AC_Current, &ac_current, 1, cb, 1);  //Power Inverter
        mb.readHreg(remote, REG_I_AC_Power, &power_inverter, 1, cb, 1);  //Power Inverter
        mb.readHreg(remote, REG_I_AC_Power_SF, &power_inverter_scale, 1, cb, 1); //Power Inverter Scale Factor
        mb.readHreg(remote, REG_M_AC_Power, &power_meter, 1, cb, 1);  //Power Zähler
        mb.readHreg(remote, REG_M_AC_Power_SF, &power_meter_scale, 1, cb, 1); //Power Zähler Scale Factor
        if (status == 0) {
#ifdef DEBUG_SOLAREDGE
            Serial.println("Modbus verbunden");
#endif
            status = 1;
        }
     } else {  
        mb.connect(remote, 1502);           // Try to connect if no connection
        status = 0;
    }
    mb.task();                      // Common local Modbus task
    int16_t s_power_meter;
    int16_t s_power_inverter;
    int16_t s_power_inverter_scale;
    int16_t s_power_meter_scale;
    s_power_meter = (int16_t)  power_meter; // Umwandlung unsigned in signed
    s_power_inverter = (int16_t) power_inverter; // Umwandlung unsigned in signed
    s_power_inverter_scale = (int16_t) power_inverter_scale; // Umwandlung unsigned in signed
    s_power_meter_scale = (int16_t) power_meter_scale; // Umwandlung unsigned in signed

    if (s_power_inverter_scale < 0)   // wenn negativ dann geteilt
    {
        int16_t divisor;
        divisor = -s_power_inverter_scale;
        s_power_inverter = s_power_inverter / pow_int16(10, divisor);
    } else {                               // wenn positiv dann multipliziert
        s_power_inverter = s_power_inverter * pow_int16(10, s_power_inverter_scale);
    }
    if (s_power_meter_scale < 0)   // wenn negativ dann geteilt
    {
        int16_t divisor;
        divisor = -s_power_meter_scale;   //
        s_power_meter = s_power_meter / pow_int16(10, divisor);
    } else {                               // wenn positiv dann multipliziert
        s_power_meter = s_power_meter * pow_int16(10, s_power_meter_scale);
    }
    power_house = s_power_inverter + (1 * -s_power_meter);

    pv_setWatt((1 * -s_power_meter));
#ifdef DEBUG
    Serial.print("Leistung Umrichter: ");
    Serial.print(s_power_inverter);   //Ausgabe von signed Wert
    Serial.print(" Watt + Gesamt Haus: ");
    Serial.print(s_power_meter);
    Serial.print(" Watt");
    Serial.print(" + Strombezug Haus: ");
    Serial.print(power_house);
    Serial.println(" Watt");
#endif
}