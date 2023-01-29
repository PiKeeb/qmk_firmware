#pragma once

#if defined (SNAKE_ENABLE) && defined (OLED_ENABLE)
#pragma message "A snake, a snake Snaaake! A snaaaake Oooh, it's a snake"
#include QMK_KEYBOARD_H

#define SNAKE_MAX_LENGTH 64

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

/**
 * 
 **/
void moveCheck(void);

// Snake direction enum
enum snakeDirections {
    sUP,
    sRIGHT,
    sDOWN,
    sLEFT
};

/**
 * 
 **/
uint8_t snake_lastdir; 

/**
 * 
 **/
uint8_t snake_dir;

/**
 * 
 **/
bool snakeRestart;

/**
 * 
 **/
bool game_is_running;

#else 
#pragma message "Ssssshoot! Snake is not enabled! Hisssss... To enable me, make ssssure you have `SNAKE_ENABLE` and `OLED_ENABLE` sssset to `yes`"
#endif