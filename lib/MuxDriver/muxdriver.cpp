#include "muxdriver.h"
#include <primitives.h>

bool mux_select(uint8_t bus_id, uint8_t channel) {
    // bus_id parameter reserved for future multi-bus support
    // For now, always use default MUX_ADDRESS
    uint8_t mask = (1 << channel);
    return i2c_write(MUX_ADDRESS, &mask, 1);
}