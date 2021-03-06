#ifndef _P_PROCESS_H
#define _P_PROCESS_H

/* library to process images */
void quick_sort(unsigned char *list, int start, int stop);
unsigned char partition(unsigned char *a, int l, int r);
rgb *process_image(int width, int height, rgb *image, int window_size);

#endif