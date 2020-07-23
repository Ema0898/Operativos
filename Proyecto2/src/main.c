#include <SDL2/SDL.h>
#include <lpthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <gui.h>
#include <utilities.h>
#include <structs.h>
#include <cfg.h>
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

llist *aliens_a;
llist *aliens_b;

int list_a_size = 0;
int list_b_size = 0;
int velocity = 0;
int finish = 0;

int percentages[6];

int alien_a_thread(void *param);
int alien_b_thread(void *param);
int automatic_mode_thread(void *param);

int spawn_alien(int community, int type);

int main(int argc, char *argv[])
{
  int medium = 0;
  int manual = 0;

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

  srand(time(NULL));

  aliens_a = llist_create(NULL);
  aliens_b = llist_create(NULL);

  memset(percentages, 0, 6);
  load_alien(&velocity, percentages);

  lpthread_t automatic_mode;

  if (medium != 0)
  {
    printf("Automatic Mode Selected\n");

    double *arg = malloc(sizeof(*arg));
    *arg = (float)medium / 100;

    Lthread_create(&automatic_mode, NULL, &automatic_mode_thread, arg);
  }
  else
  {
    printf("Manual Mode Selected\n");
    manual = 1;
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

  SDL_Texture *alien_a = load_texture("../assets/images/a1.png", ren);
  SDL_Texture *alien_b = load_texture("../assets/images/b1.png", ren);
  SDL_Texture *alien_a_alpha = load_texture("../assets/images/a2.png", ren);
  SDL_Texture *alien_b_alpha = load_texture("../assets/images/b2.png", ren);
  SDL_Texture *alien_a_beta = load_texture("../assets/images/a3.png", ren);
  SDL_Texture *alien_b_beta = load_texture("../assets/images/b3.png", ren);

  SDL_Texture *brigde_road = load_texture("../assets/images/road.png", ren);
  SDL_Texture *bridge = load_texture("../assets/images/bridge.png", ren);
  SDL_Texture *road = load_texture("../assets/images/asphalt.png", ren);
  SDL_Texture *background = load_texture("../assets/images/background.png", ren);
  SDL_Texture *base_a = load_texture("../assets/images/A.png", ren);
  SDL_Texture *base_b = load_texture("../assets/images/B.png", ren);

  int quit = 0;

  SDL_Event e;

  SDL_Rect mouse_rect;
  SDL_Rect img_rect;
  mouse_rect.w = mouse_rect.h = 1;

  int aliens_a_size = 0;
  int aliens_b_size = 0;

  while (!quit)
  {
    aliens_a_size = llist_get_size(aliens_a);
    aliens_b_size = llist_get_size(aliens_b);

    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit = 1;
        finish = 1;
      }

      if (e.type == SDL_KEYDOWN && manual)
      {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
          spawn_alien(0, 0);
          break;

        case SDLK_s:
          spawn_alien(0, 1);
          break;

        case SDLK_d:
          spawn_alien(0, 2);
          break;

        case SDLK_b:
          spawn_alien(1, 0);
          break;

        case SDLK_n:
          spawn_alien(1, 1);
          break;

        case SDLK_m:
          spawn_alien(1, 2);
          break;

        default:
          break;
        }
      }

      if ((e.type == SDL_MOUSEBUTTONDOWN) & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        if (aliens_a_size != 0)
        {
          SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);

          for (int i = 0; i < aliens_a_size; ++i)
          {
            alien *curr = llist_get_by_index(aliens_a, i);

            img_rect = get_texture_rect_wh(alien_a, curr->pos.x, curr->pos.y, 32, 32);

            if (SDL_HasIntersection(&mouse_rect, &img_rect))
            {
              lpthread_t *thread = curr->thread;
              Lthread_exit(thread->pid);

              llist_remove_by_index(aliens_a, i);
              list_a_size--;
              aliens_a_size--;
            }
          }
        }

        if (aliens_b_size != 0)
        {
          SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);

          for (int i = 0; i < aliens_b_size; ++i)
          {
            alien *curr = llist_get_by_index(aliens_b, i);

            img_rect = get_texture_rect_wh(alien_b, curr->pos.x, curr->pos.y, 32, 32);

            if (SDL_HasIntersection(&mouse_rect, &img_rect))
            {
              lpthread_t *thread = curr->thread;
              Lthread_exit(thread->pid);

              llist_remove_by_index(aliens_b, i);
              list_b_size--;
              aliens_b_size--;
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

    if (aliens_a_size != 0)
    {
      for (int i = 0; i < aliens_a_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens_a, i);
        switch (curr->type)
        {
        case 0:
          render_scale_texture(alien_a, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
          break;

        case 1:
          render_scale_texture(alien_a_alpha, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
          break;

        case 2:
          render_scale_texture(alien_a_beta, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
          break;

        default:
          break;
        }
      }
    }

    if (aliens_b_size != 0)
    {
      for (int i = 0; i < aliens_b_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens_b, i);
        switch (curr->type)
        {
        case 0:
          render_scale_texture(alien_b, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
          break;

        case 1:
          render_scale_texture(alien_b_alpha, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
          break;

        case 2:
          render_scale_texture(alien_b_beta, ren, curr->pos.x, curr->pos.y, ALIEN_SIZE, ALIEN_SIZE);
          break;

        default:
          break;
        }
      }
    }

    SDL_RenderPresent(ren);

    SDL_Delay(16.666667);
  }

  SDL_DestroyTexture(alien_a);
  SDL_DestroyTexture(alien_b);
  SDL_DestroyTexture(brigde_road);
  SDL_DestroyTexture(bridge);
  SDL_DestroyTexture(road);
  SDL_DestroyTexture(background);
  SDL_DestroyTexture(base_a);
  SDL_DestroyTexture(base_b);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  quit_graphics();
  quit_cfg();

  if (aliens_a_size != 0)
  {
    for (int i = 0; i < aliens_a_size; ++i)
    {
      alien *curr = llist_get_by_index(aliens_a, i);
      Lthread_join(*(curr->thread), NULL);
    }
  }

  if (aliens_b_size != 0)
  {
    for (int i = 0; i < aliens_b_size; ++i)
    {
      alien *curr = llist_get_by_index(aliens_b, i);
      Lthread_join(*(curr->thread), NULL);
    }
  }

  Lthread_join(automatic_mode, NULL);

  llist_free(aliens_a);
  llist_free(aliens_b);

  return 0;
}

int spawn_alien(int community, int type)
{
  int iret1;

  int *arg = malloc(sizeof(*arg));

  alien *entity = (alien *)malloc(sizeof(alien));
  lpthread_t *thread = (lpthread_t *)malloc(sizeof(lpthread_t));
  entity->thread = thread;
  entity->type = type;

  int percentage = (rand() % (200 - 50 + 1)) + 50;
  entity->velocity = generate_alien_velocity(entity->type, velocity, percentage);

  if (community == 0)
  {
    entity->pos.x = 90;
    entity->pos.y = 180;

    *arg = list_a_size;

    llist_insert_by_index(aliens_a, entity, list_a_size);

    iret1 = Lthread_create(entity->thread, NULL, &alien_a_thread, arg);

    list_a_size++;
  }
  else if (community == 1)
  {
    entity->pos.x = 1290;
    entity->pos.y = 390;

    *arg = list_b_size;

    llist_insert_by_index(aliens_b, entity, list_b_size);

    iret1 = Lthread_create(entity->thread, NULL, &alien_b_thread, arg);

    list_b_size++;
  }

  if (iret1)
  {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
    return -1;
  }

  return 0;
}

int alien_a_thread(void *param)
{
  int index = *((int *)param);

  int hola = generate_random(3, 1);

  alien *my_alien = llist_get_by_index(aliens_a, index);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[0][i], my_alien->velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[hola][i], my_alien->velocity);
  }

  move_bridge(&my_alien->pos, &my_alien->progress, 1);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[hola + 3][i], my_alien->velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[7][i], my_alien->velocity);
  }

  //free(param);

  llist_remove_by_index(aliens_a, index);
  list_a_size--;

  printf("Thread end\n");

  return 0;
}

