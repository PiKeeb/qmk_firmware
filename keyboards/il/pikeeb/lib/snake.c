#include "snake.h"
#include <stdio.h>
#include <stdbool.h>

#ifndef SNAKE_STARTING_LENGTH
#   define SNAKE_STARTING_LENGTH 3
#endif

#ifndef FOOD_FLASH_RATE
#   define FOOD_FLASH_RATE 450
#endif

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

#ifndef GAME_SCALE
#   define GAME_SCALE 2
#endif

#define GAME_DISPLAY_WIDTH OLED_DISPLAY_WIDTH / GAME_SCALE
#define GAME_DISPLAY_HEIGHT OLED_DISPLAY_HEIGHT / GAME_SCALE

typedef struct {
  uint8_t x;
  uint8_t y;
} SnakeSegment;

SnakeSegment snake[SNAKE_MAX_LENGTH];

char scoreString[9];
char highScoreString[9];

uint8_t snakeLength = 3;
uint8_t snake_dir;
uint8_t snake_lastdir;
uint8_t foodX, foodY;
uint8_t score = 0;
uint8_t highScore = 0;

uint16_t foodTimer = 0;
uint16_t moveTimer;

bool foodVisible = true;

bool snakeInitialised = false;
bool snakeRestart = false;
bool game_is_running = false;

bool game_over = false;
bool game_win = false;

bool check_food_overlap(void) {
    for (uint8_t i = 0; i < snakeLength; i++) {
        if (foodX == snake[i].x && foodY == snake[i].y) {
            return true;
        }
    };
    return false;
}

void spawn_food(void) {
    foodX = random() % GAME_DISPLAY_WIDTH;
    foodY = random() % GAME_DISPLAY_HEIGHT;

    while(check_food_overlap()) {
        foodX = random() % GAME_DISPLAY_WIDTH;
        foodY = random() % GAME_DISPLAY_HEIGHT;
    }
}

void update_snake(void) {
    for (uint8_t i = snakeLength; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    snake[0].x += snake_dir;
    if (snake[0].x >= GAME_DISPLAY_WIDTH) {
        snake[0].x = 0;
    } else if (snake[0].x < 0) {
        snake[0].x = GAME_DISPLAY_WIDTH - 1;
    }
    if (snake[0].y >= GAME_DISPLAY_HEIGHT) {
        snake[0].y = 0;
    } else if (snake[0].y < 0) {
        snake[0].y = GAME_DISPLAY_HEIGHT - 1;
    }
    // Check for collision with itself
    for (uint8_t i = 1; i < snakeLength; i++) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
            game_over = true;
        }
    }
    // Check for collision with food
    if (snake[0].x == foodX && snake[0].y == foodY) {
        snakeLength++;
        score++;
        if (snakeLength >= SNAKE_MAX_LENGTH) {
            game_win = true;
            game_over = true;
        }
        spawn_food();
    }
}

void update_food(void) {
    uint16_t currentTime = timer_read();
    if(currentTime - foodTimer > FOOD_FLASH_RATE) {
        foodVisible = !foodVisible;
        foodTimer = currentTime;
    }
}

void init_snake(void) {
    snakeLength = SNAKE_STARTING_LENGTH;
    for (uint8_t i = 0; i < snakeLength; i++) {
        snake[i].x = GAME_DISPLAY_WIDTH / 2;
        snake[i].y = GAME_DISPLAY_HEIGHT / 2;
    }
    snake_dir = sRIGHT;
    for(uint8_t i=1;i<snakeLength;i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            snake[0].x++;
        }
    }
    spawn_food();
    moveTimer = timer_read();
    snakeInitialised = true;
    game_is_running = true;
}

void moveCheck(void) {
  if (snake[0].x == snake[1].x && snake[0].y == snake[1].y) {
    if (snake_dir == sRIGHT && snake_lastdir == sLEFT) {
      snake_dir = snake_lastdir;
    }
    else if (snake_dir == sLEFT && snake_lastdir == sRIGHT) {
      snake_dir = snake_lastdir;
    }
    else if (snake_dir == sUP && snake_lastdir == sDOWN) {
      snake_dir = snake_lastdir;
    }
    else if (snake_dir == sDOWN && snake_lastdir == sUP) {
      snake_dir = snake_lastdir;
    }
  }
  snake_lastdir = snake_dir;
}

