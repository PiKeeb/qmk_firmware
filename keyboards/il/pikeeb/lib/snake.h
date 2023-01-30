#pragma once

#if defined (SNAKE_ENABLE) && defined (OLED_ENABLE)
#pragma message "A snake, a snake Snaaake! A snaaaake Oooh, it's a snake"
#include QMK_KEYBOARD_H

#ifdef EEPROM_HIGHSCORE
typedef union {
  uint8_t raw;
  struct {
    uint8_t storedHighScore :8;
  };
} gameData_t;

gameData_t gameData;

extern gameData_t gameData;
#endif

/**
 * 
 **/
void render_game(void);

// Snake direction enum
enum snakeDirections {
    sUP,
    sRIGHT,
    sDOWN,
    sLEFT
};

bool game_is_running;

/**
 * 
 **/
uint8_t headDir;

/**
 * 
 **/
bool gameRestart;

/**
 * 
 **/
bool easymode;

#else 
#pragma message "Ssssshoot! Snake is not enabled! Hisssss... To enable me, make ssssure you have `SNAKE_ENABLE` and `OLED_ENABLE` sssset to `yes`"
#endif