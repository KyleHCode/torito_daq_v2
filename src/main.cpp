// src/main.cpp
#include <Arduino.h>
#include <Wire.h>
#include <hwconfig.h>
#include <daqloop.h>
#include <ringbuffer.h>
#include <sensordispatcher.h>
#include <muxdriver.h>
#include <sdwrite.h>
#include <dispatcherthread.h>
#include <loramodule.h>
#include <lorasend.h>
#include <lora_config.h>

// Define the buffers
RingBuffer daq_buffer;
RingBuffer sd_buffer;
RingBuffer lora_buffer;

// SD writer instance (drains `sd_buffer` and performs block writes)
SDWrite sdwriter;

// LoRa module + sender
LoraModule lora_module(0, 0, LORA_SENDER_ADDRESS); // pins ignored on Teensy
LoraSend lora_sender;
void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("DAQ System Starting...");
    
    // Initialize I2C (Teensy 4.1 uses default pins SDA=18, SCL=19)
    Wire.begin();
    
    // Initialize mux hardware
    if (!mux_init()) {
        Serial.println("ERROR: Mux init failed!");
        while(1) delay(1000);
    }
    
    // Select mux channel for sensor initialization
    // (All sensors are on channel 0 per sensorconfig.h)
    if (!mux_select(0, 0)) {
        Serial.println("ERROR: Mux channel select failed!");
        while(1) delay(1000);
    }
    delay(5);  // Allow mux to stabilize
    
    // Initialize sensors
    if (!sensor_dispatcher_init()) {
        Serial.println("ERROR: Sensor init failed!");
        while(1) delay(1000);
    }
    
    daq_init();

    // Initialize SD writer (attach the sd_buffer the dispatcher pushes into)
    if (!sdwriter.init(&sd_buffer, "data.bin")) {
        Serial.println("ERROR: SD writer init failed — continuing without SD logging");
    }

    // Initialize LoRa module and sender
    if (!lora_module.begin()) {
        Serial.println("WARNING: LoRa module did not respond during init");
    } else {
        lora_module.configure(LORA_SENDER_ADDRESS, LORA_BAND, LORA_NETWORK_ID);
        lora_sender.init(&lora_module, &lora_buffer, LORA_RECEIVER_ADDRESS);
    }

    Serial.println("DAQ Ready!");
    Serial.println("Seq,S0,S1,S2,S3");
}

void loop() {
    static uint32_t next_daq = 0;
    
    // Run DAQ at 20Hz (50ms)
    if (millis() >= next_daq) {
        daq_step();
        next_daq += 50;
    }
    if (!dispatcher_thread_step()) {
        Serial.println("ERROR: Dispatcher thread step failed! Overflow detetcted!");
    }

    // Drive SD writer (drains `sd_buffer` and writes in blocks).
    // NOTE: `sd_buffer` is now owned/consumed by the SD writer —
    // don't pop it elsewhere (use a debug buffer or peek API for prints).
    sdwriter.data();

    // Attempt to send one pending LoRa frame each loop (if available)
    lora_sender.send_next();
}