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

bool sensor_read_dispatch(const SensorDesc &sensor, int32_t &data) {
    switch (sensor.type) {
        case SensorType::PRESSURE:
            return PressureSensor().read(sensor, data);
        // Add cases for other sensor types here
        default:
            return false; // Unsupported sensor type
    }
}