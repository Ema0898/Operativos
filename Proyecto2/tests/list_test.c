#include <stdio.h>
#include <structs.h>
#include <linked_list.h>

int main()
{
  node_t *head = NULL;

  add_start(&head, 1);
  add_end(head, 2);
  add_end(head, 3);
  add_end(head, 4);
  add_end(head, 5);

  printf("Original List\n");
  print_list(head);

  remove_last(head);

  printf("New List\n");

  print_list(head);

  printf("List SIZE = %d\n", get_size());

  return 0;
}