#include "snake.h"
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <stdbool.h>

#ifndef SNAKE_STARTING_LENGTH
#   define SNAKE_STARTING_LENGTH 10
#endif

#ifndef SNAKE_MAX_LENGTH
#   define SNAKE_MAX_LENGTH 32
#endif

#ifndef OLED_DISPLAY_WIDTH
#   define OLED_DISPLAY_WIDTH 128
#endif

#ifndef OLED_DISPLAY_HEIGHT
#   define OLED_DISPLAY_HEIGHT 32
#endif

// Update game values in ms
#ifndef GAME_SPEED
#   define GAME_SPEED 250
#endif

#ifndef FOOD_FLASH_RATE
#   define FOOD_FLASH_RATE GAME_SPEED / 2
#endif

#define GAME_DISPLAY_WIDTH OLED_DISPLAY_WIDTH - 1
#define GAME_DISPLAY_HEIGHT OLED_DISPLAY_HEIGHT - 1

uint8_t headX, headY;
uint8_t tailX[SNAKE_MAX_LENGTH];
uint8_t tailY[SNAKE_MAX_LENGTH];

char scoreString[9];
char highScoreString[9];

uint8_t tailLength;

uint8_t headDir;
uint8_t headLastDir;
uint8_t foodX, foodY;
uint8_t score = 0;
uint8_t highScore = 0;

uint8_t lastTailX;
uint8_t lastTailY;

uint16_t foodTimer = 0;
uint16_t moveTimer;

bool foodVisible = true;

bool snakeInitialised = false;
bool gameRestart = false;
bool gameOver = false;
bool game_win = false;
bool easymode = false;

bool check_food_overlap(void) {
    if (foodX == headX && foodY == headY) {
            return true;
    }
    for (uint8_t i = 0; i < tailLength; i++) {
        if (foodX == tailX[i] && foodY == tailY[i]) {
            return true;
        }
    };
    return false;
}

void spawn_food(void) {

    // (rand() % (upper - lower + 1)) + lower;
    // upper = GAME_DISPLAY_* - 1; lower = 1;
    // ((GAME_DISPLAY_* - 1) - 1) + 1 = GAME_DISPLAY_* - 1

    foodX = (random() % GAME_DISPLAY_WIDTH - 1) + 1; 
    foodY = (random() % GAME_DISPLAY_HEIGHT - 1) + 1; 

    while(check_food_overlap()) {
        foodX = (random() % GAME_DISPLAY_WIDTH - 1) + 1;
        foodY = (random() % GAME_DISPLAY_HEIGHT - 1) + 1;
    }
}

void check_game_over(void) {
    // Check the gameOver flag
    if (gameOver) {
        return;
    }

    // Check the length of a tail
    if (tailLength >= SNAKE_MAX_LENGTH) {
        OLED_TO_CLEAR = true;
        game_win = true;
        gameOver = true;
    }

    if (!easymode) {
        if (headX == 0 && headY == 0) {
            headY = 1;
            headX = 1;
        }
        // Check if snake is at the edge of the screen
        if (headX == GAME_DISPLAY_WIDTH - 1) {
            OLED_TO_CLEAR = true;
            gameOver = true;
        } else if (headX == 0) {
            OLED_TO_CLEAR = true;
            gameOver = true;
        }
        if (headY == GAME_DISPLAY_HEIGHT - 1) {
            OLED_TO_CLEAR = true;
            gameOver = true;
        } else if (headY == 0) {
            OLED_TO_CLEAR = true;
            gameOver = true;
        }
    }

    // Check for collision with itself
    for (uint8_t i = 1; i < tailLength; i++) {
        if (headX == tailX[i] && headY == tailY[i]) {
            OLED_TO_CLEAR = true;
            gameOver = true;
        }
    }    
}

void checkFood(void) {
    // Check for collision with food
    if (headX == foodX && headY == foodY) {
        tailLength++;
        score++;
        spawn_food();
    }
}

void init_snake(void) {
    tailLength = SNAKE_STARTING_LENGTH;
    headX = 10;
    headY = 10;
    headDir = sRIGHT;
    headLastDir = headDir;
    for(uint8_t i = 0; i<tailLength; i++) {
        tailY[i] = headY;
        tailX[i] = headX - i;
    }
    spawn_food();
    moveTimer = timer_read();
    snakeInitialised = true;
    game_is_running = true;
}

