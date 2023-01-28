#include "snake.h"
#include <stdio.h>

#define GAME_SCALE 2

#define GAME_DISPLAY_WIDTH OLED_DISPLAY_WIDTH / GAME_SCALE
#define GAME_DISPLAY_HEIGHT OLED_DISPLAY_HEIGHT / GAME_SCALE

#define SNAKE_LENGTH 3

#define SNAKE_MAX_LENGTH 256 // GAME_DISPLAY_WIDTH * GAME_DISPLAY_HEIGHT // Maximum length is the entire screen

char scoreString[9];
char highScoreString[9];

typedef struct {
  uint8_t x;
  uint8_t y;
} SnakeSegment;

SnakeSegment snake[SNAKE_MAX_LENGTH];

uint8_t snakeLength = 3;
uint8_t snake_dir;
uint8_t foodX, foodY;
uint8_t score = 0;
uint8_t highScore = 0;
uint16_t moveTimer;
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

void init_snake(void) {
    snakeLength = SNAKE_LENGTH;
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
}

void move_snake(void) {
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
    if (!game_win) {
        oled_clear();
        oled_set_cursor(1, 8);
        oled_write_P(PSTR("WOW!"), false); 
        oled_set_cursor(3, 2);
        oled_write_P(PSTR("Go touch grass!"), false); 
    } else {
        // Update highscore
        if (score > highScore) {
            highScore = score;
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
        game_is_running = true;
        render_snake();
        render_food();
    }
    if (!game_over) {
        if (timer_elapsed(moveTimer) > GAME_SPEED) {
            move_snake();
            update_snake();
        }
        oled_clear();
        render_snake();
        render_food();
    } else {
        render_gameover();
        game_is_running = false;
    }
}

