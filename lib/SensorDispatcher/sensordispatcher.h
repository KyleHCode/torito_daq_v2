#ifndef SENSOR_DISPATCHER_H
#define SENSOR_DISPATCHER_H

#include <SensorDesc.h>

bool sensor_read_dispatch(const SensorDesc &sensor, int32_t &data);

#endif // SENSOR_DISPATCHER_H