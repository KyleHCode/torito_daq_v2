// src/main.cpp
#include <Arduino.h>
#include <Wire.h>
#include <daqloop.h>
#include <ringbuffer.h>
#include <sensordispatcher.h>

// Define the buffers
RingBuffer sd_buffer;
RingBuffer lora_buffer;

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("DAQ System Starting...");
    
    Wire.begin();
    
    // Initialize sensors
    if (!sensor_dispatcher_init()) {
        Serial.println("ERROR: Sensor init failed!");
        while(1) delay(1000);
    }
    
    daq_init();
    Serial.println("DAQ Ready!");
}

void loop() {
    static uint32_t next_daq = 0;
    
    // Run DAQ at 20Hz (50ms)
    if (millis() >= next_daq) {
        daq_step();
        next_daq += 50;
    }
    
    // Print frames from buffer (replaces SD logger for now)
    SampleFrame frame;
    while (sd_buffer.pop(&frame)) {
        Serial.print("Seq: "); Serial.print(frame.seq);
        Serial.print(" | Time: "); Serial.print(frame.timestamp_us);
        Serial.print(" | Valid: 0x"); Serial.print(frame.valid_mask, HEX);
        Serial.print(" | Status: 0x"); Serial.print(frame.status_bits, HEX);
        Serial.print(" | Data: ");
        for (int i = 0; i < 4; i++) {
            Serial.print(frame.payload[i]); Serial.print(" ");
        }
        Serial.println();
    }
}