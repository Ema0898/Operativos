#include <structs.h>
#include <list.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <schedulers.h>

/* Method to update the bridge list */
void bridge_list_update(algs_params *params)
{
  /* variables to get time and list of aliens */
  struct timeval tic, toc;
  float time_difference, time_difference2;
  int list_size = llist_get_size(params->bridge);
  alien *alien_now;

  for (int i = 0; i < list_size; i++)
  {
    alien_now = (alien *)llist_get_by_index(params->bridge, i);
    if (alien_now == NULL)
    {
      break;
    }
    /* Get time toc */
    gettimeofday(&toc, NULL);
    tic = alien_now->last_update;
    /* Time total */
    time_difference = (double)(toc.tv_usec - tic.tv_usec) / 1000000 + (double)(toc.tv_sec - tic.tv_sec);
    alien_now->last_update = toc;
    alien_now->accumulator += time_difference;
    /* Calculate progress */
    alien_now->progress = alien_now->accumulator / alien_now->duration;

    /* Finish to pass bridge */
    if (alien_now->progress >= 1)
    {
      alien_now->working = 0;
      *params->weight_now -= alien_now->weight;
      llist_remove_by_index(params->bridge, i);
      break;
    }
    else
    {
      /* Condition of Round Robin */
      if (params->bridge_struct->bridge_type == ROUND_ROBIN)
      {
        time_difference2 = (double)(alien_now->last_update.tv_usec - alien_now->work_init_time.tv_usec) / 1000000 + (double)(alien_now->last_update.tv_sec - alien_now->work_init_time.tv_sec);
        /* Condition of quantum */
        if (params->bridge_struct->quantum <= time_difference2)
        {
          alien_now->rr_quantum = 1;
          llist_remove_by_index(params->bridge, i);
          *params->weight_now -= alien_now->weight;
          if (params->turn == 0)
          {
            llist_insert_end(params->north, alien_now);
          }
          else
          {
            llist_insert_end(params->south, alien_now);
          }
          alien_now->working = 0;
          break;
        }
      }
    }
  }
}

/* Algorithm Y */
void Y_algorithm(algs_params *params)
{

  /* Variables */
  int turn = 0;
  int i;
  int alien_id = 0;
  alien *alien_to_move;

  while (1)
  {
    /* Turns, Conditions if are not amount */
    if (turn == 0)
    {
      /* Condition if there are not enough aliens */
      if (llist_get_size(params->north) < params->amount_to_pass && llist_get_size(params->south) > llist_get_size(params->north))
      {
        turn = 1;
      }
    }
    else
    {
      /* Condition if there are not enough aliens */
      if (llist_get_size(params->south) < params->amount_to_pass && llist_get_size(params->north) > llist_get_size(params->south))
      {
        turn = 0;
      }
    }

    /* Logic of aliens to pass */
    for (i = 0; i < params->amount_to_pass; i++)
    {
      if (llist_get_size(params->north) == 0 && llist_get_size(params->south) == 0)
      {
        sleep(1);
      }

      if (turn == 0)
      {
        if (llist_get_size(params->north) == 0)
        {
          break;
        }
      }
      else
      {
        if (llist_get_size(params->south) == 0)
        {
          break;
        }
      }

      /* Weight of bridge */
      while (*params->weight_now + params->weight_alien > params->bridge_weight)
      {
        params->turn = turn;
        bridge_list_update(params);
      }

      if (turn == 0)
      {
        if (params->bridge_struct->bridge_type == 4)
        {
          alien_to_move = (alien *)lottery_winner(params->north);
        }
        else
        {
          alien_to_move = (alien *)llist_get_by_index(params->north, 0);
        }
      }
      else
      {
        /* if Lotery Scheduler */
        if (params->bridge_struct->bridge_type == 4)
        {
          alien_to_move = (alien *)lottery_winner(params->south);
        }
        else
        {
          alien_to_move = (alien *)llist_get_by_index(params->south, 0);
        }
      }

      if (turn == 0)
      {
        alien_id = llist_get_alien_index(params->north, alien_to_move->id);
        llist_remove_by_index(params->north, alien_id);
      }
      else
      {
        alien_id = llist_get_alien_index(params->south, alien_to_move->id);
        llist_remove_by_index(params->south, alien_id);
      }

      *params->weight_now += alien_to_move->weight;
      alien_to_move->working = 1;
      /* Update of time */
      gettimeofday(&alien_to_move->last_update, NULL);
      gettimeofday(&alien_to_move->work_init_time, NULL);
      llist_insert_end(params->bridge, alien_to_move);
      if (turn == 0)
      {
        *params->aliens_count_north += 1;
      }
      else
      {
        *params->aliens_count_south += 1;
      }
    }
    while (llist_get_size(params->bridge) > 0)
    {
      params->turn = turn;
      bridge_list_update(params);
    }

    /* change turn */
    turn = !turn;
    *params->aliens_count_north = 0;
    *params->aliens_count_south = 0;
  }
}

