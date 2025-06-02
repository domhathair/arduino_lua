# Arduino Lua Interpreter

A basic example of an embedded Lua interpreter within the Arduino framework

## Overview

This project demonstrates how to integrate a Lua interpreter with Arduino, allowing you to run Lua scripts on Arduino-compatible hardware. This can be useful for adding scripting capabilities to your embedded projects.  
The project was built on a Raspberry Pi Pico board, but can be easily edited for other boards thanks to the PlatformIO IDE

## How to build PlatformIO based project

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Сlone this repository
3. Run these commands:

```shell
# Build project
$ pio run

# Upload project
$ pio run --target upload

# Clean build files
$ pio run --target clean
```
