# mp3_shine_esp32
Shine MP3 encoder for ESP32

This is a 'port' of the old Shine MP3 encoder originally written for ARM or MIPS arch devices, quote a long time ago.

Memory allocation has been optimised for the ESP32
Some asm has been added to boost performance in the ESP32 arch

The are leftovers of the beggings of dual core implementation but was not required in the end.

You should init the encoder ASAP in your code as the encoder need large contiguous chunks of RAM

A sample FreeRTOS task has been included as a guide. Change it to suit your own needs.

For full 48000Hz Joint Stereo performance on one core your will need to compile with the -Os optimisation option.

I will try to get an HTTP MP3 streaming demo up at some stage.

Any questions feel free to ask.

Cheers!
-fkn
