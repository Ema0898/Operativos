#include <SDL2/SDL.h>
#include <gui.h>
#include <utilities.h>
#include <structs.h>
#include <cfg.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <list.h>

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = 30;

const int X_TILES = 46;
const int Y_TILES = 24;

int map[24][46];

point positions[3];
int list_size = 0;

point actual_pos;

void *print_message_function(void *param);
void move(point *actual, point dest, int velocity);
int stop_move(point actual, point dest, float dist_x, float dist_y);
int spawn_alien(void);

int main(int argc, char *argv[])
{
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

  int medium = 0;
  if (!valdite_args(argc, argv, &medium))
  {
    return 1;
  }

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

  bridge hola;
  hola.weight = 5;
  hola.weight = 5;

  load_bridge(&hola);

  node_t *head = NULL;

  add_start(&head, 1);
  add_end(head, 2);

  printf("Original List\n");
  print_list(head);

  pthread_t thread1;
  int iret1;

  iret1 = pthread_create(&thread1, NULL, &print_message_function, NULL);
  if (iret1)
  {
    fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
    exit(EXIT_FAILURE);
  }

  /* Test End */

  /* Alien Position Initialization */
  actual_pos.x = 50;
  actual_pos.y = 300;

  // positions[0].x = 200;
  // positions[0].y = 300;

  // positions[1].x = 200;
  // positions[1].y = 500;

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

  SDL_Texture *image = load_texture("../assets/images/image5.png", ren);
  SDL_Texture *brigde_road = load_texture("../assets/images/road.png", ren);
  SDL_Texture *bridge = load_texture("../assets/images/bridge.png", ren);
  SDL_Texture *road = load_texture("../assets/images/asphalt.png", ren);
  SDL_Texture *background = load_texture("../assets/images/background2.png", ren);
  SDL_Texture *base_a = load_texture("../assets/images/A.png", ren);
  SDL_Texture *base_b = load_texture("../assets/images/B.png", ren);

  SDL_Rect clips[4];

  for (int i = 0; i < 4; ++i)
  {
    clips[i].x = i / 2 * 100;
    clips[i].y = i % 2 * 100;
    clips[i].w = 100;
    clips[i].h = 100;
  }

  int use_clip = 0;

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
      }

      if (e.type == SDL_KEYDOWN)
      {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
          printf("KEY A PRESSED\n");
          spawn_alien();
          break;

        default:
          break;
        }
      }

      if ((e.type == SDL_MOUSEBUTTONDOWN) & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);
        img_rect = get_texture_rect_wh(image, actual_pos.x, actual_pos.y, 100, 100);
        if (SDL_HasIntersection(&mouse_rect, &img_rect))
        {
          actual_pos.x = 50;
          actual_pos.y = 300;
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

    render_sheet_texture(image, ren, actual_pos.x, actual_pos.y, &clips[use_clip]);

    for (int i = 0; i < 3; ++i)
    {
      if (positions[i].x != 0 && positions[i].y != 0)
        render_sheet_texture(image, ren, positions[i].x, positions[i].y, &clips[use_clip]);
    }

    SDL_RenderPresent(ren);

    SDL_Delay(0.016666);

    use_clip++;
    use_clip %= 4;
  }

  SDL_DestroyTexture(image);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  quit_graphics();

  pthread_cancel(iret1);

  return 0;
}

int stop_move(point actual, point dest, float dist_x, float dist_y)
{
  int cond_x, cond_y;

  if (dist_x > 0)
    cond_x = actual.x >= dest.x;
  else if (dist_x < 0)
    cond_x = actual.x <= dest.x;
  else
    cond_x = actual.x == dest.x;

  if (dist_y > 0)
    cond_y = actual.y >= dest.y;
  else if (dist_y < 0)
    cond_y = actual.y <= dest.y;
  else
    cond_y = actual.y == dest.y;

  return cond_x && cond_y;
}

void move(point *actual, point dest, int velocity)
{
  float dist_x = dest.x - actual->x;
  float dist_y = dest.y - actual->y;

  float norm = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

  dist_x /= norm;
  dist_y /= norm;

  int moving = 1;

  printf("HOLA %f\n", actual->y);

  while (moving)
  {
    actual->x += dist_x * velocity;
    actual->y += dist_y * velocity;

    if (stop_move(actual_pos, dest, dist_x, dist_y))
    {
      moving = 0;
    }

    usleep(16666);
  }

  actual->x = ceil(actual->x);
  actual->y = ceil(actual->y);
}

void *print_message_function(void *param)
{
  int moving = 1;

  point dest1;
  dest1.x = 100;
  dest1.y = 500;

  point dest2;
  dest2.x = 400;
  dest2.y = 300;

  while (moving)
  {
    move(&actual_pos, dest1, 3);
    move(&actual_pos, dest2, 1);
    moving = 0;
  }

  printf("Thread end\n");
}

int spawn_alien(void)
{
  if (list_size > 2)
  {
    return -1;
  }

  positions[list_size].x = (list_size + 1) * 100;
  positions[list_size].y = (list_size + 1) * 100;

  list_size++;

  printf("List SIZE = %d\n", list_size);
  printf("POSX = %d\n", (list_size + 1) * 100);
}
