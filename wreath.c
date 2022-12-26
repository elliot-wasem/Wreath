#include <stdlib.h>
#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include "wreath.h"

// quick color definitions, for ease of use.
// colors declared inside main
#define RED           1
#define GREEN         2
#define YELLOW        3
#define BLUE          4
#define RESET         5
#define RED_GREEN     6
#define YELLOW_GREEN  7
#define BLUE_GREEN    8
#define CYAN_GREEN    9
#define YELLOW_RED   10

void screen_initialization();
void draw_wreath();
void draw_bow();
void draw_lights();
void draw_snowflakes(struct flake flakes[]);
void draw_greeting();

/* dimensions relative to which everything should be drawn. */
int zero_y = 0;
int zero_x = 0;

/* dimensions of screen */
int max_y = 0;
int max_x = 0;

int main() {
    int in = 0;

    screen_initialization();

    struct flake *flakes = malloc(sizeof(struct flake) * max_x);

    /*
     * fills snowflakes with initial positions, such that the y-coordinate is
     * random, and the x-coordinate is iterative
     */
    for(int i = 0; i < max_x; i++) {
        flakes[i].x = i;
        flakes[i].y = rand() % max_y;
    }

    while (in != 'q') {
        draw_snowflakes(flakes);
        draw_wreath();
        draw_bow();
        draw_lights();
        draw_greeting();
        refresh();
        in = getch();
    }


    endwin();

    free(flakes);

    return 0;
}

void screen_initialization() {

    // necessary for unicode display
    setlocale(LC_ALL, "");

    /* initialize screen */
    initscr();

    /* enables colors */
    start_color();

    /* initially refreshes screen, emptying it */
    refresh();

    /* keypresses will not be displayed on screen */
    noecho();

    /* hide cursor */
    curs_set(0);

    /* initialize input delay */
    timeout(200);

    /* setup lights randomizer */
    srand(time(NULL));

    /* sets color pairs to numbers defined above */
    init_pair(RED,          COLOR_RED,    COLOR_BLACK);
    init_pair(GREEN,        COLOR_GREEN,  COLOR_BLACK);
    init_pair(YELLOW,       COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE,         COLOR_BLUE,   COLOR_BLACK);
    init_pair(RESET,        COLOR_WHITE,  COLOR_BLACK);
    init_pair(RED_GREEN,    COLOR_RED,    COLOR_GREEN);
    init_pair(YELLOW_GREEN, COLOR_YELLOW, COLOR_GREEN);
    init_pair(BLUE_GREEN,   COLOR_BLUE,   COLOR_GREEN);
    init_pair(CYAN_GREEN,   COLOR_CYAN,   COLOR_GREEN);
    init_pair(YELLOW_RED,   COLOR_YELLOW, COLOR_RED);

    /* Setup screen dimensions */
    getmaxyx(stdscr, max_y, max_x);

    zero_y = max_y / 2 - (wreath_height / 2);
    zero_x = max_x / 2 - (wreath_width  / 2);

}

void draw_wreath() {
    attron(COLOR_PAIR(GREEN));

    for (int i = 0; wreath[i] != NULL; i++) {
        mvprintw(zero_y+i, zero_x, wreath[i]);
    }

    attroff(COLOR_PAIR(GREEN));

}

void draw_bow() {
    attron(COLOR_PAIR(RED_GREEN));
    mvprintw(zero_y+12, zero_x+11, "▟███");
    attroff(COLOR_PAIR(RED_GREEN));

    attron(COLOR_PAIR(RED));
    mvprintw(zero_y+12, zero_x+15, "▙     ▟");
    attroff(COLOR_PAIR(RED));

    attron(COLOR_PAIR(RED_GREEN));
    mvprintw(zero_y+12, zero_x+22, "███▙");
    attroff(COLOR_PAIR(RED_GREEN));

    attron(COLOR_PAIR(RED_GREEN));
    mvprintw(zero_y+13, zero_x+11, bow[1]);
    mvprintw(zero_y+14, zero_x+11, bow[2]);
    mvprintw(zero_y+15, zero_x+11, bow[3]);
    mvprintw(zero_y+16, zero_x+11, bow[4]);
    mvprintw(zero_y+17, zero_x+11, bow[5]);
    attroff(COLOR_PAIR(RED_GREEN));

    attron(COLOR_PAIR(RED));
    mvprintw(zero_y+18, zero_x+11, bow[6]);
    mvprintw(zero_y+19, zero_x+11, bow[7]);
    attroff(COLOR_PAIR(RED));

    attron(COLOR_PAIR(YELLOW_RED));
    mvprintw(zero_y+15, zero_x+18, "◉");
    attroff(COLOR_PAIR(YELLOW_RED));
}

void draw_lights() {
    static int do_draw_lights = 0;
    do_draw_lights = (do_draw_lights + 1) % 2;

    if (!do_draw_lights) {
        return;
    }

    int light_colors[] = { RED_GREEN, YELLOW_GREEN, BLUE_GREEN, CYAN_GREEN };
    for (int i = 0; i < wreath_height; i++) {
        for (int j = 0; j < wreath_width; j++) {
            if (lights[i][j]) {
                int color = light_colors[rand() % (sizeof(light_colors) / sizeof(int))];
                attron(COLOR_PAIR(color));
                mvprintw(zero_y+i, zero_x+j, "◉");
                attroff(COLOR_PAIR(color));
            }
        }
    }
}

void draw_snowflakes(struct flake flakes[]) {
    for (int i = 0; i < max_x; i++) {
        mvprintw(flakes[i].y, flakes[i].x, " ");
    }

    for(int i = 0; i < max_x; i++) {
        int flake_variety = rand() % 3;

        if(flakes[i].y < max_y-1) {
            flakes[i].y++;
        }

        mvprintw(flakes[i].y,
             flakes[i].x,
             (flake_variety == 2 ? "*" : (flake_variety == 1 ? "+" : "x")));
    }

    int random_flake = rand() % max_x;
    mvprintw(flakes[random_flake].y, flakes[random_flake].x, " ");
    flakes[random_flake].y = 0;
}

void draw_greeting() {
    mvprintw(zero_y+7, zero_x+16, "MERRY");
    mvprintw(zero_y+8, zero_x+14, "CHRISTMAS");
    mvprintw(zero_y+9, zero_x+13, "AND A HAPPY");
    mvprintw(zero_y+10, zero_x+14, "NEW YEAR");
    mvprintw(zero_y+22, zero_x+8, "Github: elliot-wasem");
    mvprintw(zero_y+24, zero_x+15, "Quit: q");
}
