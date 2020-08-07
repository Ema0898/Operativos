#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <structs.h>

unsigned char partition(unsigned char *a, int l, int r) {
    unsigned char pivot, t;
    int i, j;
    pivot = a[l];
    i = l; j = r + 1;

    while (1)
    {
        do ++i; while (a[i] <= pivot && i <= r);
        do --j; while (a[j] > pivot);
        if (i >= j) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
    }
    t = a[l];
    a[l] = a[j];
    a[j] = t;
    return j;
}

void quick_sort(unsigned char *a, int l, int r)
{
    int j;

    if (l < r)
    {
        j = partition(a, l, r);
        quick_sort(a, l, j - 1);
        quick_sort(a, j + 1, r);
    }
}

rgb *process_image(int width, int height, rgb *image, int window_size)
{
    int i, j, k, l;

    if (window_size == 1)
    {
        return image;
    }

    // Process pixel by pixel
    rgb *filtered = (rgb*)malloc(height * width * sizeof(rgb));
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            unsigned char *windowR = (unsigned char*)malloc(sizeof(unsigned char) * window_size * window_size);
            unsigned char *windowB = (unsigned char*)malloc(sizeof(unsigned char) * window_size * window_size);
            unsigned char *windowG = (unsigned char*)malloc(sizeof(unsigned char) * window_size * window_size);

            int start = 0;
            int left = i - window_size / 2;
            int right = i + window_size / 2;
            int up = j - window_size / 2;
            int down = j + window_size / 2;
            for (k = left; k < right; k++)
            {
                for (l = up; l < down; l++)
                {
                    if (k >= 0 && l >= 0 && k < height && l < width) {
                        windowR[start] = image[k * width + l].r;
                        windowG[start] = image[k * width + l].g;
                        windowB[start] = image[k * width + l].b;
                        start++;
                    }
                }
            }

            unsigned char filteredElementR;
            unsigned char filteredElementG;
            unsigned char filteredElementB;

            quick_sort(windowR, 0, start);
            quick_sort(windowG, 0, start);
            quick_sort(windowB, 0, start);

            filtered[i * width + j].r = windowR[start / 2];
            filtered[i * width + j].g = windowG[start / 2];
            filtered[i * width + j].b = windowB[start / 2];
        }
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i * width + j] = filtered[i * width + j];
        }
    }

    return image;
}