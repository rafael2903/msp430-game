#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "oled.h"
#include "elements.h"
#include "timers.h"

#define CAR_HEIGHT          28
#define CAR_WIDTH           2
#define ROAD_1              0
#define ROAD_2              3
#define ROAD_3              6
#define PLAYER_CAR_COLUMN   100
#define TRUE                1
#define FALSE               0
#define MAX                 8

typedef struct {
    uint8_t column;
    uint8_t page;
    uint8_t* image;
} Car;

typedef struct {
    int first;
    int last;
    int length;
    Car cars[MAX];
} CarsQueue;

enum STATE {
    INITIAL,
    PLAYING,
    LOST
};

extern enum STATE state;

void show_start_game_screen();
void show_game_over_scene();
void move_player_car_right();
void move_player_car_left();
void move_cars_forward();
void start_game();

#endif  // GAME_H_
