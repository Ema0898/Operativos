#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <structs.h>

/* partition for quick sort */
unsigned char partition(unsigned char *list, int start, int stop) {
    unsigned char pivot, t;
    int i, j;
    pivot = list[start];
    i = start; j = stop + 1;

    while (1)
    {
        do ++i; while (list[i] <= pivot && i <= stop);
        do --j; while (list[j] > pivot);
        if (i >= j) break;

        t = list[i];
        list[i] = list[j];
        list[j] = t;
    }

    t = list[start];
    list[start] = list[j];
    list[j] = t;

    return j;
}

/* sort median pixels */
void quick_sort(unsigned char *list, int start, int stop)
{
    int tmp;

    if (start < stop)
    {
        tmp = partition(list, start, stop);
        quick_sort(list, start, tmp - 1);
        quick_sort(list, tmp + 1, stop);
    }
}

rgb *process_image(int width, int height, rgb *noisy_image, int window_size)
{
    if (window_size == 1)
    {
        return noisy_image;
    }

    // Process pixel by pixel
    rgb *filtered_image = (rgb*)malloc(height * width * sizeof(rgb));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char *r_channel = (unsigned char*)malloc(sizeof(unsigned char) * window_size * window_size);
            unsigned char *g_channel = (unsigned char*)malloc(sizeof(unsigned char) * window_size * window_size);
            unsigned char *b_channel = (unsigned char*)malloc(sizeof(unsigned char) * window_size * window_size);

            int index = 0;
            int part_left = i - window_size / 2;
            int part_right = i + window_size / 2;
            int part_up = j - window_size / 2;
            int part_down = j + window_size / 2;

            for (int k = part_left; k < part_right; k++)
            {
                for (int l = part_up; l < part_down; l++)
                {
                    if (k >= 0 && l >= 0 && k < height && l < width) {
                        r_channel[index] = noisy_image[k * width + l].r;
                        g_channel[index] = noisy_image[k * width + l].g;
                        b_channel[index] = noisy_image[k * width + l].b;
                        index++;
                    }
                }
            }

            quick_sort(r_channel, 0, index);
            quick_sort(g_channel, 0, index);
            quick_sort(b_channel, 0, index);

            filtered_image[i * width + j].r = r_channel[index / 2];
            filtered_image[i * width + j].g = g_channel[index / 2];
            filtered_image[i * width + j].b = b_channel[index / 2];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            noisy_image[i * width + j] = filtered_image[i * width + j];
        }
    }

    return noisy_image;
}