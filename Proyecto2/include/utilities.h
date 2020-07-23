#ifndef _UTILITIES_H
#define _UTILITIES_H

int load_map(int map[24][46]);
int split(char *str, char c, char ***arr);
int check_bin_dir(void);
char *concat(const char *s1, const char *s2);
int valdite_args(int argc, char *argv[], int *medium);
int is_number(char *text);
int generate_random(int upper, int lower);
double ran_expo(double lambda);
void generate_probability(int *a_probability, int *b_probability, int *percentages);

#endif