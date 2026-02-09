#ifndef SAMPLEFRAME_H
#define SAMPLEFRAME_H

#include "sensorconfig.h"
#include <stdint.h>

struct SampleFrame {
    uint32_t timestamp;
    uint32_t sequence_number;
    uint8_t valid_mask;
    uint8_t status_bits;
    int32_t payload[MAX_SENSORS];
    int16_t raw_adc[MAX_SENSORS];
};

#endif // SAMPLEFRAME_H