esp-hello-world
========

This a simple "Hello World" application for the low cost WiFi SoC ESP8266.

How to build this project:

* Clone [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk)
* Build the sdk (and add it to your PATH)
* Clone this project
* Edit the ``Makefile`` (path of SDK, serial port with your ESP8266, ... Or you can specify these variables with ``make VAR=value``)
* Prepare your ESP to be flashed
* Run ``make && make flash``
