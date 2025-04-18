# dragonSlayer

**Description**: a way to integrate RGB led strip driver into Home Assistant via webhooks.

## Hardware
This project aim to provide a way to control an ESP32-C3 RGB driver via web comands.
Since the specific board is not currently well supported by ESP Home, we need another way
to control the device. I used an [Elcectrodragon](https://www.electrodragon.com/product/esp-led-strip-board/) board.
It's a cheap solution for controlling a RGB led strip but unfortuanly it's not currently well supported
in ESP Home.

## Installation  
### Step1
Flash the ino file to the board using Arduino ide. The settings are:
- Board: ESP32C3 Dev board
- CPU frequency: 160MHz (WiFi)
- Flash frequency: 80MHz
- Flash Mode: "QIO"
- Flash sizre: "4MB (32Mb)"
- Partition Scheme: "Default 4MB with spiffs (1.2MB APP/1.25MB SPIFFS)2
- Upload Speed: "921600"
### Step 2
Integrate the content of configuration.yaml into your configuration file
(the one found in the root of your Home Assistant). 

## Features  
The folowing comands are integrated
- /red: turns on the red leds at max brigthness  
- /green: turns on the green leds at max brigthness
- /blue: turns on the blue leds at max brigthness
- /off: turns off the strip
- /state: returns a json file with the state of the 3 strips