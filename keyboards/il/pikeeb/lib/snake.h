#pragma once

#if defined (SNAKE_ENABLE) && defined (OLED_ENABLE)
#pragma message "A snake, a snake Snaaake! A snaaaake Oooh, it's a snake"

#include QMK_KEYBOARD_H

#ifndef OLED_DISPLAY_WIDTH
#   define OLED_DISPLAY_WIDTH 128
#endif

#ifndef OLED_DISPLAY_HEIGHT
#   define OLED_DISPLAY_HEIGHT 32
#endif

// Update game values in ms
#ifndef GAME_SPEED
#   define GAME_SPEED 1000
#endif


// Snake direction enum
enum snakeDirection {
    sUP,
    sRIGHT,
    sDOWN,
    sLEFT
};

/**
 * 
 **/
void render_game(void);

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