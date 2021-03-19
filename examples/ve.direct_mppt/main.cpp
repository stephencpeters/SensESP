#include <Arduino.h>
#include <SoftwareSerial.h>  // Need software serial for chargers 3-6
#include "sensesp_app.h"
#include "vedirect_helper.h"  // Need the companion helper app
#include "sensesp_app_builder.h"

ReactESP app([]() {
  SensESPAppBuilder builder;
  sensesp_app = builder.set_standard_sensors(ALL) //or, IP_ADDRESS, UPTIME, NONE
                    ->get_app();

#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  // Software serial port is used for receiving data on ESP8266
  // ESP8266 pins are specified as DX
  // ESP32 pins are specified as just the X in GPIOX

#ifdef ESP8266
  uint8_t pin = D4;
  SoftwareSerial* serial = new SoftwareSerial(pin, -1, false);
  serial->begin(19200, SWSERIAL_8N1);
#elif defined(ESP32)

//  Assign pins for ESP32 serial communication.
//  The first 2 are connected to hardware UARTs
//  The next 4 are software UARTs.
//  Unknown if more could be defined.
//  The default pins are 27, 16, 26, 17, 25, 33
//
//  Define hardware UARTs
//  Use these first if you have up to two ve.direct MPPTs
//  These are labeled 1, 2, 3 in the Signal K path names

  uint8_t pinA = 27;   // Reassign UART1 to pin 27
  HardwareSerial* serialA = &Serial1;
  serialA->begin(19200, SERIAL_8N1, pinA, -1);

  uint8_t pinB = 16;   //  Keep UART2 on default pin 16
  HardwareSerial* serialB = &Serial2;
  serialB->begin(19200, SERIAL_8N1, pinB, -1);

  //Note that this UART defaults to the USB chip on many dev kits
  uint8_t pinC = 26;  // Reassign UART0 to pin 26
  HardwareSerial* serialC = &Serial;
  serialC->begin(19200, SERIAL_8N1, pinC, -1);
 
//  Define the rest of the serial ports in software
//  These are labeled 4, 5, 6 in the Signal K path names

  uint8_t pinD = 17;  // Use software serial on pin 17
  SoftwareSerial* serialD = new SoftwareSerial(pinD, -1, false);
  serialD->begin(19200, SWSERIAL_8N1);

  uint8_t pinE = 25;  // Use software serial on pin 25
  SoftwareSerial* serialE = new SoftwareSerial(pinE, -1, false);
  serialE->begin(19200, SWSERIAL_8N1);

  uint8_t pinF = 33;  // Use software serial on pin 33
  SoftwareSerial* serialF = new SoftwareSerial(pinF, -1, false);
  serialF->begin(19200, SWSERIAL_8N1);

#endif

// Instantiate each MPPT charger
// auto* charger1 = setup_vedirect(serialA, 1);
// The serial[A..F] refers to the serial ports/pins
// The number is concatenated to the Signal K path.
// The variables are not used elsewhere, so you will
// likely get a warning.  


  auto* charger1 = setup_vedirect(serialA, 1);
   // String("Davits").value(
  //   new SKOutputString("electrical.solar.charger1.name", "/ve.direct.1/chargerNamePath"));

  auto* charger2 = setup_vedirect(serialB, 2);

  auto* charger3 = setup_vedirect(serialC, 3);

  auto* charger4 = setup_vedirect(serialD, 4);

  auto* charger5 = setup_vedirect(serialE, 5);

  auto* charger6 = setup_vedirect(serialF, 6);

  sensesp_app->enable();
});
