#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void log_error(char *msg)
{
  printf("Error in %s: %s\n", msg, SDL_GetError());
}

SDL_Texture *load_texture(const char *file, SDL_Renderer *ren)
{
  SDL_Texture *texture = IMG_LoadTexture(ren, file);

  if (texture == NULL)
  {
    log_error("Load Texture");
  }
  return texture;
}

SDL_Texture *render_text(char *msg, char *file, SDL_Color color, int size, SDL_Renderer *ren)
{
  TTF_Font *font = TTF_OpenFont(file, size);
  if (font == NULL)
  {
    log_error("TTF Open Font");
    return NULL;
  }

  SDL_Surface *surf = TTF_RenderText_Blended(font, msg, color);
  if (surf == NULL)
  {
    TTF_CloseFont(font);
    log_error("TFF Render Text");
    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
  if (texture == NULL)
  {
    log_error("Create Texture");
  }

  SDL_FreeSurface(surf);
  TTF_CloseFont(font);

  return texture;
}

void render_scale_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
  SDL_Rect destination;

  destination.x = x;
  destination.y = y;
  destination.w = w;
  destination.h = h;

  SDL_RenderCopy(ren, tex, NULL, &destination);
}

void render_full_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
  int w, h;

  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  render_scale_texture(tex, ren, x, y, w, h);
}

void render_clip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect destination, SDL_Rect *clip)
{
  SDL_RenderCopy(ren, tex, clip, &destination);
}

void render_sheet_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
{
  SDL_Rect destination;
  destination.x = x;
  destination.y = y;

  if (clip != NULL)
  {
    destination.w = clip->w;
    destination.h = clip->h;
  }
  else
  {
    SDL_QueryTexture(tex, NULL, NULL, &destination.w, &destination.h);
  }

  render_clip(tex, ren, destination, clip);
}

int init_graphics(void)
{
  int result = 0;
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    log_error("SDL Init Error");
    return -1;
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    log_error("IMG_Init");
    SDL_Quit();
    return -1;
  }

  if (TTF_Init() != 0)
  {
    log_error("TTF Init");
    IMG_Quit();
    SDL_Quit();
    return -1;
  }
}

SDL_Rect get_texture_rect(SDL_Texture *tex, int x, int y)
{
  SDL_Rect rect;
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

SDL_Rect get_texture_rect_wh(SDL_Texture *tex, int x, int y, int w, int h)
{
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

void quit_graphics(void)
{
  IMG_Quit();
  SDL_Quit();
}
