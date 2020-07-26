#ifndef _UTILITIES_H
#define _UTILITIES_H

/* library for general purpose functions */
int load_map(int map[24][46]);
int split(char *str, char c, char ***arr);
int check_bin_dir(void);
char *concat(const char *s1, const char *s2);
int valdite_args(int argc, char *argv[], int *medium);
int is_number(char *text);
int generate_random(int upper, int lower);
double ran_expo(double lambda);
void generate_probability(int *a_probability, int *b_probability, int *percentages);
void printc(char *msg, int color);
void print_instructions(void);

#endif