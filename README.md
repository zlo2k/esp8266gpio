# esp8266gpio

Universal GPIO control.

## Main features:

* Easy enable control to all aviable GPIO
* Easy configure WiFi
* WEB update
* Light integrate Web GUI


![Like this](https://github.com/zlo2k/esp8266gpio/blob/master/gpio.jpg?raw=true)

## Requirements

Any **esp8266** board.
* FOR WEB update required minimum 1mb (8Mbit) board 
Arduino 1.6.8 or earlier.

Сonfigured for use esp8266 board [Arduino esp8266](https://github.com/esp8266/Arduino)

Install Sketch data upload [How to install](http://arduino.esp8266.com/versions/1.6.5-1160-gef26c5f/doc/reference.html#file-system)

## Instalation

First upload data to spifs. (data folder - 2 files)
Configure use GPIO in sketch.

*Edit part marked  // ---  CONFIG --- //

```C++
int gpio[] = {0, 2};   // GPIO list to control
int gpiostat[] = {1, 0};  // Default status of GPIO on start - 0 disabled, 1 - enabled
int gpio_count = 2;  // total count of GPIO used
```
Upload sketch to esp8266.

Now open in browser IP you'r esp (If esp in AP mode, this 192.168.4.1 else any ip from you'r network) and controll pressing buttons OR you can use direct URL request to control like this:

###http://IP_ESP/gpio?gpio={index gpio from config}&status={on|off}



