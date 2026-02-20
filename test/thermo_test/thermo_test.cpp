#include <SPI.h>
#include <Adafruit_MAX31856.h>

// Create the MAX31856 object with the appropriate CS pin
Adafruit_MAX31856 max31856 = Adafruit_MAX31856(D1);  // Change 5 to your CS pin

void setup() {
  Serial.begin(115200);
  // Initialize the SPI bus and the MAX31856 module
  if (!max31856.begin()) {
    Serial.println("Could not find a valid MAX31856 sensor, check wiring!");
    while (1);
  }

  // Set the thermocouple type
  max31856.setThermocoupleType(MAX31856_TCTYPE_T);
  Serial.println("MAX31856 initialized with T-type thermocouple.");
}

void loop() {
  // Read the temperature
  double temperature = max31856.readThermocoupleTemperature();
  double offset = 3;

  double calibratedTemp = temperature - offset;
  
  // Print the temperature
  Serial.print("Temperature: ");
  Serial.print(calibratedTemp);
  Serial.println(" °C");

  delay(1000);  // Wait a second before the next reading
}
