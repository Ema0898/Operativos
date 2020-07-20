#include <math.h>
#include <structs.h>
#include <unistd.h>

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

void move(point *actual, point dest, int velocity)
{
  float dist_x = dest.x - actual->x;
  float dist_y = dest.y - actual->y;

  float norm = sqrt(pow(dist_x, 2) + pow(dist_y, 2));

  dist_x /= norm;
  dist_y /= norm;

  int moving = 1;

  while (moving)
  {
    actual->x += dist_x * velocity;
    actual->y += dist_y * velocity;

    if (stop_move(actual, dest, dist_x, dist_y))
    {
      moving = 0;
    }

    usleep(16666);
  }

  actual->x = ceil(actual->x);
  actual->y = ceil(actual->y);
}

// int spawn_alien(point *positions, int *list_size)
// {
//   if (*list_size > 2)
//   {
//     return -1;
//   }

//   positions[*list_size].x = (*list_size + 1) * 100;
//   positions[*list_size].y = (*list_size + 1) * 100;

//   *list_size = *list_size + 1;
// }

void init_routes(point routes_a[5][5], point routes_b[5][5])
{
  /* Community A to bridges */
  routes_a[0][0].x = 90;
  routes_a[0][0].y = 60;

  routes_a[0][1].x = 630;
  routes_a[0][1].y = 60;

  routes_a[0][2].x = 630;
  routes_a[0][2].y = 120;

  /* Left Bridge A */
  routes_a[1][0].x = 330;
  routes_a[1][0].y = 120;

  routes_a[1][1].x = 330;
  routes_a[1][1].y = 530;

  routes_a[1][2].x = 690;
  routes_a[1][2].y = 530;

  /* Central Bridge A */
  routes_a[2][0].x = 660;
  routes_a[2][0].y = 150;

  routes_a[2][1].x = 660;
  routes_a[2][1].y = 500;

  routes_a[2][2].x = 690;
  routes_a[2][2].y = 530;

  /* Right Bridge A */
  routes_a[3][0].x = 660;
  routes_a[3][0].y = 150;

  routes_a[3][1].x = 990;
  routes_a[3][1].y = 150;

  routes_a[3][2].x = 990;
  routes_a[3][2].y = 500;

  routes_a[3][3].x = 720;
  routes_a[3][3].y = 500;

  routes_a[3][4].x = 690;
  routes_a[3][4].y = 530;

  /* Bridges to Community A */
  routes_a[4][0].x = 690;
  routes_a[4][0].y = 630;

  routes_a[4][1].x = 1230;
  routes_a[4][1].y = 630;

  routes_a[4][2].x = 1230;
  routes_a[4][2].y = 390;

  /* Community to Bridges B */
  routes_b[0][0].x = 1290;
  routes_b[0][0].y = 680;

  routes_b[0][1].x = 630;
  routes_b[0][1].y = 680;

  routes_b[0][2].x = 630;
  routes_b[0][2].y = 530;

  /* Right Bridge B*/
  routes_b[1][0].x = 1020;
  routes_b[1][0].y = 530;

  routes_b[1][1].x = 1020;
  routes_b[1][1].y = 120;

  routes_b[1][2].x = 690;
  routes_b[1][2].y = 120;

  /* Central Bridge B*/
  routes_b[2][0].x = 690;
  routes_b[2][0].y = 500;

  routes_b[2][1].x = 690;
  routes_b[2][1].y = 180;

  routes_b[2][2].x = 690;
  routes_b[2][2].y = 120;

  /* Left Bridge B */
  routes_b[3][0].x = 600;
  routes_b[3][0].y = 500;

  routes_b[3][1].x = 360;
  routes_b[3][1].y = 500;

  routes_b[3][2].x = 360;
  routes_b[3][2].y = 150;

  routes_b[3][3].x = 660;
  routes_b[3][3].y = 150;

  routes_b[3][4].x = 690;
  routes_b[3][4].y = 120;

  /* Bridges to Community B*/
  routes_b[4][0].x = 690;
  routes_b[4][0].y = 0;

  routes_b[4][1].x = 30;
  routes_b[4][1].y = 0;

  routes_b[4][2].x = 30;
  routes_b[4][2].y = 180;
}