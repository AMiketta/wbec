#include <SPI.h>
#include <globalConfig.h>
#include <Ethernet.h>       // Ethernet library v2 is required
#include <ModbusIP_ESP8266.h>
#include <logger.h>
#include <pvAlgo.h>

IPAddress remote(192, 168, 178, 68);  // Address of Modbus Slave device
ModbusIP mb;               // Declare ModbusTCP instance

bool isConnected = false;
uint16_t power_inverter = 0;
uint16_t power_inverter_scale = 0;
uint16_t power_meter = 0;
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
    mb.readHreg(remote, 40083, &power_inverter, 1, cb, 1);  //Power Inverter
    mb.readHreg(remote, 40084, &power_inverter_scale, 1, cb, 1); //Power Inverter Scale Factor
    mb.readHreg(remote, 40206, &power_meter, 1, cb, 1);  //Power Zähler
    mb.readHreg(remote, 40210, &power_meter_scale, 1, cb, 1); //Power Zähler Scale Factor
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
#ifdef DEBUG_SOLAREDGE
  Serial.print(s_power_inverter);
  Serial.print(" / ");
  Serial.print(s_power_inverter_scale);
  Serial.print(" + ");
  Serial.print(s_power_meter);
  Serial.print(" / ");
  Serial.println(s_power_meter_scale);
#endif
  if (s_power_inverter_scale < 0)   // wenn negativ dann geteilt
  {
    int16_t divisor;
    divisor = -s_power_inverter_scale;
    s_power_inverter = s_power_inverter / pow_int16(10, divisor);
  }
  else {                               // wenn positiv dann multipliziert
    s_power_inverter = s_power_inverter * pow_int16(10, s_power_inverter_scale);
  }
  if (s_power_meter_scale < 0)   // wenn negativ dann geteilt
  {
    int16_t divisor;
    divisor = -s_power_meter_scale;   //
    s_power_meter = s_power_meter / pow_int16(10, divisor);
  }
  else {                               // wenn positiv dann multipliziert
    s_power_meter = s_power_meter * pow_int16(10, s_power_meter_scale);
  }
  power_house = s_power_inverter + (1 * -s_power_meter);
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

  char c_haus_watt[5];
  char c_photovoltaik_watt[5];

  float prozent;
  float max_photovoltaik = 9000;

  dtostrf(s_power_inverter, 1, 0, c_photovoltaik_watt); // 1 Stelle hinter dem Komma
  dtostrf(power_house, 1, 0, c_haus_watt); // 1 Stelle hinter dem Komma

//   //Ausgabe auf Nextion
//   photo_w.setText(c_photovoltaik_watt);
//   haus_w.setText(c_haus_watt);

//   if (power_house > s_power_inverter) {
//     haus_w.Set_font_color_pco(63488); //rot
//   }
//   else {
//     haus_w.Set_font_color_pco(1024); //grün
//   }

//   //Berechnung der Auslastung der Anlage
//   prozent = s_power_inverter / (max_photovoltaik / 100);
//   //Ausgabe Prozentwert Display
//   n_prozent.setValue(prozent);

//   //Ausgabe der Schriftfarben Grün und Rot
//   if (s_power_inverter >= max_photovoltaik * 0.7) {
//     n_prozent.Set_font_color_pco(39043);    //rot
//   }
//   else {
//     n_prozent.Set_font_color_pco(30144);    //grün
//   }
}