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

/**
 * Prints board name
 **/
static int lua_whoami(lua_State *L) {
    lua_pushstring(L, "Raspberry Pi Pico interpreter: " LUA_RELEASE);

    return 1;
}

/**
 * Custom reboot function
 **/
static int lua_reboot(lua_State *) {
    volatile uint32_t *AIRCR = (volatile uint32_t *)0xE000ED0C;
    *AIRCR = 0x5FA0004;

    return 0;
}

static const struct luaL_Reg lua_hw[] = {
    /* Register your own functions here */
    {"whoami", lua_whoami}, /* \n */
    {"reboot", lua_reboot}, /* \n */
    {NULL, NULL}};

static int luaopen_hw(lua_State *L) {
    luaL_newlib(L, lua_hw);
    return 1;
}

extern void setup() {
    Serial.begin(UART_BAUD);

    L = luaL_newstate();
    if (L == NULL)
        while (true);
    luaL_openlibs(L);

    luaL_requiref(L, "hw", luaopen_hw, 1);
    lua_pop(L, 1);

    Serial.print(LUA_COPYRIGHT "\r\n" LUA_PROMPT);
}

static void atLoopExit(void *) { digitalWrite(LED_BUILTIN, LOW); }

extern void loop() {
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
