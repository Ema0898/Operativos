#include <stdio.h>
#include <time.h>
#include "imagefilters.h"

int main()
{
  // average_filter("./media/", "barbara.jpg", "./media/averageFilter");
  // median_filter("./media/", "barbara.jpg", "./media/medianFilter");

  classify("./media/", "lena.jpg", "./media/classify/");

  // time_t t = time(NULL);
  // struct tm *tm = localtime(&t);
  // char s[64];
  // strftime(s, sizeof(s), "%c", tm);
  // printf("%s\n", s);
  return 0;
}