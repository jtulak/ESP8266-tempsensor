ESP8266 tempsensor
==============

A 2-in-1 quick and dirty code for a "little smarter home". Allows for multiple DS18B20 sensors to be attached on GPIO 2 and read with HTTP request (with their IDs), and it can also control output on GPIO 16 (because my dev board for ESP-201 has a relay on this pin).

I made this for my own use as home temperature control. One ESP with a relay for controlling 24V heater input, and multiple ESPs as temperature sensors around the house. Because the firmware image is about only 200kB and can fit into every ESP, I saw it easier to have just a single firmware for all my ESPs and doesn't care what to load on each.

Build
-----
* Clone [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk)
* Build the sdk (and add it to your PATH)
* Clone this project
* Edit the ``Makefile`` (path of SDK, serial port with your ESP8266, ... Or you can specify these variables with ``make VAR=value``)
* Prepare your ESP to be flashed
* Run ``make && make flash``

Use
---
The ESP is listening on port 80 and you can connect either with a browser, or telnet. In both cases the use is straightforward.

**Browser** access shows all connected DS18B20 separated by new line when accessing index (``http://1.2.3.4/``). With ``/relay-on`` and ``/relay-off``, you can control the output of GPIO 16. There is no answer for relay control other that "HTTP/1.0 200 OK". Note that there is no HTML to parse at all.

**Telnet** allows three commands. ``relay-on``, ``relay-off`` and ``temperature``.

Current version is limited to 5 attached DS18B20. If you need more, change ``DS_MAX_PROBES`` in ``drivers/ds18b20.h``.

License
-------
My code is licensed under MIT, but some parts in ``driver/`` are under another licension.
