#include <stdio.h>
#include <math.h>
#include <time.h>
 
ddouble U_Random ();
int possion ();
 
 
void main () {
  double u = U_Random ();
  int p = possion ();
  printf("% fn", u );
  printf("% dn", p );
}
 
int possion ()/* generates a random number with a Poisson distribution. Lamda is the average number */
{
  int lambda = 20, k = 0;
  long double p = 1.0;
  long double l = exp (-Lambda);/* it is defined as long double for precision, and exp (-Lambda) is a decimal near 0 */
  printf ("%. 15Lfn", l );
  while (p >= l) {
    double u = U_Random ();
    P * = u;
    K ++;
  }
  rreturn K-1;
}
 
ddouble u_random ()/* generates a 0 ~ Random number between 1 */{
  d  double f;
  s  srand (unsigned) time (NULL));
  f = (float) (rand () % 100 );
  return f/100;
}