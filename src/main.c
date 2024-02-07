#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 169
#define HEIGHT 39

#define DELAY 90000

#define ON '@'
#define OFF ' '

bool grid[HEIGHT][WIDTH] = {0};

bool rand_bool(void)
{
    return ( (float) rand() / (float) RAND_MAX ) > 0.5f;
}

void clear_grid(void)
{
    for(size_t y = 0; y < HEIGHT; ++y) {
        for(size_t x = 0; x < WIDTH; ++x) {
            grid[y][x] = 0;
        }
    }
}

void random_grid(void)
{
    for(size_t y = 0; y < HEIGHT; ++y) {
        for(size_t x = 0; x < WIDTH; ++x) {
            grid[y][x] = rand_bool();
        }
    }
}

void display_grid(void)
{
    for(size_t y = 0; y < HEIGHT; ++y) {
        for(size_t x = 0; x < WIDTH; ++x) {
            /* fputc((grid[y][x] ? '#' : ' '), stdout); */
            printw("%c", (grid[y][x] ? ON : OFF));
        }
        printw("\n");
    }
}

/*
int get_live_neighbors(size_t x, size_t y) {
    int live_neighbors = 0;
    for(size_t dy = -1; dy < 2; ++dy) {
        if((y+dy) < 0 || (y+dy) > HEIGHT) continue;
        for(size_t dx = -1; dx < 2; ++dx) {
            if((x+dx) < 0 || (x+dx) > WIDTH) continue;
            if(grid[y+dy][x+dx]) ++live_neighbors;
        }
    }

    return live_neighbors;
}
*/

void tick(void)
{
    refresh();
    clear();
    bool grid1[HEIGHT][WIDTH] = {0};
    for(size_t y = 0; y < HEIGHT; ++y) {
        for(size_t x = 0; x < WIDTH; ++x) {
            bool up, down, left, right;
            up = y!=0;
            down = y!=HEIGHT-1;
            left = x!=0;
            right = x!=WIDTH-1;

            int live_neighbors = 0;
            live_neighbors += (left && up && grid[y-1][x-1]);
            live_neighbors += (left && grid[y][x-1]);
            live_neighbors += (left && down && grid[y+1][x-1]);
            live_neighbors += (up && grid[y-1][x]);
            live_neighbors += (down && grid[y+1][x]);
            live_neighbors += (right && up && grid[y-1][x+1]);
            live_neighbors += (right && grid[y][x+1]);
            live_neighbors += (right && down && grid[y+1][x+1]);

            if(grid[y][x]) {
                grid1[y][x] = (live_neighbors == 2 || live_neighbors == 3) && live_neighbors > 1 && live_neighbors < 4;
            } else {
                grid1[y][x] = (live_neighbors == 3);
            }
        }
    }

    for(size_t y = 0; y < HEIGHT; ++y) {
        for(size_t x = 0; x < WIDTH; ++x) {
            grid[y][x] = grid1[y][x];
        }
    }

    display_grid();
}

int main(void)
{
    srand(time(0));

    /* Initialize ncurses */
    initscr();
    cbreak();
    keypad(stdscr, true);
    timeout(0);
    noecho();

    random_grid();
    display_grid();

    bool pause = false;
    int c = 0;
    while(c != 'q') {
        c = getch();
        switch(c) {
            case 'r': 
                random_grid();
                break;
            case 'p':
            case ' ':
                pause = !pause;
                break;
                
            case 'c':
                clear_grid();
                break;
            case KEY_RIGHT:
                tick();
                break;
            default:
                break;;
        }
        if(!pause) tick();
        usleep(DELAY);
    }

    refresh();
    endwin();

    return 0;
}
