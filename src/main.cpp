#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    // Wait for Serial to be ready (important for Teensy + USB)
    while (!Serial && millis() < 3000) {
        ; // wait up to 3 seconds
    }

    delay(2000);

    Serial.println("=================================");
    Serial.println("PlatformIO Serial Test");
    Serial.println("If you can read this, it works!");
    Serial.println("=================================");
}

void loop() {
    static uint32_t counter = 0;

    Serial.print("Hello from loop. Counter = ");
    Serial.println(counter++);

    delay(1000); // 1 second
}
