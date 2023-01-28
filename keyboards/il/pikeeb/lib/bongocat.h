#pragma once

#if defined (BONGOCAT_ENABLE) && defined (WPM_ENABLE) && defined (OLED_ENABLE)
#pragma message "Meow! =^_^= Bongocat is enabled! =UwU="

#include QMK_KEYBOARD_H
#include <stdio.h>

#ifndef IDLE_FRAMES
#   define IDLE_FRAMES 5
#endif

#ifndef IDLE_SPEED
#   define IDLE_SPEED 20  // below this wpm value your animation will idle
#endif

#ifndef PREP_FRAMES
// #   define PREP_FRAMES 1 // uncomment if >1
#endif

#ifndef TAP_FRAMES
#   define TAP_FRAMES 2
#endif

#ifndef TAP_SPEED
#   define TAP_SPEED 40  // above this wpm value typing animation to trigger
#endif

#ifndef ANIM_FRAME_DURATION
#   define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#endif

#ifndef SLEEP_TIMER
// #   define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm
#endif

#ifndef ANIM_SIZE
#   define ANIM_SIZE 636  // number of bytes in array, minimize for adequate firmware size, max is 1024
#endif

/* 
    Put `render_anim()` in your `oled_task_*` to render animation. 
    This code is good for the most common 128x32 or 128x64 display withs.
    Any other display width would require changing the code.
*/
void render_anim(void);

#else 
#pragma message "Oh neow! =0_o= Bongocat is not enabled! =X_x= To enable me, make sure you have `BONGOCAT_ENABLE`, `WPM_ENABLE` and `OLED_ENABLE` set to `yes`"
#endif