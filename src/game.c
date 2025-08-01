typedef unsigned int uint32_t;

typedef int bool;
#define true 1
#define false 0

#include "prng.h"

#ifndef WIDTH
#define WIDTH 640
#endif

#ifndef HEIGHT
#define HEIGHT 480
#endif

static uint32_t cells[WIDTH*HEIGHT];
static uint32_t framebuffer[WIDTH*HEIGHT];

struct game_canvas {
	uint32_t *cells;
	uint32_t *framebuffer;
	int width, height;
};

void swap_buffers(struct game_canvas *gc) {
	for(int y = 0; y < gc->height; ++y) {
		for(int x = 0; x < gc->width; ++x) {
			// uint32_t tmp = gc->cells[y * gc->width + x];
			gc->cells[y * gc->width + x] = gc->framebuffer[y * gc->width + x];
			// gc->framebuffer[y * gc->width + x] = tmp;
		}
	}
}

void clear_canvas(struct game_canvas *gc) {
	for(int y = 0; y < gc->height; ++y) {
		for(int x = 0; x < gc->width; ++x) {
			gc->cells[y * gc->width + x] = 0;
		}
	}
}

void clear_framebuffer(struct game_canvas *gc) {
	for(int y = 0; y < gc->height; ++y) {
		for(int x = 0; x < gc->width; ++x) {
			gc->framebuffer[y * gc->width + x] = 0;
		}
	}
}

// byte order is reversed here for some reason idk
#define ALIVE 0xFFFF0000
#define DEAD  0x00000000
#define CELL_LIVE(gc, x, y) (gc)->framebuffer[(y * (gc)->width) + x] = ALIVE
#define CELL_DEAD(gc, x, y) (gc)->framebuffer[(y * (gc)->width) + x] = DEAD

bool is_alive(struct game_canvas gc, int x, int y) {
	return gc.cells[(y * gc.width) + x] == ALIVE;
}

int get_live_neighbors(struct game_canvas gc, int x, int y) {
	int total = 0;

	bool up = y != 0;
	bool down = y != gc.height-1;
	bool left = x != 0;
	bool right = x != gc.width-1;

	if(left && up && is_alive(gc, x-1, y-1)) ++total;
	if(left && is_alive(gc, x-1, y)) ++total;
	if(left && down && is_alive(gc, x-1, y+1)) ++total;
	if(up && is_alive(gc, x, y-1)) ++total;
	if(down && is_alive(gc, x, y+1)) ++total;
	if(right && up && is_alive(gc, x+1, y-1)) ++total;
	if(right && is_alive(gc, x+1, y)) ++total;
	if(right && down && is_alive(gc, x+1, y+1)) ++total;

	return total;
}

struct game_canvas randomize(unsigned int seed) {
	srand(seed);
	struct game_canvas gc = { cells, framebuffer, WIDTH, HEIGHT };

	// clear
	for(int y = 0; y < gc.height; ++y) {
		for(int x = 0; x < gc.width; ++x) {
			CELL_DEAD(&gc, x, y);
		}
	}

	for(int y = 0; y < gc.height; ++y) {
		for(int x = 0; x < gc.width; ++x) {
			if(randf() > 0.9f) CELL_LIVE(&gc, x, y);
		}
	}

	swap_buffers(&gc);
	return gc;
}

struct game_canvas render(void) {
	struct game_canvas gc = { cells, framebuffer, WIDTH, HEIGHT };

	clear_framebuffer(&gc);

	for(int y = 0; y < gc.height; ++y) {
		for(int x = 0; x < gc.width; ++x) {
			int live_neighbors = get_live_neighbors(gc, x, y);
			if(is_alive(gc, x, y)) {
				if(live_neighbors < 2 || live_neighbors > 3) CELL_DEAD(&gc, x, y);
				else CELL_LIVE(&gc, x, y);
			} else {
				if(live_neighbors == 3) CELL_LIVE(&gc, x, y);
				else CELL_DEAD(&gc, x, y);
			}
		}
	}

	swap_buffers(&gc);
	return gc;
}
