Formalize the three buffers and the transfers from the main to the two separate buffers.
Incorporate and write out sd card write and LoRa send modules. Also add health checks for both sd card and lora: loop time, SD stalls and LoRa queue depth.
Work on the LoRa code from daq to gsc.
Implement CRC (Cyclic redundancy check) for packet data corruption checking. Implement this across gsc to relay code and daq to gsc LoRa code.

If you decide to work on a module or add in functionality, please share a short plan/draft with the team describing the module's purpose and the exact features it adds.

Keep in mind we need additional safety features for packet integridy and fail-safes in case theres a failure during transmission.