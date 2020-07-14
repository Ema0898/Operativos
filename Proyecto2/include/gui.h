#ifndef _GUI_H
#define _GUI_H

SDL_Texture *load_texture(const char *file, SDL_Renderer *ren);
SDL_Texture *render_text(char *msg, char *file, SDL_Color color, int size, SDL_Renderer *ren);
void render_scale_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void render_full_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void render_clip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect destination, SDL_Rect *clip);
void render_sheet_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);
SDL_Rect get_texture_rect(SDL_Texture *tex, int x, int y);
SDL_Rect get_texture_rect_wh(SDL_Texture *tex, int x, int y, int w, int h);
void log_error(char *msg);
int init_graphics(void);
void quit_graphics(void);

#endif