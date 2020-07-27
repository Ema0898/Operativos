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
#include <algorithms.h>
#include <schedulers.h>

/* global variables */
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

configurable *bridge_left_conf;
configurable *bridge_right_conf;
configurable *bridge_center_conf;

bridge *bridge_struct_left;
bridge *bridge_struct_right;
bridge *bridge_struct_center;

llist *list_bridge_left;
llist *list_bridge_right;
llist *list_bridge_center;

llist *aliens_left_north;
llist *aliens_left_south;
llist *aliens_right_north;
llist *aliens_right_south;
llist *aliens_center_north;
llist *aliens_center_south;

algs_params *params_left;
algs_params *params_right;
algs_params *params_center;

int *weight_now_left;
int *weight_now_right;
int *weight_now_center;

int *aliens_count_north_left;
int *aliens_count_south_left;
int *aliens_count_north_right;
int *aliens_count_south_right;
int *aliens_count_north_center;
int *aliens_count_south_center;

alien *invader;
int invader_alive;

int list_a_size = 0;
int list_b_size = 0;
int velocity = 0;
int aliens_weight = 0;
int finish = 0;

int *turn_semaphore_left;
int *turn_semaphore_center;
int *turn_semaphore_right;

int percentages[6];

lpthread_t invader_thread_id;

int alien_a_thread(void *param);
int alien_b_thread(void *param);
int automatic_mode_thread(void *param);
int invader_thread(void *param);

int spawn_alien(int community, int type);
int spawn_invader(void);

