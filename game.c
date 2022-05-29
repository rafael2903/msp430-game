#include "game.h"

CarsQueue cars_queue;
Car* collided_car;
Car player_car = { .column = PLAYER_CAR_COLUMN, .page = ROAD_2, .image = UP_CAR };
enum STATE state = INITIAL;

CarsQueue create_cars_queue() {
    CarsQueue cars_queue;
    cars_queue.first = 0;
    cars_queue.last = 0;
    cars_queue.length = 0;

    return cars_queue;
}

void enqueue_car(uint8_t column, uint8_t page, uint8_t* image) {
    cars_queue.cars[cars_queue.last].column = column;
    cars_queue.cars[cars_queue.last].page = page;
    cars_queue.cars[cars_queue.last].image = image;

    cars_queue.last = (cars_queue.last + 1) % MAX;
    cars_queue.length++;
}

void dequeue_car() {
    cars_queue.first = (cars_queue.first + 1) % MAX;
    cars_queue.length--;
}

void print_car(Car* car) {
    oled_print_area(car->column, car->page, CAR_HEIGHT, CAR_WIDTH, car->image);
}

void remove_car(Car* car) {
    oled_clear_area(car->column, car->page, CAR_HEIGHT, CAR_WIDTH);
}

void show_start_game_screen() {
    oled_print_screen(START_GAME_SCREEN);
    oled_update_screen();
}

void print_game_over_text() {
    oled_print_area(29, 2, 18, 4, GAME_OVER_TEXT);
}

void show_game_over_scene() {
    unsigned int i = 4;

    oled_clear_screen();

    while (i--) {
        print_game_over_text();
        print_car(collided_car);
        print_car(&player_car);
        oled_update_screen();
        start_buzzer();
        wait(_1_2_SECOND);

        oled_clear_screen();
        oled_update_screen();
        stop_buzzer();
        wait(_1_2_SECOND);
    }
}

void move_player_car_right() {
    if (player_car.page - 3 < 0) return;
    remove_car(&player_car);
    player_car.page -= 3;
    print_car(&player_car);
}

void move_player_car_left() {
    if (player_car.page + 3 > 6) return;
    remove_car(&player_car);
    player_car.page += 3;
    print_car(&player_car);
}

void create_new_cars() {
    enqueue_car(0, player_car.page, DOWN_CAR); // always create a new car in the same user road

    uint8_t road;

    switch (player_car.page) {
    case ROAD_1:
        road = rand() % 2 ? ROAD_2 : ROAD_3;
        break;
    case ROAD_2:
        road = rand() % 2 ? ROAD_1 : ROAD_3;
        break;
    case ROAD_3:
        road = rand() % 2 ? ROAD_1 : ROAD_2;
        break;
    default:
        break;
    }
    enqueue_car(0, road, DOWN_CAR);
}

int collided(Car* game_car) {
    return (
        game_car->column + CAR_HEIGHT >= PLAYER_CAR_COLUMN &&
        game_car->page == player_car.page
    );
}

void move_car_forward(Car* car) {
    remove_car(car);
    car->column++;
    print_car(car);
}

void move_cars_forward() {
    int are_cars_far = TRUE;
    unsigned int counter = cars_queue.length;
    unsigned int i = cars_queue.first;
    Car* current_car;

    while (counter != 0) {
        current_car = &cars_queue.cars[i];
        if (current_car->column < 66) {
            are_cars_far = FALSE;
        }

        if (current_car->column > 99) {
            remove_car(current_car);
            dequeue_car();
        }
        else {
            move_car_forward(current_car);
            if (collided(current_car)) {
                collided_car = current_car;
                state = LOST;
                return;
            }
        }
        i++;
        if (i > MAX - 1) i = 0;
        counter--;
    }

    if (are_cars_far) create_new_cars();
    print_car(&player_car);
}

void start_game() {
    srand(time(NULL));
    oled_clear_screen();
    player_car.page = ROAD_2;
    print_car(&player_car);
    cars_queue = create_cars_queue();
    create_new_cars();
}
