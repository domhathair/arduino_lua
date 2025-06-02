#include <Arduino.h>
#include <limits>

#define LUA_IMPL
#define LUA_USE_C89
#define LUA_USE_LONGJMP
#include "minilua.h"

#ifndef LUA_MAXINPUT
#define LUA_MAXINPUT 2048
#endif

#ifndef LUA_PROMPT
#define LUA_PROMPT "> "
#endif

#define UART_BAUD 115200
#define UART_SIZE LUA_MAXINPUT

#undef Serial
#define Serial SerialUSB

lua_State *L;

void setup() {
    Serial.begin(UART_BAUD);

    L = luaL_newstate();
    if (L == NULL)
        while (true);
    luaL_openlibs(L);

    Serial.print(LUA_COPYRIGHT "\r\n" LUA_PROMPT);
}

void atLoopExit(void *) { digitalWrite(LED_BUILTIN, LOW); }

void loop() {
    static struct fat {
        size_t len;
        char buf[UART_SIZE + 1];
    } input = {0};
    [[gnu::__cleanup__(atLoopExit)]] char byte;

    if (Serial.available() < 1)
        return;
    digitalWrite(LED_BUILTIN, HIGH);

    byte = Serial.read() & SCHAR_MAX;
    switch (byte) {
    case '\r':
    case '\n':
        if (input.len == 0)
            return;
        input.buf[input.len] = '\0';
        Serial.print("\r\n");
        break;
    case '\b':
        if (input.len == 0)
            return;
        input.len--;
        Serial.print("\b \b");
        return;
    default:
        if (input.len >= UART_SIZE)
            return;
        input.buf[input.len++] = byte;
        Serial.print(byte);
        return;
    }

    int status = luaL_dostring(L, input.buf);
    if (status != LUA_OK) {
        Serial.print("Error: "), Serial.println(lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    input.len = 0;
    Serial.print(LUA_PROMPT);
}

extern size_t fwrite(const void *_s, size_t _size, size_t _n, FILE *) {
    for (size_t i = 0; i < _n; i += _size)
        for (size_t j = 0; j < _size; j++) /* \n */
            Serial.write(((const char *)_s)[i + j]);
    Serial.write('\r');

    return _size * _n;
}
