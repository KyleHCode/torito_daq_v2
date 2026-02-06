#include "muxdriver.h"

bool mux_select(uint8_t bus_id, uint8_t channel) {
    uint8_t mask = (1 << channel);
    return i2c_write(bus_id, &mask, 1);
}