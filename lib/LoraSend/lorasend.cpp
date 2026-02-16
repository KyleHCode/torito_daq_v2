#include "lorasend.h"

bool LoraSend::init(LoraModule *lora_mod, RingBuffer *rb, uint8_t dest_address_) {
    if (!lora_mod || !rb) {
        return false;
    }
    lora_module = lora_mod;
    ring_buffer = rb;
    dest_address = dest_address_;
    return true;
}

bool LoraSend::send_next() {
    if (!ring_buffer || !lora_module) return false;

    SampleFrame frame;
    if (!ring_buffer->pop(&frame)) {
        // nothing to send
        return false;
    }

    uint8_t buf[MAX_SERIALIZED_HEADER];
    size_t len = serialize_frame_header(frame, buf, sizeof(buf));
    if (len == 0) return false; // serialization failure

    // Send packed header as one LoRa frame (hex-encoded internally)
    return lora_module->send_data_hexstr(dest_address, buf, len);
}

size_t LoraSend::send_all() {
    if (!ring_buffer || !lora_module) return 0;
    size_t sent = 0;
    SampleFrame frame;
    while (ring_buffer->pop(&frame)) {
        uint8_t buf[MAX_SERIALIZED_HEADER];
        size_t len = serialize_frame_header(frame, buf, sizeof(buf));
        if (len == 0) break;
        if (lora_module->send_data_hexstr(dest_address, buf, len)) {
            ++sent;
        } else {
            // stop on failure to avoid spinning on a bad link
            break;
        }
    }
    return sent;
}

// Pack header fields tightly (no compiler padding) in little-endian order.
size_t LoraSend::serialize_frame_header(const SampleFrame &frame, uint8_t *out, size_t out_len) {
    // required size: timestamp(4) + seq(4) + valid_mask(1) + status_bits(1) +
    // raw_adc[SENSOR_COUNT] (2 bytes each)
    const size_t required = 4 + 4 + 1 + 1 + (sizeof(frame.raw_adc));
    if (out_len < required || required > MAX_SERIALIZED_HEADER) return 0;

    size_t pos = 0;
    // 32-bit little-endian
    out[pos++] = (uint8_t)(frame.timestamp_us & 0xFF);
    out[pos++] = (uint8_t)((frame.timestamp_us >> 8) & 0xFF);
    out[pos++] = (uint8_t)((frame.timestamp_us >> 16) & 0xFF);
    out[pos++] = (uint8_t)((frame.timestamp_us >> 24) & 0xFF);

    out[pos++] = (uint8_t)(frame.seq & 0xFF);
    out[pos++] = (uint8_t)((frame.seq >> 8) & 0xFF);
    out[pos++] = (uint8_t)((frame.seq >> 16) & 0xFF);
    out[pos++] = (uint8_t)((frame.seq >> 24) & 0xFF);

    out[pos++] = frame.valid_mask;
    out[pos++] = frame.status_bits;

    // raw_adc[] as little-endian uint16_t
    for (size_t i = 0; i < SENSOR_COUNT; ++i) {
        uint16_t v = frame.raw_adc[i];
        out[pos++] = (uint8_t)(v & 0xFF);
        out[pos++] = (uint8_t)((v >> 8) & 0xFF);
    }

    return pos;
}