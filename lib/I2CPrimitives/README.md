=========
i2c_write
=========
Mainly being used for the mux to select a channel and switch/listen to it

=========================
i2c_read / i2c_write_read
=========================

Going to keep it a buck 50 with you reader. Don't use these, just use the adafruit libraries and Wire. 
Originally made to directly control the I2C writes but I switched to using the libraries instead in the other function calls for simplicity. 
Im fairly sure those libraries will handle I2C writes better than any code I'd write.