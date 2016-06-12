/*
 * This file reads all attached one wire temperature sensors,
 * and prints their GUID and current temperature in fahrenheit.
 * 
 * I'm using this device to monitor temperatures a 
 * fridge that is used for homebrewing beer.
 * 
 */

#include <OneWire.h>
#include <DallasTemperature.h>

#define DEVICE_PIN D4
#define READ_DELAY 5 * 1000

OneWire  ds(DEVICE_PIN);
DallasTemperature sensors(&ds);

typedef struct  {
  float tempf;
  DeviceAddress address;
} temperature;

void setup(void) {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  sensors.begin();
}

void loop(void) {  
  digitalWrite(LED_BUILTIN, LOW); // turn on LED
  
  temperature* temperatures = discoverOneWireDevices();
  logFoundDevices(temperatures);
  
  free(temperatures);

  digitalWrite(LED_BUILTIN, HIGH); // turn off LED
  delay(READ_DELAY);
}

temperature *discoverOneWireDevices(void) {
  ds.reset_search();
  sensors.requestTemperatures(); // Send the command to get temperatures

  int count = sensors.getDeviceCount();
  temperature* temperatures = (temperature*)malloc(sizeof(temperature) * count);
  
  for(int i = 0; i < count; i++) {
    temperature* t =  (temperature*)malloc(sizeof(temperature));
    sensors.getAddress(t->address, 0);
    t->tempf = sensors.getTempF(t->address);
    temperatures[i] = *t;
  }
  
  return temperatures;
}

void logFoundDevices(temperature* temperatures) {
  Serial.print("Devices: ");
  Serial.println(sizeof(temperatures) / sizeof(void*));

  for(int i = 0; i < sizeof(temperatures) / sizeof(void*); i++) {
    printAddress(temperatures[i].address);
    Serial.print(" - ");
    Serial.println(temperatures[i].tempf);
  }

  Serial.println("");
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}




