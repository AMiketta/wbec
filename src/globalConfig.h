// Copyright (c) 2021 steff393, MIT license

#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#define WB_CNT             16   // max. possible number of wallboxes in the system (NodeMCU has Bus-ID = 0)
#define OPENWB_MAX_LP       8   // maximum supported loadpoints by openWB
#define REG_WD_TIME_OUT   257   // modbus register for "ModBus-Master Watchdog Timeout in ms"
#define REG_STANDBY_CTRL  258   // modbus register for "Standby Function Control"
#define REG_REMOTE_LOCK   259   // modbus register for "Remote lock (only if extern lock unlocked)"
#define REG_CURR_LIMIT    261   // modbus register for "Maximal current command"
#define REG_CURR_LIMIT_FS 262   // modbus register for "FailSafe Current configuration (in case loss of Modbus communication)"

#define CURR_ABS_MIN       60   // absolute possible lower limit for current
#define CURR_ABS_MAX      160   // absolute possible upper limit for current

#define PIN_DI              5   // GPIO5, NodeMCU pin D1
#define PIN_RO              2   // GPIO2, NodeMCU pin D4
#define PIN_DE_RE           4   // GPIO4, NodeMCU pin D2
#define PIN_RST             0   // GPIO0, NodeMCU pin D3 
#define PIN_SS             15   // GPIO15,NodeMCU pin D8

// SolarEdge (sunspec) registers for modbusTCP
#define REG_I_AC_Current 40071 // 40072 1 I_AC_Current uint16 Amps AC Total Current value
#define REG_I_AC_Power 40083 //modbus register for "AC Power value", int16 in Watts 
#define REG_I_AC_Power_SF 40084 // modbus register for "AC Power scale factor" int16
#define REG_M_AC_Power 40206 // modbus  register for "Total Real Power (sum of active phases)" int16 in Watts
#define REG_M_AC_Power_SF 40210 // modbus register for "AC Real Power Scale Factor" int16 SF

extern char     cfgWbecVersion[];	           // wbec version
extern char     cfgBuildDate[];	          	 // wbec build date

extern char     cfgApSsid[32];               // SSID of the initial Access Point
extern char     cfgApPass[63];               // Password of the initial Access Point
extern uint8_t  cfgCntWb;                    // number of connected wallboxes in the system
extern uint8_t  cfgMbCycleTime;              // cycle time of the modbus (in seconds)
extern uint16_t cfgMbDelay;                  // delay time of the modbus before sending new message (in milliseconds)
extern uint16_t cfgMbTimeout;                // Reg. 257: Modbus timeout (in milliseconds)
extern uint16_t cfgStandby;                  // Reg. 258: Standby Function Control: 0 = enable standby, 4 = disable standby
extern char     cfgMqttIp[16];               // IP address of MQTT broker, "" to disable MQTT
extern uint16_t cfgMqttPort;                 // Port of MQTT broker (optional)
extern char     cfgMqttUser[32];             // MQTT: Username
extern char     cfgMqttPass[32];             // MQTT: Password
extern uint8_t  cfgMqttLp[WB_CNT];           // Array with assignments to openWB loadpoints, e.g. [4,2,0,1]: Box0 = LP4, Box1 = LP2, Box2 = no MQTT, Box3 = LP1
extern char     cfgNtpServer[30];            // NTP server
extern char     cfgFoxUser[32];              // powerfox: Username
extern char     cfgFoxPass[16];              // powerfox: Password
extern char     cfgFoxDevId[16];             // powerfox: DeviceId
extern uint8_t  cfgPvActive;                 // PV charging: Active (1) or inactive (0)
extern uint8_t  cfgPvCycleTime;              // PV charging: cycle time (in seconds)
extern uint8_t  cfgSolarEdgeCycleTime;      // SolardEdge: cycle time (in seconds)
extern uint8_t  cfgPvLimStart;               // PV charging: Target current needed for starting (in 0.1A), e.g. 61=6.1A
extern uint8_t  cfgPvLimStop;                // PV charging: Target current to stop charging when below (in 0.1A)
extern uint8_t  cfgPvPhFactor;               // PV charging: Power/Current factor, e.g. 69: 1A equals 690W at 3phases, 23: 1A equals 230W at 1phase
extern uint16_t cfgPvOffset;                 // PV charging: Offset for the available power calculation (in W); can be used to assure that no/less current is consumed from net
extern uint16_t cfgTotalCurrMax;             // <don't use - still beta> Total current limit for load management (in 0.1A)
extern uint8_t  cfgHwVersion;                // Selection of the used HW
extern uint8_t  cfgWifiSleepMode;            // Set sleep type for power saving, recomendation is 255 (=no influence) or 0 (=WIFI_NONE_SLEEP)
extern char     cfgSolarEdgeIp[16];          // IP address of SolarEdge inverter, "" to disable 

extern void loadConfig();

#endif	/* GLOBALCONFIG_H */
