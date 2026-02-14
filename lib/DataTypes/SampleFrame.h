#ifndef SAMPLEFRAME_H
#define SAMPLEFRAME_H

#include <sensorconfig.h>
#include <stdint.h>

struct SampleFrame {
    uint32_t timestamp_us;
    uint32_t seq;
    uint8_t valid_mask;
    uint8_t status_bits;
    int32_t payload[SENSOR_COUNT];
    int16_t raw_adc[SENSOR_COUNT];
};

#endif // SAMPLEFRAME_H