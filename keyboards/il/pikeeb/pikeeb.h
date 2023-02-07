#pragma once
#include "quantum.h"

// Keyboard Layer names
enum layer_kb_names {
  _BASE,
  _FN1,
  _FN2,
  _SET
};

// Clears OLED when true
bool OLED_TO_CLEAR;

// OLED Layer names
enum layer_oled_names {
  _OLED_STATUS,
  _OLED_VOLTAGE,
  _OLED_WPM,
  _OLED_GAME
};

// Custom keycodes
enum pikeeb_keycodes {
  USB_SW = SAFE_RANGE,
  OLED_SW,
  OLED_PG_CYCL,
  OLED_PG_STAT,
  USER_SAFE_RANGE // Safe range for a user defined keycodes
};

// EEPROM values
typedef union {
  uint32_t raw;
  struct {
    bool usbswitch_high :1;
    uint8_t current_layer_kb :8;
    uint8_t current_layer_oled :8;
  };
} kb_config_t;

kb_config_t kb_config;

extern kb_config_t kb_config;