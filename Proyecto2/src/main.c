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

alien *invader;
int invader_alive;

int list_a_size = 0;
int list_b_size = 0;
int velocity = 0;
int finish = 0;

int percentages[6];

// lpthread_mutex_t lock_a;
// lpthread_mutex_t lock_b;

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

  // if (Lmutex_init(&lock_a, NULL) != 0)
  // {
  //   printf("\n Mutex for aliens A init has failed\n");
  //   return 1;
  // }

  // if (Lmutex_init(&lock_b, NULL) != 0)
  // {
  //   printf("\n Mutex for aliens B init has failed\n");
  //   return 1;
  // }

  srand(time(NULL));

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

  bridge_struct_left = (bridge *)malloc(sizeof(bridge));

  bridge_struct_right = (bridge *)malloc(sizeof(bridge));

  bridge_struct_center = (bridge *)malloc(sizeof(bridge));

  bridge *test = (bridge *)malloc(sizeof(bridge));
  load_bridge(test);

  printf("WEIGHT = %d\n", test->weight);
  printf("LENGTH = %d\n", test->length);
  printf("QUANTUM = %d\n", test->quantum);
  printf("TYPE = %d\n", test->bridge_type);

  bridge_struct_left->weight = 20;
  bridge_struct_left->length = 50;
  bridge_struct_left->bridge_type = ROUND_ROBIN;
  bridge_struct_left->quantum = 30;

  weight_now_left = (int *)malloc(sizeof(int));
  *weight_now_left = 0;

  params_left = (algs_params *)malloc(sizeof(algs_params));
  params_left->north = aliens_left_north;
  params_left->south = aliens_left_south;
  params_left->bridge = list_bridge_left;
  params_left->amount_to_pass = 5;
  params_left->bridge_weight = 50;
  params_left->weight_now = weight_now_left;
  params_left->bridge_struct = bridge_struct_left;

  lpthread_t algorithms;

  Lthread_create(&algorithms, NULL, &semaphore_algorithm, params_left);

  invader_alive = 0;

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
  SDL_Texture *invader_img = load_texture("../assets/images/invader.png", ren);

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

        case SDLK_x:
          spawn_invader();
          break;

        default:
          break;
        }
      }

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
              printf("INDEX OUT OF RANGE ERROR. BREAKING LOOP\n");
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
              printf("INDEX OUT OF RANGE ERROR. BREAKING LOOP\n");
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

    aliens_a_size = llist_get_size(aliens_a);

    if (aliens_a_size != 0)
    {
      for (int i = 0; i < aliens_a_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens_a, i);
        if (curr == NULL)
        {
          printf("INDEX OUT OF RANGE ERROR. BREAKING LOOP\n");
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
      }
    }

    aliens_b_size = llist_get_size(aliens_b);

    if (aliens_b_size != 0)
    {
      for (int i = 0; i < aliens_b_size; ++i)
      {
        alien *curr = llist_get_by_index(aliens_b, i);
        if (curr == NULL)
        {
          printf("INDEX OUT OF RANGE ERROR. BREAKING LOOP\n");
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
      }
    }

    if (invader_alive)
    {
      render_scale_texture(invader_img, ren, invader->pos.x, invader->pos.y, ALIEN_SIZE, ALIEN_SIZE);
    }

    SDL_RenderPresent(ren);

    SDL_Delay(16.666667 * 2);
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

  aliens_a_size = llist_get_size(aliens_a);
  aliens_b_size = llist_get_size(aliens_b);

  /* Free Memory */
  llist_free(list_bridge_left);
  llist_free(list_bridge_center);
  llist_free(list_bridge_right);

  llist_free(aliens_left_north);
  llist_free(aliens_left_south);
  llist_free(aliens_right_north);
  llist_free(aliens_right_south);
  llist_free(aliens_center_north);
  llist_free(aliens_center_south);

  free(bridge_struct_left);
  free(bridge_struct_right);
  free(bridge_struct_center);

  free(test);

  free(weight_now_left);

  free(params_left);

  /* JOIN THREADS */
  if (aliens_a_size != 0)
  {
    for (int i = 0; i < aliens_a_size; ++i)
    {
      alien *curr = llist_get_by_index(aliens_a, i);
      if (curr == NULL)
      {
        printf("INDEX OUT OF RANGE ERROR. BREAKING LOOP\n");
        break;
      }
      Lthread_join(*(curr->thread), NULL);
      printf("THREAD FOR ALIEN A INDEX %d JOINED\n", i);
    }
  }

  printf("ALIENS A JOINED\n");

  if (aliens_b_size != 0)
  {
    for (int i = 0; i < aliens_b_size; ++i)
    {
      alien *curr = llist_get_by_index(aliens_b, i);
      if (curr == NULL)
      {
        printf("INDEX OUT OF RANGE ERROR. BREAKING LOOP\n");
        break;
      }
      Lthread_join(*(curr->thread), NULL);
      printf("THREAD FOR ALIEN B INDEX %d JOINED\n", i);
    }
  }

  printf("ALIENS B JOINED\n");

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
  gettimeofday(&entity->work_init_time, NULL);
  entity->last_update = entity->work_init_time;
  entity->progress = 0;
  entity->duration = 10;
  entity->working = 0;
  entity->accumulator = 0;
  entity->priority = 0;
  entity->weight = 15;

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

int spawn_invader(void)
{
  if (invader_alive == 0)
  {
    invader_alive = 1;
    invader = (alien *)malloc(sizeof(alien));

    Lthread_create(&invader_thread_id, NULL, &invader_thread, NULL);
  }
}

int alien_a_thread(void *param)
{
  int index = *((int *)param);

  //int hola = generate_random(3, 1);
  int hola = 1;

  printf("ANTES GET ALIEN\n");
  alien *my_alien = llist_get_by_index(aliens_a, index);
  printf("DESPUES GET ALIEN\n");

  int id = my_alien->thread->pid;
  my_alien->id = id;

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[0][i], my_alien->velocity, aliens_a, my_alien->id, 0);
  }

  /* PARTE ROSADA */
  FIFO_scheduler(aliens_left_north, (void *)my_alien);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[hola][i], my_alien->velocity, aliens_a, my_alien->id, 0);
  }

  /* INICIO PUENTE */

  move_bridge(&my_alien->pos, &my_alien->progress, 1);

  /* FIN PUENTE */

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[hola + 3][i], my_alien->velocity, aliens_a, my_alien->id, 0);
  }

  /* FIN PARTE ROSADA */

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_a[7][i], my_alien->velocity, aliens_a, my_alien->id, 0);
  }

  //free(param);

  int curr_index = llist_get_alien_index(aliens_a, my_alien->id);

  printf("CURR INDEX = %d\n", curr_index);

  llist_remove_by_index(aliens_a, curr_index);
  list_a_size--;

  printf("Thread end\n");

  return 0;
}

