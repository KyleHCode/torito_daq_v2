#include "sensordispatcher.h"
#include <Sensors/Pressure/pressure.h>

// Sesnor class instances
PressureSensor g_pressure_sensor;

bool sensor_dispatcher_init() {
    if (!g_pressure_sensor.init()) {
        return false;
    }
    // Initialize other sensor types here
    return true;
}

bool sensor_read_dispatch(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc) {
    switch (sensor.sensorType) {
        case SensorDesc::type::PRESSURE:
            return g_pressure_sensor.read(sensor, data, raw_adc);
        default:
            return false;
    }
}