void checkMove(void) {
    if (headDir == sRIGHT && headLastDir == sLEFT) {
      headDir = headLastDir;
    }
    if (headDir == sLEFT && headLastDir == sRIGHT) {
      headDir = headLastDir;
    }
    if (headDir == sUP && headLastDir == sDOWN) {
      headDir = headLastDir;
    }
    if (headDir == sDOWN && headLastDir == sUP) {
      headDir = headLastDir;
    }
}

void move_snake(void) {
    checkMove();

    // Record the last tail coordinates
    lastTailX = tailX[tailLength - 1];
    lastTailY = tailY[tailLength - 1];

    switch (headDir) {
    case sUP:
        headY --;
        break;
    case sDOWN:
        headY ++;
        break;
    case sLEFT:
        headX --;
        break;
    case sRIGHT:
        headX ++;
    default:
        break;
    }

    headLastDir = headDir;

    // Move the tail along with the head
    for (uint8_t i = tailLength - 1; i > 0; i--) {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }
    tailX[0] = headX;
    tailY[0] = headY;

    // Teleport the head in easy mode
    if (easymode) {
        if (headX >= GAME_DISPLAY_WIDTH - 1) {
            headX = 1;
        } else if (headX <= 1) {
            headX = GAME_DISPLAY_WIDTH - 1;
        }
        if (headY >= GAME_DISPLAY_HEIGHT - 1) {
            headY = 1;
        } else if (headY <= 1) {
            headY = GAME_DISPLAY_HEIGHT - 1;
        }
    }
}

void draw_border(void) {
    for (uint8_t x = 0; x < GAME_DISPLAY_WIDTH; x++) {
        oled_write_pixel(x, 0, true);
        oled_write_pixel(x, GAME_DISPLAY_HEIGHT, true);
    }
    for (uint8_t y = 0; y < GAME_DISPLAY_HEIGHT; y++) {
        oled_write_pixel(0, y, true);
        oled_write_pixel(GAME_DISPLAY_WIDTH, y, true);
    }
}

void render_snake(void) {
    for (uint8_t i = 0; i < tailLength; i++) {
        oled_write_pixel(tailX[i], tailY[i], true);
    }
    oled_write_pixel(lastTailX, lastTailY, false);
}

void render_food(void) {
    if(foodVisible) {
        oled_write_pixel(foodX, foodY, true);
    } else {
        oled_write_pixel(foodX, foodY, false);
    }
}

void check_highscore(void){
#ifdef EEPROM_HIGHSCORE
  gameData.raw = eeconfig_read_kb();
  highScore = gameData.storedHighScore;
  if ( score > highScore ) {
    gameData.storedHighScore = score;
    eeconfig_update_kb(gameData.raw);
  }
#else
  if (score > highScore) {
    highScore = score;
  }
#endif
}

void check_restart(void){
    if (gameRestart) {
        gameOver = false;
        game_win = false;
        snakeInitialised = false;
        score = 0;
        OLED_TO_CLEAR = true;
        gameRestart = !gameRestart;
    }
}

void render_gameover(void) {
    if (game_win) {
        oled_set_cursor(0, 1);
        oled_write_P(PSTR("WOW!"), false);
        oled_set_cursor(0, 2);
        oled_write_P(PSTR("YOU WON!"), false); 
        oled_set_cursor(0, 3);
        oled_write_P(PSTR("Go touch grass!"), false); 
    } else {
        // Update highscore
        check_highscore();
        // Output score to scoreString
        sprintf(scoreString, "%u", score);
        sprintf(highScoreString, "%u", highScore);
        oled_set_cursor(0, 0);
        oled_write_P(PSTR("GAME OVER"), false); 
        oled_set_cursor(0, 1);
        oled_write_P(PSTR("Score:"), false); 
        oled_write_ln(scoreString, false);
        oled_set_cursor(0, 2);
        oled_write_P(PSTR("HiScore:"), false); 
        oled_write_ln(highScoreString, false);
        oled_set_cursor(0, 3);
        oled_write_P(PSTR("Play Again? y/n"), false);
    }
}

void render_game(void) {
    if (gameOver) {
        render_gameover();
        check_restart();
        return;
    }
    if (!easymode) {
        draw_border();
    }
    if (!snakeInitialised) {
        init_snake();
    }
    if (timer_elapsed(moveTimer) > GAME_SPEED) {
        move_snake();
        checkFood();
        moveTimer = timer_read();
    }
    if(timer_elapsed(foodTimer) > FOOD_FLASH_RATE) {
        foodVisible = !foodVisible;
        foodTimer = timer_read();
    }
    render_snake();
    render_food();
    check_game_over();
}