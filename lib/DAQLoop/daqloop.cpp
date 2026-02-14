#include "daqloop.h"
#include <ringbuffer.h>
#include <sensordispatcher.h>
#include <muxdriver.h>
#include <sensorconfig.h>
#include <Arduino.h>

// Global state
static uint32_t seq = 0;
static uint32_t tick = 0;

// External buffers (declared in main)
extern RingBuffer sd_buffer;
extern RingBuffer lora_buffer;

void daq_init() {
    seq = 0;
    tick = 0;
}

void daq_step() {
    SampleFrame frame;
    frame.seq = seq++;
    frame.timestamp_us = micros();
    frame.valid_mask = 0;
    frame.status_bits = 0;

    for (int i = 0; i < SENSOR_COUNT; i++) {
        const SensorDesc &desc = sensor_table[i];
        
        // Check if scheduled this frame
        if ((tick % desc.period_ticks) != 0) {
            continue;
        }

        // Select MUX channel if needed (DAQ owns bus control)
        if (desc.mux_channel != NO_MUX) {
            if (!mux_select(desc.bus_id, desc.mux_channel)) {
                frame.status_bits |= MUX_ERR;
                continue;
            }
        }

        // Read sensor (sensor just reads I2C, doesn't touch MUX)
        int32_t processed_value;
        int16_t raw_adc;
        if (sensor_read_dispatch(desc, processed_value, raw_adc)) {
            frame.payload[desc.id] = processed_value;
            frame.raw_adc[desc.id] = raw_adc;
            frame.valid_mask |= (1 << desc.id);
        } else {
            frame.status_bits |= I2C_ERR;
        }
    }

    // Push to SD buffer (every frame)
    if (!sd_buffer.push(&frame)) {
        frame.status_bits |= OVERRUN;
    }

    // Push to LoRa buffer (every 9th frame)
    static uint32_t lora_counter = 0;
    if ((lora_counter++ % 9) == 0) {
        lora_buffer.push(&frame);
    }

    tick++;
}
