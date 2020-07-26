#include <math.h>
#include <structs.h>
#include <unistd.h>
#include <stdio.h>
#include <list.h>
#include <SDL2/SDL.h>
#include <lpthread.h>
#include <utilities.h>
#include <lpthread.h>

/* checks if alien reaches its destiny */
int stop_move(point *actual, point dest, float dist_x, float dist_y)
{
  int cond_x, cond_y;

  if (dist_x > 0)
    cond_x = actual->x >= dest.x;
  else if (dist_x < 0)
    cond_x = actual->x <= dest.x;
  else
    cond_x = actual->x == dest.x;

  if (dist_y > 0)
    cond_y = actual->y >= dest.y;
  else if (dist_y < 0)
    cond_y = actual->y <= dest.y;
  else
    cond_y = actual->y == dest.y;

  return cond_x && cond_y;
}

/* move alien and checks collisions */
void move(point *actual, point dest, float velocity, llist *list, int index, int community, short *working)
{
  float dist_x = dest.x - actual->x;
  float dist_y = dest.y - actual->y;

  float norm = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

  dist_x /= norm;
  dist_y /= norm;

  int moving = 1;

  float tmp_x, tmp_y;

  SDL_Rect myself, other;

  int square;

  if (community == 0)
  {
    square = 28;
  }
  else if (community == 1)
  {
    square = 20;
  }

  myself.h = square;
  myself.w = square;

  other.h = square;
  other.w = square;

  int intersection = 0;
  int size = 0;

  while (moving)
  {
    tmp_x = actual->x + dist_x * velocity;
    tmp_y = actual->y + dist_y * velocity;

    myself.x = tmp_x;
    myself.y = tmp_y;

    size = llist_get_size(list);

    intersection = 0;

    if (!(*working))
    {
      for (int i = 0; i < size; ++i)
      {
        alien *curr = llist_get_by_index(list, i);

        if (curr == NULL)
        {
          break;
        }

        if (index != curr->id)
        {
          other.x = curr->pos.x;
          other.y = curr->pos.y;

          if (SDL_HasIntersection(&myself, &other))
          {
            intersection = 1;
          }
        }
        size = llist_get_size(list);
      }
    }

    if (!intersection)
    {
      actual->x += dist_x * velocity;
      actual->y += dist_y * velocity;
    }

    intersection = 0;

    if (stop_move(actual, dest, dist_x, dist_y))
    {
      moving = 0;
    }

    usleep(16666 * 2);
  }

  actual->x = ceil(actual->x);
  actual->y = ceil(actual->y);
}

/* moves alien in the bridge section */
int move_bridge(point *actual, float *progress, int direcction, llist *list, int index, int community, int *quantum_over, short *working)
{
  float init_pos_y = actual->y;
  int bridge_height = 120;

  float tmp_x, tmp_y;

  SDL_Rect myself, other;

  int square;

  if (community == 0)
  {
    square = 28;
  }
  else if (community == 1)
  {
    square = 20;
  }

  myself.h = square;
  myself.w = square;

  other.h = square;
  other.w = square;

  int intersection = 0;
  int size = 0;

  tmp_x = actual->x;

  while (*progress < 1)
  {
    tmp_y = (bridge_height * (*progress)) * direcction + init_pos_y;

    myself.x = tmp_x;
    myself.y = tmp_y;

    size = llist_get_size(list);

    intersection = 0;

    for (int i = 0; i < size; ++i)
    {
      alien *curr = llist_get_by_index(list, i);

      if (curr == NULL)
      {
        break;
      }

      if (index != curr->id)
      {
        other.x = curr->pos.x;
        other.y = curr->pos.y;

        if (SDL_HasIntersection(&myself, &other) && curr->working)
        {
          intersection = 1;
        }
      }
      size = llist_get_size(list);
    }

    if (!intersection && *working)
    {
      actual->y = (bridge_height * (*progress)) * direcction + init_pos_y;
    }

    if (*quantum_over)
    {
      return 1;
    }

    intersection = 0;

    usleep(16666 * 2);
  }

  return 0;
}

