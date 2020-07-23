#include <libconfig.h>
#include <structs.h>

config_t cfg, *cf;

int init_cfg(void)
{
  cf = &cfg;
  config_init(cf);

  if (!config_read_file(cf, "../config/bridge.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf),
            config_error_line(cf),
            config_error_text(cf));
    config_destroy(cf);
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

void load_bridge(bridge *a)
{
  config_lookup_int(cf, "weight", &a->weight);
  config_lookup_int(cf, "length", &a->length);
}

void load_alien(int *velocity, int *percentages)
{
  config_lookup_int(cf, "velocity", velocity);

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
}