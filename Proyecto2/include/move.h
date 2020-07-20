#ifndef MOVE_H
#define MOVE_H

void move(point *actual, point dest, int velocity);
int stop_move(point *actual, point dest, float dist_x, float dist_y);
//int spawn_alien(point *positions, int *list_size);
void init_routes(point routes_a[5][5], point routes_b[5][5]);

#endif