/* move invader alien */
void move_invader(point *actual, point dest, float velocity, llist *aliens_a, llist *aliens_b, llist *list_bridge_left, llist *list_bridge_right, llist *list_bridge_center,
                  llist *aliens_left_north, llist *aliens_left_south, llist *aliens_right_north, llist *aliens_right_south, llist *aliens_center_north, llist *aliens_center_south, int *list_a_size, int *list_b_size, int *weight_now_left, int *weight_now_right, int *weight_now_center)
{
  float dist_x = dest.x - actual->x;
  float dist_y = dest.y - actual->y;

  float norm = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

  dist_x /= norm;
  dist_y /= norm;

  int moving = 1;

  float tmp_x, tmp_y;

  SDL_Rect myself, other, other2;

  myself.h = 28;
  myself.w = 28;

  other.h = 28;
  other.w = 28;

  other2.h = 28;
  other2.w = 28;

  int size_a = 0;
  int size_b = 0;

  while (moving)
  {
    tmp_x = actual->x + dist_x * velocity;
    tmp_y = actual->y + dist_y * velocity;

    myself.x = tmp_x;
    myself.y = tmp_y;

    size_a = llist_get_size(aliens_a);
    size_b = llist_get_size(aliens_b);

    int alien_mouse_pos = 0;

    for (int i = 0; i < size_a; ++i)
    {
      alien *curr = llist_get_by_index(aliens_a, i);

      if (curr == NULL)
      {
        break;
      }

      other.x = curr->pos.x;
      other.y = curr->pos.y;

      if (SDL_HasIntersection(&myself, &other) && curr->type == 2)
      {
        lpthread_t *thread = curr->thread;
        Lthread_exit(thread->pid);

        *list_a_size -= 1;

        llist_remove_by_index(aliens_a, i);
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

      size_a = llist_get_size(aliens_a);
    }

    for (int i = 0; i < size_b; ++i)
    {
      alien *curr = llist_get_by_index(aliens_b, i);

      if (curr == NULL)
      {
        break;
      }

      other2.x = curr->pos.x;
      other2.y = curr->pos.y;

      if (SDL_HasIntersection(&myself, &other2) && curr->type == 2)
      {
        lpthread_t *thread2 = curr->thread;
        Lthread_exit(thread2->pid);

        *list_b_size -= 1;

        llist_remove_by_index(aliens_b, i);

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

      size_b = llist_get_size(aliens_b);
    }

    actual->x += dist_x * velocity;
    actual->y += dist_y * velocity;

    if (stop_move(actual, dest, dist_x, dist_y))
    {
      moving = 0;
    }

    usleep(16666 * 2);
  }

  actual->x = ceil(actual->x);
  actual->y = ceil(actual->y);
}

/* generates aliens velocity based in its type */
float generate_alien_velocity(int type, int base_velocity, int percentage)
{
  float result = 1;

  switch (type)
  {
  case 0:
    result = base_velocity;
    break;

  case 1:
    result = base_velocity + (base_velocity * 0.2);
    break;

  case 2:
    result = base_velocity + base_velocity * (percentage / 100);
    break;

  default:
    break;
  }

  return result;
}

/* init aliens routes */
void init_routes(point routes_a[8][3], point routes_b[8][3])
{
  /* Community A to Init Pink */
  routes_a[0][0].x = 90;
  routes_a[0][0].y = 60;

  routes_a[0][1].x = 630;
  routes_a[0][1].y = 60;

  routes_a[0][2].x = 630;
  routes_a[0][2].y = 120;

  /* Left A Init Pink to Bridge Init */
  routes_a[1][0].x = 330;
  routes_a[1][0].y = 120;

  routes_a[1][1].x = 330;
  routes_a[1][1].y = 240;

  routes_a[1][2].x = 330;
  routes_a[1][2].y = 270;

  /* Left A End Bridge to End Pink */
  routes_a[4][0].x = 330;
  routes_a[4][0].y = 530;

  routes_a[4][1].x = 630;
  routes_a[4][1].y = 530;

  routes_a[4][2].x = 690;
  routes_a[4][2].y = 530;

  /* Right A Init Pink to Bridge Init  A */
  routes_a[2][0].x = 660;
  routes_a[2][0].y = 150;

  routes_a[2][1].x = 990;
  routes_a[2][1].y = 150;

  routes_a[2][2].x = 990;
  routes_a[2][2].y = 270;

  /* Right A Bridge End to Pink End */
  routes_a[5][0].x = 990;
  routes_a[5][0].y = 510;

  routes_a[5][1].x = 730;
  routes_a[5][1].y = 510;

  routes_a[5][2].x = 690;
  routes_a[5][2].y = 530;

  /* Center A Init Pink to Bridge Init */
  routes_a[3][0].x = 660;
  routes_a[3][0].y = 150;

  routes_a[3][1].x = 660;
  routes_a[3][1].y = 240;

  routes_a[3][2].x = 660;
  routes_a[3][2].y = 270;

  /* Center A End Bridge to Pink End */
  routes_a[6][0].x = 660;
  routes_a[6][0].y = 420;

  routes_a[6][1].x = 660;
  routes_a[6][1].y = 500;

  routes_a[6][2].x = 690;
  routes_a[6][2].y = 530;

  /* End Pink to Community A */
  routes_a[7][0].x = 690;
  routes_a[7][0].y = 630;

  routes_a[7][1].x = 1230;
  routes_a[7][1].y = 630;

  routes_a[7][2].x = 1230;
  routes_a[7][2].y = 390;

  /* Community B to Pink */
  routes_b[0][0].x = 1290;
  routes_b[0][0].y = 680;

  routes_b[0][1].x = 630;
  routes_b[0][1].y = 680;

  routes_b[0][2].x = 630;
  routes_b[0][2].y = 530;

  /* Left B Init Pink to Init Bridge */
  routes_b[1][0].x = 600;
  routes_b[1][0].y = 500;

  routes_b[1][1].x = 360;
  routes_b[1][1].y = 500;

  routes_b[1][2].x = 360;
  routes_b[1][2].y = 390;

  /* Left B End Bridge to End Pink */
  routes_b[4][0].x = 360;
  routes_b[4][0].y = 150;

  routes_b[4][1].x = 660;
  routes_b[4][1].y = 150;

  routes_b[4][2].x = 690;
  routes_b[4][2].y = 120;

  /* Right B Init Pink to Init Pink */
  routes_b[2][0].x = 1020;
  routes_b[2][0].y = 530;

  routes_b[2][1].x = 1020;
  routes_b[2][1].y = 420;

  routes_b[2][2].x = 1020;
  routes_b[2][2].y = 390;

  /* Right B End Bridge to End Pink */
  routes_b[5][0].x = 1020;
  routes_b[5][0].y = 120;

  routes_b[5][1].x = 720;
  routes_b[5][1].y = 120;

  routes_b[5][2].x = 690;
  routes_b[5][2].y = 120;

  /* Center B Init Pink to Init Bridge */
  routes_b[3][0].x = 690;
  routes_b[3][0].y = 500;

  routes_b[3][1].x = 690;
  routes_b[3][1].y = 420;

  routes_b[3][2].x = 690;
  routes_b[3][2].y = 390;

  /* Center B End Pink to End Bridge */
  routes_b[6][0].x = 690;
  routes_b[6][0].y = 240;

  routes_b[6][1].x = 690;
  routes_b[6][1].y = 210;

  routes_b[6][2].x = 690;
  routes_b[6][2].y = 120;

  /* Pink End to Community B*/
  routes_b[7][0].x = 690;
  routes_b[7][0].y = 0;

  routes_b[7][1].x = 30;
  routes_b[7][1].y = 0;

  routes_b[7][2].x = 30;
  routes_b[7][2].y = 180;
}