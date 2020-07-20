#include <SDL2/SDL.h>
#include <gui.h>
#include <utilities.h>
#include <structs.h>
#include <cfg.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <list.h>
#include <move.h>

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = 30;
const int ALIEN_SIZE = 30;

const int X_TILES = 46;
const int Y_TILES = 24;

int map[24][46];

point positions[3];
int list_size = 0;

// point actual_pos;
int velocity = 0;

int finish = 0;

point routes_a[5][5];
point routes_b[5][5];

pthread_t threads[2];
pthread_mutex_t lock;

void *alien_thread(void *param);
int spawn_alien(void);

int main(int argc, char *argv[])
{
  int medium = 0;
  if (!valdite_args(argc, argv, &medium))
  {
    return 1;
  }

  if (init_graphics() != 0)
  {
    return 1;
  }

  if (!check_bin_dir())
  {
    printf("Please execute this program from the bin directory\n");
    return 1;
  }

  if (!load_map(map))
  {
    printf("Can't read map\n");
    return 1;
  }

  if (!init_cfg())
  {
    printf("Can't read configuration file\n");
    return 1;
  }

  load_alien(&velocity);

  /* Test Start*/
  if (medium != 0)
  {
    printf("Automatic Mode Selected\n");
    printf("%d\n", medium);
  }
  else
  {
    printf("Manual Mode Selected\n");
  }

  // pthread_t thread1;
  // int iret1;

  // iret1 = pthread_create(&thread1, NULL, &alien_thread, NULL);
  // if (iret1)
  // {
  //   fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
  //   exit(EXIT_FAILURE);
  // }

  /* Test End */

  /* Alien Position Initialization */
  // actual_pos.x = 1290;
  // actual_pos.y = 390;

  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("Mutex init has failed\n");
    return 1;
  }

  init_routes(routes_a, routes_b);

  SDL_Window *win = SDL_CreateWindow("Alien's Community", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                     SDL_WINDOW_SHOWN);
  if (win == NULL)
  {
    log_error("SDL Create Window Error");
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL)
  {
    SDL_DestroyWindow(win);
    log_error("SDL Create Renderer Error");
    SDL_Quit();
    return 1;
  }

  SDL_Texture *alien_a = load_texture("../assets/images/alien_b.png", ren);
  SDL_Texture *brigde_road = load_texture("../assets/images/road.png", ren);
  SDL_Texture *bridge = load_texture("../assets/images/bridge.png", ren);
  SDL_Texture *road = load_texture("../assets/images/asphalt.png", ren);
  SDL_Texture *background = load_texture("../assets/images/background2.png", ren);
  SDL_Texture *base_a = load_texture("../assets/images/A.png", ren);
  SDL_Texture *base_b = load_texture("../assets/images/B.png", ren);

  int quit = 0;

  SDL_Event e;

  SDL_Rect mouse_rect;
  SDL_Rect img_rect;
  mouse_rect.w = mouse_rect.h = 1;

  while (!quit)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit = 1;
        finish = 1;
      }

      if (e.type == SDL_KEYDOWN)
      {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
          //spawn_alien(positions, &list_size);
          spawn_alien();
          break;

        default:
          break;
        }
      }

      if ((e.type == SDL_MOUSEBUTTONDOWN) & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);
        for (int i = 0; i < list_size; ++i)
        {
          img_rect = get_texture_rect_wh(alien_a, positions[i].x, positions[i].y, 100, 100);
          if (SDL_HasIntersection(&mouse_rect, &img_rect))
          {
            positions[i].x = 0;
            positions[i].y = 0;

            list_size--;
          }
        }
        printf("MOUSE PRESSED\n");
      }
    }

    SDL_RenderClear(ren);

    render_scale_texture(background, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int i = 0; i < Y_TILES; ++i)
    {
      for (int j = 0; j < X_TILES; ++j)
      {
        if (map[i][j] == 1)
        {
          render_scale_texture(road, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
        else if (map[i][j] == 2)
        {
          render_scale_texture(brigde_road, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
        else if (map[i][j] == 3)
        {
          render_scale_texture(bridge, ren, 15 + j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
      }
    }

    render_scale_texture(base_a, ren, 10, 200, 170, 170);
    render_scale_texture(base_b, ren, SCREEN_WIDTH - 180, 235, 170, 170);

    //render_scale_texture(alien_a, ren, actual_pos.x, actual_pos.y, ALIEN_SIZE, ALIEN_SIZE);

    for (int i = 0; i < 3; ++i)
    {
      if (positions[i].x != 0 && positions[i].y != 0)
        render_scale_texture(alien_a, ren, positions[i].x, positions[i].y, ALIEN_SIZE, ALIEN_SIZE);
    }

    SDL_RenderPresent(ren);

    SDL_Delay(16.666667);
  }

  SDL_DestroyTexture(alien_a);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  quit_graphics();

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  pthread_join(threads[2], NULL);

  return 0;
}

int spawn_alien(void)
{
  if (list_size > 2)
  {
    return -1;
  }

  int iret1;

  int *arg = malloc(sizeof(*arg));
  *arg = list_size;

  positions[list_size].x = 1290;
  positions[list_size].y = 390;

  iret1 = pthread_create(&threads[list_size], NULL, &alien_thread, arg);

  if (iret1)
  {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
    return -1;
  }

  list_size++;
}

void *alien_thread(void *param)
{
  int index = *((int *)param);

  for (int i = 0; i < 3; ++i)
  {
    move(&positions[index], routes_b[0][i], velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&positions[index], routes_b[1][i], velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&positions[index], routes_b[4][i], velocity);
  }

  free(param);

  printf("Thread end\n");
}