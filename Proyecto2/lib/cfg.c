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

void load_alien(int *velocity)
{
  config_lookup_int(cf, "velocity", velocity);
}

void quit_cfg(void)
{
  config_destroy(cf);
}