int alien_b_thread(void *param)
{
  int index = *((int *)param);

  int hola = generate_random(3, 1);

  alien *my_alien = llist_get_by_index(aliens_b, index);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[0][i], my_alien->velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[hola][i], my_alien->velocity);
  }

  printf("MOVE BRIDGE LOGIC\n");

  move_bridge(&my_alien->pos, &my_alien->progress, -1);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[hola + 3][i], my_alien->velocity);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[7][i], my_alien->velocity);
  }

  //free(param);

  llist_remove_by_index(aliens_b, index);
  list_b_size--;

  printf("Thread end\n");

  return 0;
}

int automatic_mode_thread(void *param)
{
  double lambda = *((double *)param);
  double wait = ran_expo(lambda);

  int a_probability[10];
  int b_probability[10];

  for (int i = 0; i < 10; ++i)
  {
    a_probability[i] = 0;
    b_probability[i] = 0;
  }

  printf("AUTOMATIC THREAD WAITING TIME = %f\n", wait);

  generate_probability(a_probability, b_probability, percentages);

  while (!finish)
  {
    int index_a = generate_random(9, 0);
    int index_b = generate_random(9, 0);

    printf("SPAWNED A ALIEN, TYPE = %d\n", a_probability[index_a]);
    printf("SPAWNED B ALIEN, TYPE = %d\n", b_probability[index_b]);

    spawn_alien(0, a_probability[index_a]);
    spawn_alien(1, b_probability[index_b]);

    sleep(wait);
  }

  return 0;
}