#include <SDL2/SDL.h>
#include <gui.h>
#include <utilities.h>
#include <structs.h>
#include <cfg.h>

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = 30;

const int X_TILES = 46;
const int Y_TILES = 24;

int map[24][46];

int main()
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

  bridge hola;
  hola.weight = 5;
  hola.weight = 5;

  load_bridge(&hola);

  printf("%d, %d\n", hola.weight, hola.length);

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
  SDL_Texture *BiRoad = load_texture("../assets/images/road.jpg", ren);
  SDL_Texture *Bridge = load_texture("../assets/images/bridge.jpg", ren);
  SDL_Texture *Road = load_texture("../assets/images/asphalt.png", ren);
  SDL_Texture *background = load_texture("../assets/images/background2.jpg", ren);
  SDL_Texture *base_a = load_texture("../assets/images/A.png", ren);
  SDL_Texture *base_b = load_texture("../assets/images/B.png", ren);

  int iw = 100, ih = 100;
  int x = SCREEN_WIDTH / 2 - iw / 2;
  int y = SCREEN_HEIGHT / 2 - ih / 2;

  SDL_Rect clips[4];

  for (int i = 0; i < 4; ++i)
  {
    clips[i].x = i / 2 * iw;
    clips[i].y = i % 2 * ih;
    clips[i].w = iw;
    clips[i].h = ih;
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

      if ((e.type == SDL_MOUSEBUTTONDOWN) & SDL_BUTTON(SDL_BUTTON_LEFT))
      {
        SDL_GetMouseState(&mouse_rect.x, &mouse_rect.y);
        img_rect = get_texture_rect_wh(image, x, y, 100, 100);
        if (SDL_HasIntersection(&mouse_rect, &img_rect))
        {
          x = 0;
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
          render_scale_texture(Road, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
        else if (map[i][j] == 2)
        {
          render_scale_texture(BiRoad, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
        else if (map[i][j] == 3)
        {
          render_scale_texture(Bridge, ren, j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
      }
    }

    render_scale_texture(base_a, ren, 10, 200, 170, 170);
    render_scale_texture(base_b, ren, SCREEN_WIDTH - 180, 235, 170, 170);

    render_sheet_texture(image, ren, x, y, &clips[use_clip]);
    SDL_RenderPresent(ren);

    SDL_Delay(500);

    x += 10;
    x %= SCREEN_WIDTH;

    use_clip++;
    use_clip %= 4;
  }

  SDL_DestroyTexture(image);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  quit_graphics();

  return 0;
}
