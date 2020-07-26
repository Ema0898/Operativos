#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

TTF_Font *font;

/* print errors */
void log_error(char *msg)
{
  printf("Error in %s: %s\n", msg, SDL_GetError());
}

/* load texture */
SDL_Texture *load_texture(const char *file, SDL_Renderer *ren)
{
  SDL_Texture *texture = IMG_LoadTexture(ren, file);

  if (texture == NULL)
  {
    log_error("Load Texture");
  }
  return texture;
}

/* render text from string */
SDL_Texture *render_text(char *msg, char *file, SDL_Color color, int size, SDL_Renderer *ren)
{
  font = TTF_OpenFont(file, size);
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

/* render texture with fixed width and height */
void render_scale_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
  SDL_Rect destination;

  destination.x = x;
  destination.y = y;
  destination.w = w;
  destination.h = h;

  SDL_RenderCopy(ren, tex, NULL, &destination);
}

/* render texture with its complete width and height */
void render_full_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
  int w, h;

  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  render_scale_texture(tex, ren, x, y, w, h);
}

/* render specific section of image */
void render_clip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect destination, SDL_Rect *clip)
{
  SDL_RenderCopy(ren, tex, clip, &destination);
}

/* render spritesheet texture */
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

/* init graphics library */
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

/* get texture hit box */
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

/* get texture hit box with fixed width and height */
SDL_Rect get_texture_rect_wh(SDL_Texture *tex, int x, int y, int w, int h)
{
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

/* finish graphic library */
void quit_graphics(void)
{
  IMG_Quit();
  SDL_Quit();
}
