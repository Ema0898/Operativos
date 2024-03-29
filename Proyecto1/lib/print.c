#include <stdio.h>

/**
 * Prints text in color
 * 1: Bold Red
 * 2: Bold Blue
 * 3: Bold Green
 * 4: Bold Yellow
 * 5: Bold Magenta
 * 6: Bold Cyan
*/
void printc(char *msg, int color)
{
  if (color == 1)
  {
    printf("\033[1;31m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 2)
  {
    printf("\033[1;34m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 3)
  {
    printf("\033[1;32m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 4)
  {
    printf("\033[1;33m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 5)
  {
    printf("\033[1;35m");
    printf("%s", msg);
    printf("\033[0m");
  }
  else if (color == 6)
  {
    printf("\033[1;36m");
    printf("%s", msg);
    printf("\033[0m");
  }
}

/* Print message when producer writes a message */ 
void print_producer_message(int index, int consumers, int producers)
{
  char index_print[12];
  char consumers_print[8];
  char producers_print[8];

  sprintf(index_print, "\t\t\t%d\n", index);
  sprintf(consumers_print, "\t%d\n", consumers);
  sprintf(producers_print, "\t%d\n", producers);

  printc("###########################\n", 2);
  printc("      Message Written\n", 6);
  printc("Index:", 2);
  printc(index_print, 3);
  printc("Alive Consumers:", 2);
  printc(consumers_print, 3);
  printc("Alive Producers:", 2);
  printc(producers_print, 3);
  printc("###########################\n", 2);
}

/* Print message when consumer reads a message */
void print_consumer_message(int index, int consumers, int producers, char *date, int pid, int magic_number)
{
  char index_print[12];
  char consumers_print[8];
  char producers_print[8];
  char date_print[35];
  char pid_print[12];
  char magic_print[10];

  sprintf(index_print, "\t\t\t%d\n", index);
  sprintf(consumers_print, "\t%d\n", consumers);
  sprintf(producers_print, "\t%d\n", producers);
  sprintf(date_print, "\t\t\t%s\n", date);
  sprintf(pid_print, "\t\t\t%d\n", pid);
  sprintf(magic_print, "\t\t%d\n", magic_number);

  printc("#################################################\n", 2);
  printc("      Message Read\n", 6);
  printc("Index:", 2);
  printc(index_print, 3);
  printc("Alive Consumers:", 2);
  printc(consumers_print, 3);
  printc("Alive Producers:", 2);
  printc(producers_print, 3);
  printc("Date:", 2);
  printc(date_print, 3);
  printc("PID:", 2);
  printc(pid_print, 3);
  printc("Magic Number:", 2);
  printc(magic_print, 3);
  printc("#################################################\n", 2);
}

/* Print message when consumer finish */
void print_consumer_end(int pid, int type, int messages, double waiting, double blocked, double user)
{
  char pid_print[12];
  char messages_print[8];
  char waiting_print[20];
  char blocked_print[20];
  char user_print[14];
  char *type_print;

  sprintf(pid_print, "\t\t\t%d\n", pid);
  sprintf(messages_print, "\t%d\n", messages);
  sprintf(waiting_print, "\t\t%fs\n", waiting);
  sprintf(blocked_print, "\t\t%fs\n", blocked);
  sprintf(user_print, "\t\t%fs\n", user);

  if (type == 0)
  {
    type_print = "\t\t\tMagic Number\n";
  }
  else if (type == 1)
  {
    type_print = "\t\t\tFinalizer\n";
  }

  printc("#####################################\n", 2);
  printc("         End Process\n", 6);
  printc("PID:", 2);
  printc(pid_print, 3);
  printc("Type:", 2);
  printc(type_print, 3);
  printc("Consumed Messages:", 2);
  printc(messages_print, 3);
  printc("Waiting Time:", 2);
  printc(waiting_print, 3);
  printc("Blocked Time:", 2);
  printc(blocked_print, 3);
  printc("User time:", 2);
  printc(user_print, 3);
  printc("#####################################\n", 2);
}

/* Print message when producer finish */
void print_producer_end(int pid, int messages, double waiting, double blocked, double kernel)
{
  char pid_print[12];
  char messages_print[8];
  char waiting_print[20];
  char blocked_print[20];
  char kernel_print[20];

  sprintf(pid_print, "\t\t\t%d\n", pid);
  sprintf(messages_print, "\t%d\n", messages);
  sprintf(waiting_print, "\t\t%fs\n", waiting);
  sprintf(blocked_print, "\t\t%fs\n", blocked);
  sprintf(kernel_print, "\t\t%fs\n", kernel);

  printc("#####################################\n", 2);
  printc("           End Process\n", 6);
  printc("PID:", 2);
  printc(pid_print, 3);
  printc("Produced Messages:", 2);
  printc(messages_print, 3);
  printc("Waiting Time:", 2);
  printc(waiting_print, 3);
  printc("Blocked Time:", 2);
  printc(blocked_print, 3);
  printc("Kernel Time:", 2);
  printc(kernel_print, 3);
  printc("#####################################\n", 2);
}

/* Print message when finalizer is executed */
void print_finalizer_end(int messages, int buffer, int consumers, int producers, int magic, double waiting, double blocked, double user, double kernel)
{
  char messages_print[14];
  char buffer_print[12];
  char consumer_print[14];
  char producer_print[14];
  char magic_print[10];
  char waiting_print[20];
  char blocked_print[20];
  char user_print[20];
  char kernel_print[20];

  sprintf(messages_print, "\t\t\t\t%d\n", messages);
  sprintf(buffer_print, "\t\t\t%d\n", buffer);
  sprintf(consumer_print, "\t\t\t\t%d\n", consumers);
  sprintf(producer_print, "\t\t\t\t%d\n", producers);
  sprintf(magic_print, "\t\t%d\n", magic);
  sprintf(waiting_print, "\t\t\t%fs\n", waiting);
  sprintf(blocked_print, "\t\t\t%fs\n", blocked);
  sprintf(user_print, "\t\t\t%fs\n", user);
  sprintf(kernel_print, "\t\t\t%fs\n", kernel);

  printc("######################################################\n", 2);
  printc("                 End System\n", 6);
  printc("Total Messages:", 2);
  printc(messages_print, 3);
  printc("Messages in Buffer:", 2);
  printc(buffer_print, 3);
  printc("Consumers:", 2);
  printc(consumer_print, 3);
  printc("Producers:", 2);
  printc(producer_print, 3);
  printc("Consumers End by Magic Number:", 2);
  printc(magic_print, 3);
  printc("Total Waiting Time:", 2);
  printc(waiting_print, 3);
  printc("Total Blocked Time:", 2);
  printc(blocked_print, 3);
  printc("Total User Time:", 2);
  printc(user_print, 3);
  printc("Total Kernel Time:", 2);
  printc(kernel_print, 3);
  printc("######################################################\n", 2);
}
