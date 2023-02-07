#pragma once

#if defined (SNAKE_ENABLE) && defined (OLED_ENABLE)
#pragma message "A snake, a snake Snaaake! A snaaaake Oooh, it's a snake"
#include QMK_KEYBOARD_H

// EEPROM highscore value
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
 * Main function that renders the game on the OLED display
 **/
void render_game(void);

/**
 * Value that controls the head direction of the snake
 **/
uint8_t headDir;

// Snake direction enum for the headDir value
enum snakeDirections {
    sUP,
    sRIGHT,
    sDOWN,
    sLEFT
};

/**
 * Boolean flag for when the game is over 
 **/
bool gameOver;

/**
 * Boolean flag for when the game is running
 **/
bool game_is_running;

/**
 * Boolean flag for restarting the game. Flip to `true` to restart.
 **/
bool gameRestart;

/**
 * Boolean flag to toggle the easymode (`true` = no border, snake teleports to the other side of the display)
 **/
bool easymode;

#else 
#pragma message "Ssssshoot! Snake is not enabled! Hisssss... To enable me, make ssssure you have `SNAKE_ENABLE` and `OLED_ENABLE` sssset to `yes`"
#endif