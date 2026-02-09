#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// ---------------- USER SETTINGS ----------------
#define TCA_ADDR   0x70   // TCA9548A default (A0/A1/A2 = GND)
#define ADS_ADDR   0x48   // ADS1115 (ADDR = GND)
#define MUX_CH     4      // SC4/SD4 corresponds to channel 4
// ------------------------------------------------

// Pressure sensor calibration (0–2000 psi, 0.5–4.5 V)
static const float V_MIN   = 0.5f;
static const float V_MAX   = 4.5f;
static const float PSI_MAX = 2000.0f;

Adafruit_ADS1115 ads;

static bool i2cPing(uint8_t addr) {
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}

static bool tcaSelect(uint8_t channel) {
  if (channel > 7) return false;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  return (Wire.endTransmission() == 0);
}

static float voltsToPsi(float v) {
  float psi = (v - V_MIN) * (PSI_MAX / (V_MAX - V_MIN));
  if (psi < 0.0f) psi = 0.0f;
  if (psi > PSI_MAX) psi = PSI_MAX;
  return psi;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Teensy 4.1 default I2C pins:
  // SDA = 18, SCL = 19
  Wire.begin();

  // 1) Check mux exists on main I2C bus
  if (!i2cPing(TCA_ADDR)) {
    Serial.println("ERROR: TCA9548A not found at 0x70. Check SDA(18)/SCL(19), power, ground, address pins.");
    while (1) delay(1000);
  }

  // 2) Select channel 4 (SC4/SD4)
  if (!tcaSelect(MUX_CH)) {
    Serial.println("ERROR: Failed to select TCA channel 4.");
    while (1) delay(1000);
  }

  // 3) Check ADS exists behind the mux channel
  if (!i2cPing(ADS_ADDR)) {
    Serial.println("ERROR: ADS1115 not found on TCA channel 4. Check ADS power/ground/ADDR pin and mux routing.");
    while (1) delay(1000);
  }

  // 4) Init ADS + gain for up to ~4.5V
  if (!ads.begin(ADS_ADDR)) {
    Serial.println("ERROR: ads.begin() failed.");
    while (1) delay(1000);
  }
  ads.setGain(GAIN_TWOTHIRDS); // supports higher input range for your 0.5–4.5V sensors

  // Header for plotting 4 traces (tab-separated)
  Serial.println("P0\tP1\tP2\tP3");
}

void loop() {
  // Always re-select channel before talking to ADS (good habit)
  tcaSelect(MUX_CH);

  float psi[4];

  for (int ch = 0; ch < 4; ch++) {
    int16_t raw = ads.readADC_SingleEnded(ch);
    // Adafruit helper uses the currently-set gain to compute volts safely
    float voltage = ads.computeVolts(raw);
    psi[ch] = voltsToPsi(voltage);
  }

  // One line, 4 columns => plot 4 lines
  Serial.print(psi[0], 2); Serial.print('\t');
  Serial.print(psi[1], 2); Serial.print('\t');
  Serial.print(psi[2], 2); Serial.print('\t');
  Serial.println(psi[3], 2);

  delay(50); // ~20 Hz update rate
}
