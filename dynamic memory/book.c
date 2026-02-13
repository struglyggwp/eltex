#include "book.h"
#include <stdio.h>   
#include <string.h> 
#include <stdlib.h>

int add_abonent(struct List* list, char* name) { 
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
  if (new_node == NULL) {
        printf("Memory allocation error!\n");
        return -1;
  }
  
     strncpy(new_node->name, name, sizeof(new_node->name) - 1);
  new_node->name[sizeof(new_node->name) - 1] = '\0';

    new_node->next = NULL;
    new_node->prev = NULL;

	if (list->head == NULL) {
      list->head = list->tail = new_node;

      return 0;
  
    }

    list->tail->next = new_node;
    new_node->prev = list->tail;
    list->tail = new_node;
    return 0;
  
}

int delete_abonent(struct List* list, char* name) {
  struct Node* current = list->head;
  if (current == NULL) {
    printf("The phone book is empty!\n");
    return 1;
  }
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      if (current == list->head) {
        if (list->head->next == NULL) {
          list->head = NULL;
          list->tail = NULL;
          free(current);
          return 0;
        }
        list->head = list->head->next;
        list->head->prev = NULL;
        free(current);
        return 0;
      }
      if (current == list->tail) {
        list->tail->prev->next = NULL;
        list->tail = current->prev;
        free(current);
        return 0;
      }
      current->prev->next = current->next;
      current->next->prev = current->prev;
      free(current);
      return 0;
    }
    current = current->next;
  }
  printf("Contact with name %s not found!\n", name);
  return 1;
}


int find_abonent(struct List* list,char* name) {
  struct Node* current = list->head;
  if (current == NULL) {
    printf("The phone book is empty!\n");
    return 1;
  }
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      printf("Contact found: %s\n", current->name);
      return 0;
    }
    current = current->next;
  }
  printf("Contact with name %s not found!\n", name);
  return 1;
}

int print_all_abonents(struct List* list) {
  struct Node* current = list->head;
  if (current == NULL) {
    printf("The phone book is empty!\n");
    return 1;
  }
  printf("Contact list:\n");
  while (current != NULL) {
    printf("- %s\n", current->name);
    current = current->next;
  }
  return 0;
}

void printMenu() {
  printf("Menu:\n");
  printf("1. Add Contact\n");
  printf("2. Find Contact\n");
  printf("3. Delete Contact\n");
  printf("4. Print all\n");
  printf("5. Exit\n");
  printf("Enter command: ");

}


Command getUserCommand() {
  int input;
  scanf("%d", &input);

  switch (input) {
    case 1:
      return ADD;
    case 2:
      return FIND;
    case 3:
      return DELETE_ABONENT;
    case 4:
      return PRINT;
    case 5:
      return EXIT;
    default:
      printf("Invalid command!\n");
      return getUserCommand(); 
  }
}

int free_list(struct List* list) {
    struct Node* current = list->head;
  while (current != NULL) {
    struct Node* temp = current;
    current = current->next;
    free(temp);
  }
  list->head = NULL;
  list->tail = NULL;
  return 0;
}


int executeCommand(Command cmd, struct List* list) {
  char name[50];

  switch (cmd) {
    case ADD:
      printf("Enter name to add: ");  
      scanf("%49s", name);
      return add_abonent(list, name);
    case FIND:
      printf("Enter name to find: "); 
      scanf("%49s", name);
      return find_abonent(list, name);
    case DELETE_ABONENT:
      printf("Enter name to delete: ");  
      scanf("%49s", name);
      return delete_abonent(list, name);
    case PRINT:
      return print_all_abonents(list);
    case EXIT:
      printf("Exiting program.\n");  
      free_list(list);
      return 0;
    default:
      printf("Invalid command!\n");  
      return 1;
  }
}
