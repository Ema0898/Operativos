#ifndef _CFG_H
#define _CFG_H

int init_cfg(void);
void load_bridge_left(configurable *a);
void load_bridge_right(configurable *a);
void load_bridge_center(configurable *a);
void load_alien(int *velocity, int *percentages);
void quit_cfg(void);

#endif