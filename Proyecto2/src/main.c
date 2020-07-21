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

point routes_a[8][3];
point routes_b[8][3];

llist *aliens;

int list_size = 0;
int velocity = 0;
int finish = 0;

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

  aliens = llist_create(NULL);
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

  int aliens_size = 0;

  while (!quit)
  {
    aliens_size = llist_get_size(aliens);

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
          spawn_alien();
          break;

        default:
          break;
        }
      }

      if ((e.type == SDL_MOUSEBUTTONDOWN) & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        if (aliens_size != 0)
        {
          SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);

          for (int i = 0; i < aliens_size; ++i)
          {
            alien *curr = llist_get_by_index(aliens, i);

            img_rect = get_texture_rect_wh(alien_a, curr->pos.x, curr->pos.y, 32, 32);

            if (SDL_HasIntersection(&mouse_rect, &img_rect))
            {
              pthread_cancel(*(curr->thread));

              llist_remove_by_index(aliens, i);
              list_size--;
              aliens_size--;
            }
          }
        }
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

    render_scale_texture(alien_a, ren, 330, 120, ALIEN_SIZE, ALIEN_SIZE);

    aliens_size = llist_get_size(aliens);

    if (aliens_size != 0)
    {
      for (int i = 0; i < aliens_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens, i);
        render_scale_texture(alien_a, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
      }
    }

    SDL_RenderPresent(ren);

    SDL_Delay(16.666667);
  }

  SDL_DestroyTexture(alien_a);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  quit_graphics();

  aliens_size = llist_get_size(aliens);

  if (aliens_size != 0)
  {
    for (int i = 0; i < aliens_size; ++i)
    {
      alien *curr = llist_get_by_index(aliens, i);
      pthread_join(*(curr->thread), NULL);
    }
  }

  llist_free(aliens);

  return 0;
}

int spawn_alien(void)
{
  int iret1;

  int *arg = malloc(sizeof(*arg));
  *arg = list_size;

  alien *entity = (alien *)malloc(sizeof(alien));
  pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t));

  entity->pos.x = 1290;
  entity->pos.y = 390;
  entity->thread = thread;

  llist_insert_by_index(aliens, entity, list_size);

  iret1 = pthread_create(entity->thread, NULL, &alien_thread, arg);

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

  int hola = 3;

  alien *my_alien = llist_get_by_index(aliens, index);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[0][i], velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[hola][i], velocity);
  }

  printf("MOVE BRIDGE LOGIC\n");

  move_bridge(&my_alien->pos, &my_alien->progress);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[hola + 3][i], velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[7][i], velocity);
  }

  free(param);

  printf("Thread end\n");
}