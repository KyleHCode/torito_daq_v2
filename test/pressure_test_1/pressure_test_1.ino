#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// -------- USER SETTINGS --------
#define I2C_SDA D4      // CHANGE to your ESP32-S3 SDA pin
#define I2C_SCL D5      // CHANGE to your ESP32-S3 SCL pin
#define ADS_ADDR 0x48  // ADDR tied to GND
// --------------------------------

// Pressure sensor calibration (0–2000 psi, 0.5–4.5 V)
const float V_MIN = 0.5;      // Volts at 0 psi
const float V_MAX = 4.5;      // Volts at 2000 psi
const float PSI_MAX = 2000.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize ADS
  if (!ads.begin(ADS_ADDR)) {
    while (1);  // ADS not found
  }

  // IMPORTANT: needed for up to 4.5V input
  ads.setGain(GAIN_TWOTHIRDS);

  // Print header ONCE (Serial Plotter friendly)
  Serial.println("PSI");
}

void loop() {
  int16_t raw = ads.readADC_SingleEnded(0);

  // GAIN_TWOTHIRDS → 6.144V / 32768 = 0.0001875 V per count
  float voltage = raw * 0.0001875;

  // Convert voltage to PSI
  float psi = (voltage - V_MIN) * (PSI_MAX / (V_MAX - V_MIN));

  // Clamp to physical range
  if (psi < 0) psi = 0;
  if (psi > PSI_MAX) psi = PSI_MAX;

  // Serial Plotter reads this
  Serial.println(psi, 2);

  delay(50);  // ~20 Hz update rate
}