/* Survival ALgorithm */
void survival_algorithm(algs_params *params)
{
  /* Take the two lists and only pass the ones that can from each one to the algorithm Y */
  /* Egoistic */
  params->amount_to_pass = 1000;
  Y_algorithm(params);
}

/* Semaphore Algorithm */
void semaphore_algorithm(algs_params *params)
{
  /* variables */
  int *turn = params->turn_semaphore;
  int i;
  int alien_id = 0;

  struct timeval tic2, toc2;
  float time_difference2, accumulator;

  int sem_north_time = params->north_time, sem_south_time = params->south_time;

  params->amount_to_pass = 1;

  alien *alien_to_move;

  while (1)
  {
    /* get time */
    gettimeofday(&tic2, NULL);

    /* Logic of aliens to pass */
    for (i = 0; i < params->amount_to_pass; i++)
    {
      /* wait if any list has aliens */
      if (llist_get_size(params->north) == 0 && llist_get_size(params->south) == 0)
      {
        sleep(1);
      }
      if (*turn == 0)
      {
        /* North Aliens */
        if (llist_get_size(params->north) == 0)
        {
          break;
        };
        /* if lottery scheduler */
        if (params->bridge_struct->bridge_type == 4)
        {
          alien_to_move = (alien *)lottery_winner(params->north);
        }
        else
        {
          alien_to_move = (alien *)llist_get_by_index(params->north, 0);
        }
      }
      else
      {
        /* South Aliens */
        if (llist_get_size(params->south) == 0)
        {
          break;
        };
        if (params->bridge_struct->bridge_type == 4)
        {
          alien_to_move = (alien *)lottery_winner(params->south);
        }
        else
        {
          alien_to_move = (alien *)llist_get_by_index(params->south, 0);
        }
      }
      /* success pass */
      if (*params->weight_now + alien_to_move->weight <= params->bridge_weight)
      {
        if (*turn == 0)
        {
          alien_id = llist_get_alien_index(params->north, alien_to_move->id);
          llist_remove_by_index(params->north, alien_id);
        }
        else
        {
          alien_id = llist_get_alien_index(params->south, alien_to_move->id);
          llist_remove_by_index(params->south, alien_id);
        }
        *params->weight_now += alien_to_move->weight;
        alien_to_move->working = 1;
        /* get time */
        gettimeofday(&alien_to_move->last_update, NULL);
        gettimeofday(&alien_to_move->work_init_time, NULL);
        llist_insert_end(params->bridge, alien_to_move);
      }
    }

    gettimeofday(&toc2, NULL);
    /* get time */
    time_difference2 = (double)(toc2.tv_usec - tic2.tv_usec) / 1000000 + (double)(toc2.tv_sec - tic2.tv_sec);
    accumulator += time_difference2;

    params->turn = *turn;
    bridge_list_update(params);

    if (*turn == 0)
    {
      if (accumulator >= sem_north_time)
      {
        while (llist_get_size(params->bridge) > 0)
        {
          params->turn = *turn;
          bridge_list_update(params);
        }
        *turn = !*turn;
        accumulator = 0;
      }
    }
    else
    {
      if (accumulator >= sem_south_time)
      {
        while (llist_get_size(params->bridge) > 0)
        {
          params->turn = *turn;
          bridge_list_update(params);
        }

        /* change turn */
        *turn = !*turn;
        accumulator = 0;
      }
    }
  }
}
