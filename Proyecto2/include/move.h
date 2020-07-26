#ifndef MOVE_H
#define MOVE_H

void move(point *actual, point dest, float velocity, llist *list, int index, int community, short *working);
int stop_move(point *actual, point dest, float dist_x, float dist_y);
void init_routes(point routes_a[8][3], point routes_b[8][3]);
int move_bridge(point *actual, float *progress, int direcction, llist *list, int index, int community, int *quantum_over, short *working);
float generate_alien_velocity(int type, int base_velocity, int percentage);
void move_invader(point *actual, point dest, float velocity, llist *aliens_a, llist *aliens_b, llist *list_bridge_left, llist *list_bridge_right, llist *list_bridge_center,
                  llist *aliens_left_north, llist *aliens_left_south, llist *aliens_right_north, llist *aliens_right_south, llist *aliens_center_north, llist *aliens_center_south, int *list_a_size, int *list_b_size, int *weight_now_left, int *weight_now_right, int *weight_now_center);

#endif