void move_snake(void) {
    moveCheck();
    for (uint8_t i = snakeLength; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    switch (snake_dir) {
    case sUP:
        snake[0].y--;
        if (snake[0].y < 0) {
            snake[0].y = GAME_DISPLAY_HEIGHT - 1;
        }
        break;
    case sDOWN:
        snake[0].y++;
        if (snake[0].y >= GAME_DISPLAY_HEIGHT) {
            snake[0].y = 0;
        }
        break;
    case sLEFT:
        snake[0].x--;
        if (snake[0].x < 0) {
            snake[0].x = GAME_DISPLAY_WIDTH - 1;
        }
        break;
    case sRIGHT:
        snake[0].x++;
        if (snake[0].x >= GAME_DISPLAY_WIDTH) {
            snake[0].x = 0;
        }
    default:
        break;
    }
}

void draw_border(void) {
    for (uint8_t x = 0; x < OLED_DISPLAY_WIDTH; x++) {
        oled_write_pixel(x, 0, true);
        oled_write_pixel(x, OLED_DISPLAY_HEIGHT - 1, true);
    }
    for (uint8_t y = 0; y < OLED_DISPLAY_HEIGHT; y++) {
        oled_write_pixel(0, y, true);
        oled_write_pixel(OLED_DISPLAY_WIDTH - 1, y, true);
    }
}

void draw_dashed_border(uint8_t scale) {
  for (uint8_t x = 0; x < OLED_DISPLAY_WIDTH; x += scale * 2) {
    for (uint8_t y = 0; y < OLED_DISPLAY_HEIGHT; y++) {
      if (x % 2 == 0) {
        oled_write_pixel(x, y, true);
      }
    }
  }
  for (uint8_t y = 0; y < OLED_DISPLAY_HEIGHT; y += scale * 2) {
    for (uint8_t x = 0; x < OLED_DISPLAY_WIDTH; x++) {
      if (y % 2 == 0) {
        oled_write_pixel(x, y, true);
      }
    }
  }
}

void draw_block(uint8_t x, uint8_t y, uint8_t scale) {
  for (uint8_t i = 0; i < scale; i++) {
    for (uint8_t j = 0; j < scale; j++) {
      oled_write_pixel(x + i, y + j, true);
    }
  }
}

void render_snake(void) {
  for (uint8_t i = 0; i < snakeLength; i++) {
    draw_block(snake[i].x, snake[i].y, GAME_SCALE);
  }
}


void render_food(void) {
    if(foodVisible) {
        draw_block(foodX, foodY, GAME_SCALE);
    }
}

/*
void render_snake(void) {
  for (uint8_t i = 0; i < snakeLength; i++) {
    for (uint8_t x = 0; x < GAME_SCALE; x++) {
      for (uint8_t y = 0; y < GAME_SCALE; y++) {
        oled_write_pixel(snake[i].x * GAME_SCALE + x, snake[i].y * GAME_SCALE + y, true);
      }
    }
  }
}

void render_food(void) {
  for (uint8_t x = 0; x < GAME_SCALE; x++) {
    for (uint8_t y = 0; y < GAME_SCALE; y++) {
       oled_write_pixel(foodX * GAME_SCALE + x, foodY * GAME_SCALE + y, true);
    }
  }
}
*/

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
    if (snakeRestart && !game_is_running) {
        game_over = false;
        snakeInitialised = false;
        snakeRestart = !snakeRestart;
    }
    if (snakeRestart && game_is_running) {
        game_over = false;
        snakeInitialised = false;
        snakeRestart = !snakeRestart;
    }
}

void render_gameover(void) {
    if (game_win) {
        oled_clear();
        oled_set_cursor(1, 8);
        oled_write_P(PSTR("WOW!"), false);
        oled_set_cursor(2, 5);
        oled_write_P(PSTR("YOU WON!"), false); 
        oled_set_cursor(3, 2);
        oled_write_P(PSTR("Go touch grass!"), false); 
    } else {
        // Update highscore
        check_highscore();
        if (game_is_running) {
            game_is_running = !game_is_running;
        }
        // Output score to scoreString
        sprintf(scoreString, "%u", score);
        sprintf(highScoreString, "%u", highScore);
        oled_clear();
        oled_set_cursor(0, 5);
        oled_write_P(PSTR("GAME OVER"), false); 
        oled_set_cursor(1, 0);
        oled_write_P(PSTR("Score:   "), false); 
        oled_write_ln(scoreString, false);
        oled_set_cursor(2, 0);
        oled_write_P(PSTR("HiScore: "), false); 
        oled_write_ln(highScoreString, false);
        oled_set_cursor(3, 0);
        oled_write_P(PSTR("Play Again? y/n"), false);
        check_restart();
    }
}

void render_game(void) {
    if (!snakeInitialised) {
        init_snake();
        oled_clear();
        render_snake();
        render_food();
    }
    if (!game_over) {
        if (timer_elapsed(moveTimer) > GAME_SPEED) {
            move_snake();
            update_snake();
        }
        oled_clear();
        draw_border();
        render_snake();
        update_food();
        render_food();
    } else {
        render_gameover();
    }
}

