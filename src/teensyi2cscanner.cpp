#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(200);
  Wire.begin();            // join I2C bus as master (scanner)
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int count = 0;
  Serial.println("Scanning I2C addresses...");
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Found device at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      count++;
      delay(10);
    }
  }
  if (count == 0) Serial.println("No devices found");
  else Serial.print("Done — devices found: "), Serial.println(count);
  delay(5000);
}