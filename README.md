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

## Usage example (Fibonacci sequence and custom function)

```shell
$ pyserial-miniterm /dev/ttyACM0
--- Miniterm on /dev/ttyACM0  9600,8,N,1 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
> function fib(n) if n <= 0 then return 0 elseif n == 1 then return 1 else local a, b = 0, 1; for i = 2, n do a, b = b, a + b; print(b) end end end
> fib(22)
1
2
3
5
8
13
21
34
55
89
144
233
377
610
987
1597
2584
4181
6765
10946
17711
> print(hw.whoami())
Raspberry Pi Pico interpreter: Lua 5.4.7
```