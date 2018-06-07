#include <SDL2/SDL.h>

#define debug(...) fprintf(stderr, __VA_ARGS__)

#define vec2(t) struct { \
  t x; \
  t y; \
}

#define vec3(t) struct { \
  t x; \
  t y; \
  t z; \
}

typedef vec2(int) pos;
typedef vec2(int) size;
typedef vec3(int) coord;

struct vertex {
  coord coord;
  struct vertex* connections[3];
};

struct cube {
  struct vertex points[8];
};

int init_video() {
  return SDL_Init(SDL_INIT_VIDEO);
}

int running = 1;

SDL_Window* window;

struct cube cube;

SDL_Window* init_window_raw(int x, int y, int w, int h, char* name) {
  SDL_Window* window = SDL_CreateWindow(
      name, x, y, w, h,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );
  if (!window) {
    debug("error creating window: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!rend) {
    debug("error creating renderer for %p: %s\n", window, SDL_GetError());
    return NULL;
  }

  return window;
}

SDL_Window* init_window(pos pos, size size, char* name) {
  return init_window_raw(pos.x, pos.y, size.x, size.y, name);
}

int draw_line(coord a, coord b) {
  return SDL_RenderDrawLine(SDL_GetRenderer(window), a.x, a.y, b.x, b.y);
}

int draw_loop(void* data) {
  while (running) {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 3; ++j) {
        draw_line(cube.points[i].coord, cube.points[i].connections[j]->coord);
      }
    }
  }
}

#define init_coord(a, b, c) { .x = a, .y = b, .z = c }
#define ptv(x) (&(vertices[x]))
#define connect(v, i, x) vertices[v].connections[i] = ptv(x)
#define connect3(v, a, b, c) do { \
  connect(v, 0, a); \
  connect(v, 1, b); \
  connect(v, 2, c); \
} while (0)

struct cube make_cube(int x, int y, int z, int s) {
  struct vertex vertices[8] = {
    { .coord = init_coord(x,   y,   z  ) }, // 0
    { .coord = init_coord(x+s, y,   z  ) }, // 1
    { .coord = init_coord(x,   y+s, z  ) }, // 2
    { .coord = init_coord(x+s, y+s, z  ) }, // 3
    { .coord = init_coord(x,   y,   z+s) }, // 4
    { .coord = init_coord(x+s, y,   z+s) }, // 5
    { .coord = init_coord(x,   y+s, z+s) }, // 6
    { .coord = init_coord(x+s, y+s, z+s) }  // 7
  };
  connect3(0, 1, 2, 4);
  connect3(1, 0, 3, 5);
  connect3(2, 0, 3, 6);
  connect3(3, 1, 2, 7);
  connect3(4, 0, 5, 6);
  connect3(5, 1, 4, 7);
  connect3(6, 2, 4, 7);
  connect3(7, 3, 5, 6);
}

int phys_loop(void* DATA) {
  while (1) {}
}

int main(int argc, char* args[]) {
  if (!init_video()) return 1;
  cube = make_cube(50, 50, 50, 20);
  window = init_window_raw(10, 10, 100, 100, "Hi");
  SDL_CreateThread(draw_loop, "trh_draw_loop", NULL);
  phys_loop(NULL);
}