int main(int argc, char *argv[])
{
  int medium = 0;
  int manual = 0;

  int key_pressed = 0;

  int is_semaphore_left = 0;
  int is_semaphore_center = 0;
  int is_semaphore_right = 0;

  int is_y_left = 0;
  int is_y_center = 0;
  int is_y_right = 0;

  /* validate user args and program initalization */
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

  print_instructions();

  srand(time(NULL));

  /* allocates linked lists */
  aliens_a = llist_create(NULL);
  aliens_b = llist_create(NULL);

  list_bridge_left = llist_create(NULL);
  list_bridge_right = llist_create(NULL);
  list_bridge_center = llist_create(NULL);

  aliens_left_north = llist_create(NULL);
  aliens_left_south = llist_create(NULL);
  aliens_right_north = llist_create(NULL);
  aliens_right_south = llist_create(NULL);
  aliens_center_north = llist_create(NULL);
  aliens_center_south = llist_create(NULL);

  aliens_count_north_left = (int *)malloc(sizeof(int));
  aliens_count_north_right = (int *)malloc(sizeof(int));
  aliens_count_south_left = (int *)malloc(sizeof(int));
  aliens_count_south_right = (int *)malloc(sizeof(int));
  aliens_count_north_center = (int *)malloc(sizeof(int));
  aliens_count_south_center = (int *)malloc(sizeof(int));

  *aliens_count_north_left = 0;
  *aliens_count_north_right = 0;
  *aliens_count_south_left = 0;
  *aliens_count_south_right = 0;
  *aliens_count_north_center = 0;
  *aliens_count_south_center = 0;

  /* allocates global variables */
  bridge_left_conf = (configurable *)malloc(sizeof(configurable));
  bridge_right_conf = (configurable *)malloc(sizeof(configurable));
  bridge_center_conf = (configurable *)malloc(sizeof(configurable));

  load_bridge_left(bridge_left_conf);
  load_bridge_right(bridge_right_conf);
  load_bridge_center(bridge_center_conf);

  bridge_struct_left = (bridge *)malloc(sizeof(bridge));

  bridge_struct_right = (bridge *)malloc(sizeof(bridge));

  bridge_struct_center = (bridge *)malloc(sizeof(bridge));

  /* init bridge structs variables */

  /* left bridge */
  bridge_struct_left->weight = bridge_left_conf->weight_confg;
  bridge_struct_left->length = bridge_left_conf->length_confg;
  bridge_struct_left->bridge_type = bridge_left_conf->scheduler_confg;
  bridge_struct_left->quantum = bridge_left_conf->quantum_confg;
  bridge_struct_left->amount_north = bridge_left_conf->amount_north_confg;
  bridge_struct_left->amount_south = bridge_left_conf->amount_south_confg;

  weight_now_left = (int *)malloc(sizeof(int));
  *weight_now_left = 0;

  turn_semaphore_left = (int *)malloc(sizeof(int));
  *turn_semaphore_left = 0;

  params_left = (algs_params *)malloc(sizeof(algs_params));
  params_left->north = aliens_left_north;
  params_left->south = aliens_left_south;
  params_left->bridge = list_bridge_left;
  params_left->amount_to_pass = bridge_left_conf->y_algorithm_confg;
  params_left->bridge_weight = bridge_left_conf->weight_confg;
  params_left->weight_now = weight_now_left;
  params_left->bridge_struct = bridge_struct_left;
  params_left->north_time = bridge_left_conf->semaphore_north_confg;
  params_left->south_time = bridge_left_conf->semaphore_south_confg;
  params_left->turn_semaphore = turn_semaphore_left;
  params_left->aliens_count_north = aliens_count_north_left;
  params_left->aliens_count_south = aliens_count_south_left;

  /* right bridge */
  bridge_struct_right->weight = bridge_right_conf->weight_confg;
  bridge_struct_right->length = bridge_right_conf->length_confg;
  bridge_struct_right->bridge_type = bridge_right_conf->scheduler_confg;
  bridge_struct_right->quantum = bridge_right_conf->quantum_confg;
  bridge_struct_right->amount_north = bridge_right_conf->amount_north_confg;
  bridge_struct_right->amount_south = bridge_right_conf->amount_south_confg;

  weight_now_right = (int *)malloc(sizeof(int));
  *weight_now_right = 0;

  turn_semaphore_right = (int *)malloc(sizeof(int));
  *turn_semaphore_right = 0;

  params_right = (algs_params *)malloc(sizeof(algs_params));
  params_right->north = aliens_right_north;
  params_right->south = aliens_right_south;
  params_right->bridge = list_bridge_right;
  params_right->amount_to_pass = bridge_right_conf->y_algorithm_confg;
  params_right->bridge_weight = bridge_right_conf->weight_confg;
  params_right->weight_now = weight_now_right;
  params_right->bridge_struct = bridge_struct_right;
  params_right->north_time = bridge_right_conf->semaphore_north_confg;
  params_right->south_time = bridge_right_conf->semaphore_south_confg;
  params_right->turn_semaphore = turn_semaphore_right;
  params_right->aliens_count_north = aliens_count_north_right;
  params_right->aliens_count_south = aliens_count_south_right;

  /* center bridge */
  bridge_struct_center->weight = bridge_center_conf->weight_confg;
  bridge_struct_center->length = bridge_center_conf->length_confg;
  bridge_struct_center->bridge_type = bridge_center_conf->scheduler_confg;
  bridge_struct_center->quantum = bridge_center_conf->quantum_confg;
  bridge_struct_center->amount_north = bridge_center_conf->amount_north_confg;
  bridge_struct_center->amount_south = bridge_center_conf->amount_south_confg;

  weight_now_center = (int *)malloc(sizeof(int));
  *weight_now_center = 0;

  turn_semaphore_center = (int *)malloc(sizeof(int));
  *turn_semaphore_center = 0;

  params_center = (algs_params *)malloc(sizeof(algs_params));
  params_center->north = aliens_center_north;
  params_center->south = aliens_center_south;
  params_center->bridge = list_bridge_center;
  params_center->amount_to_pass = bridge_center_conf->y_algorithm_confg;
  params_center->bridge_weight = bridge_center_conf->weight_confg;
  params_center->weight_now = weight_now_center;
  params_center->bridge_struct = bridge_struct_center;
  params_center->north_time = bridge_center_conf->semaphore_north_confg;
  params_center->south_time = bridge_center_conf->semaphore_south_confg;
  params_center->turn_semaphore = turn_semaphore_center;
  params_center->aliens_count_north = aliens_count_north_center;
  params_center->aliens_count_south = aliens_count_south_center;

  lpthread_t algorithm_left, algorithm_right, algorithm_center;

  /* algorithm left bridge */
  if (bridge_left_conf->algorithm_confg == Y_ALGORITHM)
  {
    Lthread_create(&algorithm_left, NULL, &Y_algorithm, params_left);
    is_y_left = 1;
  }
  else if (bridge_left_conf->algorithm_confg == SEMAPHORE_ALGORITM)
  {
    Lthread_create(&algorithm_left, NULL, &semaphore_algorithm, params_left);
    is_semaphore_left = 1;
  }
  else
  {
    Lthread_create(&algorithm_left, NULL, &survival_algorithm, params_left);
  }

  /* algorithm right bridge */
  if (bridge_right_conf->algorithm_confg == Y_ALGORITHM)
  {
    Lthread_create(&algorithm_right, NULL, &Y_algorithm, params_right);
    is_y_right = 1;
  }
  else if (bridge_right_conf->algorithm_confg == SEMAPHORE_ALGORITM)
  {
    Lthread_create(&algorithm_right, NULL, &semaphore_algorithm, params_right);
    is_semaphore_right = 1;
  }
  else
  {
    Lthread_create(&algorithm_right, NULL, &survival_algorithm, params_right);
  }

  /* algorithm center bridge */
  if (bridge_center_conf->algorithm_confg == Y_ALGORITHM)
  {
    Lthread_create(&algorithm_center, NULL, &Y_algorithm, params_center);
    is_y_center = 1;
  }
  else if (bridge_center_conf->algorithm_confg == SEMAPHORE_ALGORITM)
  {
    Lthread_create(&algorithm_center, NULL, &semaphore_algorithm, params_center);
    is_semaphore_center = 1;
  }
  else
  {
    Lthread_create(&algorithm_center, NULL, &survival_algorithm, params_center);
  }

  invader_alive = 0;

  memset(percentages, 0, 6);
  load_alien(&velocity, percentages, &aliens_weight);

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

  /* init graphic windows */
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

  /* images textures */
  SDL_Texture *alien_a = load_texture("../assets/images/a1.png", ren);
  SDL_Texture *alien_b = load_texture("../assets/images/b1.png", ren);
  SDL_Texture *alien_a_alpha = load_texture("../assets/images/a2.png", ren);
  SDL_Texture *alien_b_alpha = load_texture("../assets/images/b2.png", ren);
  SDL_Texture *alien_a_beta = load_texture("../assets/images/a3.png", ren);
  SDL_Texture *alien_b_beta = load_texture("../assets/images/b3.png", ren);
  SDL_Texture *invader_img = load_texture("../assets/images/invader.png", ren);

  SDL_Texture *brigde_road = load_texture("../assets/images/road.png", ren);
  SDL_Texture *bridge = load_texture("../assets/images/bridge.png", ren);
  SDL_Texture *road = load_texture("../assets/images/asphalt.png", ren);
  SDL_Texture *background = load_texture("../assets/images/background.png", ren);
  SDL_Texture *base_a = load_texture("../assets/images/A.png", ren);
  SDL_Texture *base_b = load_texture("../assets/images/B.png", ren);

  SDL_Texture *semaphore_red = load_texture("../assets/images/semaphore_red.png", ren);
  SDL_Texture *semaphore_green = load_texture("../assets/images/semaphore_green.png", ren);

  SDL_Texture *text_left_north;
  SDL_Texture *text_left_south;
  SDL_Texture *text_right_north;
  SDL_Texture *text_right_south;
  SDL_Texture *text_center_north;
  SDL_Texture *text_center_south;

  int quit = 0;

  SDL_Event e;

  SDL_Rect mouse_rect;
  SDL_Rect img_rect;
  mouse_rect.w = mouse_rect.h = 1;

  SDL_Color color = {255, 255, 255, 255};

  int aliens_a_size = 0;
  int aliens_b_size = 0;
  int alien_mouse_pos = 0;

  char tmp[10];
  char tmp1[10];
  char tmp2[10];
  char tmp3[10];
  char tmp4[10];
  char tmp5[10];

  /* main loop */
  while (!quit)
  {
    aliens_a_size = llist_get_size(aliens_a);
    aliens_b_size = llist_get_size(aliens_b);

    /* checks for events */
    while (SDL_PollEvent(&e))
    {
      /* quit event */
      if (e.type == SDL_QUIT)
      {
        quit = 1;
        finish = 1;
      }

      /* key pressed event */
      if (e.type == SDL_KEYDOWN && manual)
      {
        key_pressed++;
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

      if (e.type == SDL_KEYDOWN)
      {
        key_pressed++;
        switch (e.key.keysym.sym)
        {
        case SDLK_x:
          spawn_invader();
          break;

        default:
          break;
        }
      }

      /* mouse pressed event */
      if ((e.type == SDL_MOUSEBUTTONDOWN) & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);

        if (aliens_a_size != 0)
        {
          for (int i = 0; i < aliens_a_size; ++i)
          {
            alien *curr = llist_get_by_index(aliens_a, i);

            if (curr == NULL)
            {
              printf("BREAKING LOOP\n");
              break;
            }

            img_rect = get_texture_rect_wh(alien_a, curr->pos.x, curr->pos.y, 32, 32);

            if (SDL_HasIntersection(&mouse_rect, &img_rect))
            {
              lpthread_t *thread = curr->thread;
              Lthread_exit(thread->pid);

              llist_remove_by_index(aliens_a, i);
              list_a_size--;
              aliens_a_size--;
              alien_mouse_pos = llist_get_alien_index(aliens_left_north, curr->id);
              if (alien_mouse_pos != -1)
              {
                llist_remove_by_index(aliens_left_north, alien_mouse_pos);
              }
              else
              {
                alien_mouse_pos = llist_get_alien_index(aliens_center_north, curr->id);
                if (alien_mouse_pos != -1)
                {
                  llist_remove_by_index(aliens_center_north, alien_mouse_pos);
                }
                else
                {
                  alien_mouse_pos = llist_get_alien_index(aliens_right_north, curr->id);
                  if (alien_mouse_pos != -1)
                  {
                    llist_remove_by_index(aliens_right_north, alien_mouse_pos);
                  }
                  else
                  {
                    alien_mouse_pos = llist_get_alien_index(list_bridge_left, curr->id);
                    if (alien_mouse_pos != -1)
                    {
                      *weight_now_left -= curr->weight;
                      llist_remove_by_index(list_bridge_left, alien_mouse_pos);
                    }
                    else
                    {
                      alien_mouse_pos = llist_get_alien_index(list_bridge_center, curr->id);
                      if (alien_mouse_pos != -1)
                      {
                        *weight_now_center -= curr->weight;
                        llist_remove_by_index(list_bridge_center, alien_mouse_pos);
                      }
                      else
                      {
                        alien_mouse_pos = llist_get_alien_index(list_bridge_right, curr->id);
                        if (alien_mouse_pos != -1)
                        {
                          *weight_now_right -= curr->weight;
                          llist_remove_by_index(list_bridge_right, alien_mouse_pos);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

        if (aliens_b_size != 0)
        {
          for (int i = 0; i < aliens_b_size; ++i)
          {
            alien *curr = llist_get_by_index(aliens_b, i);

            if (curr == NULL)
            {
              printf("BREAKING LOOP\n");
              break;
            }

            img_rect = get_texture_rect_wh(alien_b, curr->pos.x, curr->pos.y, 32, 32);

            if (SDL_HasIntersection(&mouse_rect, &img_rect))
            {
              lpthread_t *thread = curr->thread;
              Lthread_exit(thread->pid);

              llist_remove_by_index(aliens_b, i);
              list_b_size--;
              aliens_b_size--;

              alien_mouse_pos = llist_get_alien_index(aliens_left_south, curr->id);
              if (alien_mouse_pos != -1)
              {
                llist_remove_by_index(aliens_left_south, alien_mouse_pos);
              }
              else
              {
                alien_mouse_pos = llist_get_alien_index(aliens_center_south, curr->id);
                if (alien_mouse_pos != -1)
                {
                  llist_remove_by_index(aliens_center_south, alien_mouse_pos);
                }
                else
                {
                  alien_mouse_pos = llist_get_alien_index(aliens_right_south, curr->id);
                  if (alien_mouse_pos != -1)
                  {
                    llist_remove_by_index(aliens_right_south, alien_mouse_pos);
                  }
                  else
                  {
                    alien_mouse_pos = llist_get_alien_index(list_bridge_left, curr->id);
                    if (alien_mouse_pos != -1)
                    {
                      *weight_now_left -= curr->weight;
                      llist_remove_by_index(list_bridge_left, alien_mouse_pos);
                    }
                    else
                    {
                      alien_mouse_pos = llist_get_alien_index(list_bridge_center, curr->id);
                      if (alien_mouse_pos != -1)
                      {
                        *weight_now_center -= curr->weight;
                        llist_remove_by_index(list_bridge_center, alien_mouse_pos);
                      }
                      else
                      {
                        alien_mouse_pos = llist_get_alien_index(list_bridge_right, curr->id);
                        if (alien_mouse_pos != -1)
                        {
                          *weight_now_right -= curr->weight;
                          llist_remove_by_index(list_bridge_right, alien_mouse_pos);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

        if (invader_alive)
        {
          img_rect = get_texture_rect_wh(invader_img, invader->pos.x, invader->pos.y, 32, 32);

          if (SDL_HasIntersection(&mouse_rect, &img_rect))
          {
            invader_alive = 0;
            free(invader);
          }
        }
      }
    }
    /* render textures */
    SDL_RenderClear(ren);

    /* render background */
    render_scale_texture(background, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    /*render map tiles */
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

    /*render aliens communities */
    render_scale_texture(base_a, ren, 10, 200, 170, 170);
    render_scale_texture(base_b, ren, SCREEN_WIDTH - 180, 235, 170, 170);

    aliens_a_size = llist_get_size(aliens_a);

    /* render a aliens */
    if (aliens_a_size != 0)
    {
      for (int i = 0; i < aliens_a_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens_a, i);
        if (curr == NULL)
        {
          printf("BREAKING LOOP\n");
          break;
        }
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

        aliens_a_size = llist_get_size(aliens_a);
      }
    }

    aliens_b_size = llist_get_size(aliens_b);

    /* render b aliens */
    if (aliens_b_size != 0)
    {
      for (int i = 0; i < aliens_b_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens_b, i);
        if (curr == NULL)
        {
          printf("BREAKING LOOP\n");
          break;
        }
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
        aliens_b_size = llist_get_size(aliens_b);
      }
    }

    /* render invader alien */
    if (invader_alive)
    {
      render_scale_texture(invader_img, ren, invader->pos.x, invader->pos.y, ALIEN_SIZE, ALIEN_SIZE);
    }

    /* render semaphore algoritm info if neccesary */
    if (is_semaphore_left)
    {
      render_scale_texture(*turn_semaphore_left == 0 ? semaphore_green : semaphore_red, ren, 300, 300, 20, 20);
      render_scale_texture(*turn_semaphore_left == 1 ? semaphore_green : semaphore_red, ren, 300, 370, 20, 20);
    }

    if (is_semaphore_center)
    {
      render_scale_texture(*turn_semaphore_center == 0 ? semaphore_green : semaphore_red, ren, 630, 300, 20, 20);
      render_scale_texture(*turn_semaphore_center == 1 ? semaphore_green : semaphore_red, ren, 630, 370, 20, 20);
    }

    if (is_semaphore_right)
    {
      render_scale_texture(*turn_semaphore_right == 0 ? semaphore_green : semaphore_red, ren, 960, 300, 20, 20);
      render_scale_texture(*turn_semaphore_right == 1 ? semaphore_green : semaphore_red, ren, 960, 370, 20, 20);
    }

    /* render y algoritm info if neccesary */
    if (is_y_left)
    {
      SDL_itoa(*aliens_count_north_left, tmp, 10);

      text_left_north = render_text(tmp, "../assets/fonts/font.ttf", color, 12, ren);
      render_full_texture(text_left_north, ren, 300, 300);

      SDL_itoa(*aliens_count_south_left, tmp1, 10);

      text_left_south = render_text(tmp1, "../assets/fonts/font.ttf", color, 12, ren);
      render_full_texture(text_left_south, ren, 300, 370);
    }

    if (is_y_right)
    {
      SDL_itoa(*aliens_count_north_right, tmp2, 10);

      text_right_north = render_text(tmp2, "../assets/fonts/font.ttf", color, 12, ren);
      render_full_texture(text_right_north, ren, 960, 300);

      SDL_itoa(*aliens_count_south_right, tmp3, 10);

      text_right_south = render_text(tmp3, "../assets/fonts/font.ttf", color, 12, ren);
      render_full_texture(text_right_south, ren, 960, 370);
    }

    if (is_y_center)
    {
      SDL_itoa(*aliens_count_north_center, tmp4, 10);

      text_center_north = render_text(tmp4, "../assets/fonts/font.ttf", color, 12, ren);
      render_full_texture(text_center_north, ren, 630, 300);

      SDL_itoa(*aliens_count_south_center, tmp5, 10);

      text_center_south = render_text(tmp5, "../assets/fonts/font.ttf", color, 12, ren);
      render_full_texture(text_center_south, ren, 630, 370);
    }

    memset(tmp, 0, 10);
    memset(tmp1, 0, 10);
    memset(tmp2, 0, 10);
    memset(tmp3, 0, 10);
    memset(tmp4, 0, 10);
    memset(tmp5, 0, 10);

    //render_scale_texture(invader_img, ren, 630, 120, 150, 50);

    SDL_RenderPresent(ren);

    printf("KEY PRESSED =  %d\n", key_pressed);

    SDL_Delay(16.666667 * 2);
  }

  /* free textures */
  SDL_DestroyTexture(alien_a);
  SDL_DestroyTexture(alien_b);
  SDL_DestroyTexture(brigde_road);
  SDL_DestroyTexture(bridge);
  SDL_DestroyTexture(road);
  SDL_DestroyTexture(background);
  SDL_DestroyTexture(base_a);
  SDL_DestroyTexture(base_b);
  SDL_DestroyTexture(semaphore_red);
  SDL_DestroyTexture(semaphore_green);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  quit_graphics();
  quit_cfg();

  // aliens_a_size = llist_get_size(aliens_a);

  // if (aliens_a_size != 0)
  // {
  //   for (int i = 0; i < aliens_a_size; ++i)
  //   {
  //     alien *curr = llist_get_by_index(aliens_a, i);
  //     if (curr == NULL)
  //     {
  //       printf("BREAKING LOOP\n");
  //       break;
  //     }
  //     lpthread_t *thread = curr->thread;
  //     Lthread_exit(thread->pid);
  //     //Lthread_join(*(curr->thread), NULL);
  //     free(thread);
  //     free(curr->lottery_numbers);

  //     aliens_a_size = llist_get_size(aliens_a);
  //   }
  // }

  // printf("ALIENS A JOINED\n");

  // if (aliens_b_size != 0)
  // {
  //   for (int i = 0; i < aliens_b_size; ++i)
  //   {
  //     alien *curr = llist_get_by_index(aliens_b, i);
  //     if (curr == NULL)
  //     {
  //       printf("BREAKING LOOP\n");
  //       break;
  //     }
  //     lpthread_t *thread = curr->thread;
  //     Lthread_exit(thread->pid);
  //     //Lthread_join(*(curr->thread), NULL);
  //     free(thread);
  //     free(curr->lottery_numbers);

  //     aliens_b_size = llist_get_size(aliens_b);
  //   }
  // }

  // printf("ALIENS B JOINED\n");

  /* Free Memory */
  free(bridge_left_conf);
  free(bridge_right_conf);
  free(bridge_center_conf);

  free(bridge_struct_left);
  free(bridge_struct_right);
  free(bridge_struct_center);

  // free(weight_now_left);
  // free(weight_now_right);
  // free(weight_now_center);

  free(params_left);
  free(params_right);
  free(params_center);

  free(aliens_count_north_left);
  free(aliens_count_south_left);
  free(aliens_count_north_right);
  free(aliens_count_south_right);
  free(aliens_count_north_center);
  free(aliens_count_south_center);

  // /* JOIN THREADS */
  Lthread_exit(algorithm_left.pid);
  Lthread_exit(algorithm_right.pid);
  Lthread_exit(algorithm_center.pid);
  printf("ALGORITHMS THREADS EXIT\n");

  Lthread_exit(invader_thread_id.pid);
  printf("INVADER THREAD EXIT\n");

  // llist_free(list_bridge_left);
  // llist_free(list_bridge_center);
  // llist_free(list_bridge_right);

  // llist_free(aliens_left_north);
  // llist_free(aliens_left_south);
  // llist_free(aliens_right_north);
  // llist_free(aliens_right_south);
  // llist_free(aliens_center_north);
  // llist_free(aliens_center_south);

  // printf("LISTS FREE\n");

  // Lthread_exit(automatic_mode.pid);
  // printf("AUTOMATIC MODE JOINED\n");

  // llist_free(aliens_a);
  // llist_free(aliens_b);

  // printf("ALIEN LISTS FREE\n");

  return 0;
}

/* spawn alien function */
int spawn_alien(int community, int type)
{
  int iret1;

  int *arg = malloc(sizeof(*arg));

  alien *entity = (alien *)malloc(sizeof(alien));
  lpthread_t *thread = (lpthread_t *)malloc(sizeof(lpthread_t));

  entity->thread = thread;
  entity->type = type;
  gettimeofday(&entity->work_init_time, NULL);
  entity->last_update = entity->work_init_time;
  entity->progress = 0;
  entity->duration = 10;
  entity->working = 0;
  entity->accumulator = 0;
  entity->weight = aliens_weight;
  entity->rr_quantum = 0;

  if (entity->type == 0)
  {
    entity->priority = 1;
  }
  else if (entity->type == 1)
  {
    entity->priority = 1;
  }
  else if (entity->type == 2)
  {
    entity->priority = 0;
  }

  int percentage = (rand() % (200 - 50 + 1)) + 50;
  entity->velocity = generate_alien_velocity(entity->type, velocity, percentage);

  if (community == 0)
  {
    entity->pos.x = 90;
    entity->pos.y = 180;

    *arg = list_a_size;

    llist_insert_by_index(aliens_a, entity, list_a_size);

    iret1 = Lthread_create(entity->thread, NULL, &alien_a_thread, entity);

    list_a_size++;
  }
  else if (community == 1)
  {
    entity->pos.x = 1290;
    entity->pos.y = 390;

    *arg = list_b_size;

    llist_insert_by_index(aliens_b, entity, list_b_size);

    iret1 = Lthread_create(entity->thread, NULL, &alien_b_thread, entity);

    list_b_size++;
  }

  if (iret1)
  {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
    return -1;
  }

  return 0;
}

/* spawn invader function */
int spawn_invader(void)
{
  if (invader_alive == 0)
  {
    invader_alive = 1;
    invader = (alien *)malloc(sizeof(alien));

    Lthread_create(&invader_thread_id, NULL, &invader_thread, NULL);
  }
}

/* alien a thread */
int alien_a_thread(void *param)
{
  int bridge_decision = generate_random(3, 1);

  alien *my_alien = (alien *)param;

  int id = my_alien->thread->pid;
  my_alien->id = id;

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[0][i], my_alien->velocity, aliens_a, my_alien->id, 0, &my_alien->working, 0);
  }

  int can_move = 0;

  while (!can_move)
  {
    if (bridge_decision == 1)
    {
      if (llist_get_size(aliens_left_north) < bridge_struct_left->amount_north)
      {
        can_move = 1;
      }
      else
      {
        bridge_decision = generate_random(3, 1);
      }
    }
    else if (bridge_decision == 2)
    {
      if (llist_get_size(aliens_right_north) < bridge_struct_right->amount_north)
      {
        can_move = 1;
      }
      else
      {
        bridge_decision = generate_random(3, 1);
      }
    }
    else
    {
      if (llist_get_size(aliens_center_north) < bridge_struct_center->amount_north)
      {
        can_move = 1;
      }
      else
      {
        bridge_decision = generate_random(3, 1);
      }
    }
  }

  /* PINK PART */
  if (bridge_decision == 1)
  {
    /* left */
    my_alien->duration = bridge_struct_left->length / my_alien->velocity;
    if (bridge_struct_left->bridge_type == ROUND_ROBIN)
    {
      FIFO_scheduler(aliens_left_north, (void *)my_alien);
    }
    else if (bridge_struct_left->bridge_type == PRIORITY)
    {
      priority_scheduler(aliens_left_north, (void *)my_alien);
    }
    else if (bridge_struct_left->bridge_type == FIFO)
    {
      FIFO_scheduler(aliens_left_north, (void *)my_alien);
    }
    else if (bridge_struct_left->bridge_type == SJF)
    {
      SJF_scheduler(aliens_left_north, (void *)my_alien);
    }
    else
    {
      lottery_scheduler(aliens_left_north, (void *)my_alien);
    }
  }
  else if (bridge_decision == 2)
  {
    /* right */
    my_alien->duration = bridge_struct_right->length / my_alien->velocity;
    if (bridge_struct_right->bridge_type == ROUND_ROBIN)
    {
      FIFO_scheduler(aliens_right_north, (void *)my_alien);
    }
    else if (bridge_struct_right->bridge_type == PRIORITY)
    {
      priority_scheduler(aliens_right_north, (void *)my_alien);
    }
    else if (bridge_struct_right->bridge_type == FIFO)
    {
      FIFO_scheduler(aliens_right_north, (void *)my_alien);
    }
    else if (bridge_struct_right->bridge_type == SJF)
    {
      SJF_scheduler(aliens_right_north, (void *)my_alien);
    }
    else
    {
      lottery_scheduler(aliens_right_north, (void *)my_alien);
    }
  }
  else
  {
    /* center */
    my_alien->duration = bridge_struct_center->length / my_alien->velocity;
    if (bridge_struct_center->bridge_type == ROUND_ROBIN)
    {
      FIFO_scheduler(aliens_center_north, (void *)my_alien);
    }
    else if (bridge_struct_center->bridge_type == PRIORITY)
    {
      priority_scheduler(aliens_center_north, (void *)my_alien);
    }
    else if (bridge_struct_center->bridge_type == FIFO)
    {
      FIFO_scheduler(aliens_center_north, (void *)my_alien);
    }
    else if (bridge_struct_center->bridge_type == SJF)
    {
      SJF_scheduler(aliens_center_north, (void *)my_alien);
    }
    else
    {
      lottery_scheduler(aliens_center_north, (void *)my_alien);
    }
  }

init_bridge:

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[bridge_decision][i], my_alien->velocity, aliens_a, my_alien->id, 0, &my_alien->working, 0);
  }

  /* bridge start */

  int bridge_result = move_bridge(&my_alien->pos, &my_alien->progress, 1, aliens_a, my_alien->id, 0, &my_alien->rr_quantum, &my_alien->working);

  if (bridge_result)
  {
    my_alien->pos.y -= 240;
    if (bridge_decision == 2)
    {
      my_alien->pos.y = 150;
      my_alien->pos.x = 660;
    }
    my_alien->rr_quantum = 0;
    goto init_bridge;
  }

  /* bridge end */

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[bridge_decision + 3][i], my_alien->velocity, aliens_a, my_alien->id, 0, &my_alien->working, 1);
  }

  /* pink end */

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[7][i], my_alien->velocity, aliens_a, my_alien->id, 0, &my_alien->working, 0);
  }

  int curr_index = llist_get_alien_index(aliens_a, my_alien->id);

  llist_remove_by_index(aliens_a, curr_index);
  list_a_size--;

  return 0;
}

/* aliens b thread */
int alien_b_thread(void *param)
{

  int bridge_decision = generate_random(3, 1);

  alien *my_alien = (alien *)param;

  int id = my_alien->thread->pid;
  my_alien->id = id;

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[0][i], my_alien->velocity, aliens_b, my_alien->id, 1, &my_alien->working, 0);
  }

  int can_move = 0;

  while (!can_move)
  {
    if (bridge_decision == 1)
    {
      if (llist_get_size(aliens_left_south) < bridge_struct_left->amount_south)
      {
        can_move = 1;
      }
      else
      {
        bridge_decision = generate_random(3, 1);
      }
    }
    else if (bridge_decision == 2)
    {
      if (llist_get_size(aliens_right_south) < bridge_struct_right->amount_south)
      {
        can_move = 1;
      }
      else
      {
        bridge_decision = generate_random(3, 1);
      }
    }
    else
    {
      if (llist_get_size(aliens_center_south) < bridge_struct_center->amount_south)
      {
        can_move = 1;
      }
      else
      {
        bridge_decision = generate_random(3, 1);
      }
    }
  }

  /* PINK PART */
  if (bridge_decision == 1)
  {
    /* left */
    my_alien->duration = bridge_struct_left->length / my_alien->velocity;

    if (bridge_struct_left->bridge_type == ROUND_ROBIN)
    {
      FIFO_scheduler(aliens_left_south, (void *)my_alien);
    }
    else if (bridge_struct_left->bridge_type == PRIORITY)
    {
      priority_scheduler(aliens_left_south, (void *)my_alien);
    }
    else if (bridge_struct_left->bridge_type == FIFO)
    {
      FIFO_scheduler(aliens_left_south, (void *)my_alien);
    }
    else if (bridge_struct_left->bridge_type == SJF)
    {
      SJF_scheduler(aliens_left_south, (void *)my_alien);
    }
    else
    {
      lottery_scheduler(aliens_left_south, (void *)my_alien);
    }
  }
  else if (bridge_decision == 2)
  {
    /* right */
    my_alien->duration = bridge_struct_right->length / my_alien->velocity;
    if (bridge_struct_right->bridge_type == ROUND_ROBIN)
    {
      FIFO_scheduler(aliens_right_south, (void *)my_alien);
    }
    else if (bridge_struct_right->bridge_type == PRIORITY)
    {
      priority_scheduler(aliens_right_south, (void *)my_alien);
    }
    else if (bridge_struct_right->bridge_type == FIFO)
    {
      FIFO_scheduler(aliens_right_south, (void *)my_alien);
    }
    else if (bridge_struct_right->bridge_type == SJF)
    {
      SJF_scheduler(aliens_right_south, (void *)my_alien);
    }
    else
    {
      lottery_scheduler(aliens_right_south, (void *)my_alien);
    }
  }
  else
  {
    /* center */
    my_alien->duration = bridge_struct_center->length / my_alien->velocity;
    if (bridge_struct_center->bridge_type == ROUND_ROBIN)
    {
      FIFO_scheduler(aliens_center_south, (void *)my_alien);
    }
    else if (bridge_struct_center->bridge_type == PRIORITY)
    {
      priority_scheduler(aliens_center_south, (void *)my_alien);
    }
    else if (bridge_struct_center->bridge_type == FIFO)
    {
      FIFO_scheduler(aliens_center_south, (void *)my_alien);
    }
    else if (bridge_struct_center->bridge_type == SJF)
    {
      SJF_scheduler(aliens_center_south, (void *)my_alien);
    }
    else
    {
      lottery_scheduler(aliens_center_south, (void *)my_alien);
    }
  }

init_bridge2:

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[bridge_decision][i], my_alien->velocity, aliens_b, my_alien->id, 1, &my_alien->working, 0);
  }

  int bridge_result2 = move_bridge(&my_alien->pos, &my_alien->progress, -1, aliens_b, my_alien->id, 1, &my_alien->rr_quantum, &my_alien->working);

  if (bridge_result2)
  {
    my_alien->pos.y += 240;
    if (bridge_decision == 1)
    {
      my_alien->pos.y = 500;
      my_alien->pos.x = 600;
    }
    my_alien->rr_quantum = 0;
    goto init_bridge2;
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[bridge_decision + 3][i], my_alien->velocity, aliens_b, my_alien->id, 1, &my_alien->working, 1);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[7][i], my_alien->velocity, aliens_b, my_alien->id, 1, &my_alien->working, 0);
  }

  int curr_index = llist_get_alien_index(aliens_b, my_alien->id);

  llist_remove_by_index(aliens_b, curr_index);
  list_b_size--;

  return 0;
}

/* automatic mode thread */
int automatic_mode_thread(void *param)
{
  double lambda = *((double *)param);
  double wait = ran_expo(lambda);

  if (wait < 1)
  {
    wait = 3;
  }

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

/* invader thread */
int invader_thread(void *param)
{
  int a_b = generate_random(1, 0);
  point route[8][3];

  if (a_b == 0)
  {
    memcpy(route, routes_a, sizeof(route));
    invader->pos.x = 630;
    invader->pos.y = 120;
  }
  else
  {
    memcpy(route, routes_b, sizeof(route));
    invader->pos.x = 630;
    invader->pos.y = 530;
  }

  int rand_route = generate_random(3, 1);

  for (int i = 0; i < 3; ++i)
  {
    move_invader(&invader->pos, route[rand_route][i], velocity * 2, aliens_a, aliens_b, list_bridge_left, list_bridge_right, list_bridge_center,
                 aliens_left_north, aliens_left_south, aliens_right_north, aliens_right_south, aliens_center_north, aliens_center_south, &list_a_size, &list_b_size,
                 weight_now_left, weight_now_right, weight_now_center);
  }

  for (int i = 0; i < 3; ++i)
  {
    move_invader(&invader->pos, route[rand_route + 3][i], velocity * 2, aliens_a, aliens_b, list_bridge_left, list_bridge_right, list_bridge_center,
                 aliens_left_north, aliens_left_south, aliens_right_north, aliens_right_south, aliens_center_north, aliens_center_south, &list_a_size, &list_b_size,
                 weight_now_left, weight_now_right, weight_now_center);
  }

  for (int i = 0; i < 3; ++i)
  {
    move_invader(&invader->pos, route[7][i], velocity * 2, aliens_a, aliens_b, list_bridge_left, list_bridge_right, list_bridge_center,
                 aliens_left_north, aliens_left_south, aliens_right_north, aliens_right_south, aliens_center_north, aliens_center_south, &list_a_size, &list_b_size,
                 weight_now_left, weight_now_right, weight_now_center);
  }

  invader_alive = 0;
}