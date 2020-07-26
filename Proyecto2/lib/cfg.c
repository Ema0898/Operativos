#include <libconfig.h>
#include <structs.h>

config_t cfg, *cf;
config_t cfg1, *cf1;
config_t cfg2, *cf2;
config_t cfg3, *cf3;

int init_cfg(void)
{
  cf = &cfg;
  cf1 = &cfg1;
  cf2 = &cfg2;
  cf3 = &cfg3;

  config_init(cf);
  config_init(cf1);
  config_init(cf2);
  config_init(cf3);

  if (!config_read_file(cf1, "../config/right_bridge.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf1),
            config_error_line(cf1),
            config_error_text(cf1));
    config_destroy(cf1);
    printf("Can't read bridge configuration file\n");
    return 0;
  }

  if (!config_read_file(cf2, "../config/left_bridge.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf2),
            config_error_line(cf2),
            config_error_text(cf2));
    config_destroy(cf2);
    printf("Can't read bridge configuration file\n");
    return 0;
  }

  if (!config_read_file(cf3, "../config/center_bridge.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf3),
            config_error_line(cf3),
            config_error_text(cf3));
    config_destroy(cf3);
    printf("Can't read bridge configuration file\n");
    return 0;
  }

  if (!config_read_file(cf, "../config/alien.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf),
            config_error_line(cf),
            config_error_text(cf));
    config_destroy(cf);
    printf("Can't read alien configuration file\n");
    return 0;
  }

  return 1;
}

void load_bridge_right(configurable *a)
{
  config_lookup_int(cf1, "weight", &a->weight_confg);
  config_lookup_int(cf1, "length", &a->length_confg);
  config_lookup_int(cf1, "quantum", &a->quantum_confg);
  config_lookup_int(cf1, "scheduler", &a->scheduler_confg);
  config_lookup_int(cf1, "algorithm", &a->algorithm_confg);

  config_lookup_int(cf1, "semaphore_north", &a->semaphore_north_confg);
  config_lookup_int(cf1, "semaphore_south", &a->semaphore_south_confg);
  config_lookup_int(cf1, "y_algorithm", &a->y_algorithm_confg);

  config_lookup_int(cf1, "amount_north", &a->amount_north_confg);
  config_lookup_int(cf1, "amount_south", &a->amount_south_confg);
}

void load_bridge_left(configurable *a)
{
  config_lookup_int(cf2, "weight", &a->weight_confg);
  config_lookup_int(cf2, "length", &a->length_confg);
  config_lookup_int(cf2, "quantum", &a->quantum_confg);
  config_lookup_int(cf2, "scheduler", &a->scheduler_confg);
  config_lookup_int(cf2, "algorithm", &a->algorithm_confg);

  config_lookup_int(cf2, "semaphore_north", &a->semaphore_north_confg);
  config_lookup_int(cf2, "semaphore_south", &a->semaphore_south_confg);
  config_lookup_int(cf2, "y_algorithm", &a->y_algorithm_confg);

  config_lookup_int(cf2, "amount_north", &a->amount_north_confg);
  config_lookup_int(cf2, "amount_south", &a->amount_south_confg);
}

void load_bridge_center(configurable *a)
{
  config_lookup_int(cf3, "length", &a->length_confg);
  config_lookup_int(cf3, "quantum", &a->quantum_confg);
  config_lookup_int(cf3, "weight", &a->weight_confg);
  config_lookup_int(cf3, "scheduler", &a->scheduler_confg);
  config_lookup_int(cf3, "algorithm", &a->algorithm_confg);

  config_lookup_int(cf3, "semaphore_north", &a->semaphore_north_confg);
  config_lookup_int(cf3, "semaphore_south", &a->semaphore_south_confg);
  config_lookup_int(cf3, "y_algorithm", &a->y_algorithm_confg);

  config_lookup_int(cf3, "amount_north", &a->amount_north_confg);
  config_lookup_int(cf3, "amount_south", &a->amount_south_confg);
}

void load_alien(int *velocity, int *percentages, int *weight)
{
  config_lookup_int(cf, "velocity", velocity);
  config_lookup_int(cf, "weight", weight);

  config_lookup_int(cf, "alien_a_base", &percentages[0]);
  config_lookup_int(cf, "alien_a_alpha", &percentages[1]);
  config_lookup_int(cf, "alien_a_beta", &percentages[2]);

  config_lookup_int(cf, "alien_b_base", &percentages[3]);
  config_lookup_int(cf, "alien_b_alpha", &percentages[4]);
  config_lookup_int(cf, "alien_b_beta", &percentages[5]);
}

void quit_cfg(void)
{
  config_destroy(cf);
  config_destroy(cf1);
}