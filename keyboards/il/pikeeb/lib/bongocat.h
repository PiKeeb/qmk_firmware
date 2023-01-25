#pragma once
#include QMK_KEYBOARD_H
#include <stdio.h>

#   define IDLE_FRAMES 5
#   define IDLE_SPEED 20  // below this wpm value your animation will idle

// #define PREP_FRAMES 1 // uncomment if >1
#   define TAP_FRAMES 2
#   define TAP_SPEED 40  // above this wpm value typing animation to trigger

#   define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm
#   define ANIM_SIZE 636  // number of bytes in array, minimize for adequate firmware size, max is 1024

/* 
    Put `render_anim()` in your `oled_task_*` to render animation. 
    This code is good for the most common 128x32 or 128x64 display withs.
    Any other display width would require changing the code.
*/
void render_anim(void);