int alien_b_thread(void *param)
{
  int index = *((int *)param);

  int hola = generate_random(3, 1);

  printf("ANTES GET ALIEN\n");
  alien *my_alien = llist_get_by_index(aliens_b, index);
  printf("DESPUES GET ALIEN\n");

  int id = my_alien->thread->pid;
  my_alien->id = id;

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[0][i], my_alien->velocity, aliens_b, my_alien->id, 1);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[hola][i], my_alien->velocity, aliens_b, my_alien->id, 1);
  }

  move_bridge(&my_alien->pos, &my_alien->progress, -1);

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[hola + 3][i], my_alien->velocity, aliens_b, my_alien->id, 1);
  }

  for (int i = 0; i < 3; ++i)
  {
    move(&my_alien->pos, routes_b[7][i], my_alien->velocity, aliens_b, my_alien->id, 1);
  }

  //free(param);

  int curr_index = llist_get_alien_index(aliens_b, my_alien->id);

  printf("CURR INDEX = %d\n", curr_index);

  llist_remove_by_index(aliens_b, curr_index);
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
    move_invader(&invader->pos, route[rand_route][i], velocity * 2);
  }

  for (int i = 0; i < 3; ++i)
  {
    move_invader(&invader->pos, route[rand_route + 3][i], velocity * 2);
  }

  for (int i = 0; i < 3; ++i)
  {
    move_invader(&invader->pos, route[7][i], velocity * 2);
  }

  invader_alive = 0;
}