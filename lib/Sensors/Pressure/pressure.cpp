// pressure.cpp
#include "pressure.h"
#include <MuxDriver.h>

// Define static constants
const float PressureSensor::V_MIN = 0.5f;
const float PressureSensor::V_MAX = 4.5f;
const float PressureSensor::PSI_MAX = 2000.0f;

bool PressureSensor::init() {
    if (!ads.begin(0x48)) {
        return false;
    }
    ads.setGain(GAIN_TWOTHIRDS);
    return true;
}

bool PressureSensor::read(const SensorDesc &sensor, int32_t &data) {
    if (!mux_select(sensor.bus_id, sensor.mux_channel)) {
        return false;
    }
    
    int16_t raw = ads.readADC_SingleEnded(sensor.adc_channel);
    last_raw_adc = raw;
    
    float voltage = ads.computeVolts(raw);
    float psi = (voltage - V_MIN) * (PSI_MAX / (V_MAX - V_MIN));
    if (psi < 0.0f) psi = 0.0f;
    if (psi > PSI_MAX) psi = PSI_MAX;
    
    data = (int32_t)(psi * 100.0f);
    return true;
}