#ifndef _IMAGEFILTERS_H
#define _IMAGEFILTERS_H


#include <stdlib.h>
#include <stdio.h>
#include <MagickWand/MagickWand.h>

//https://imagemagick.org/script/magick-wand.php

/**
 Average Filter
 const char* image_path: Image directory input
 const char* image_name: Image name
 const char* destiny_path: Image directory output
*/

void average_filter(const char* image_path, const char* image_name, const char* destiny_path);

/**
 Median Filter
 const char* image_path: Image directory input
 const char* image_name: Image name
 const char* destiny_path: Image directory output
*/

void median_filter(const char* image_path, const char* image_name, const char* destiny_path);


/**
 Simple compare function
*/

int cmpfunc (const void * a, const void * b);


#endif // _IMAGEFILTERS_H