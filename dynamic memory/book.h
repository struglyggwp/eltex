#ifndef _BOOK_H_
#define _BOOK_H_



typedef enum { ADD = 1, FIND, DELETE_ABONENT, PRINT, EXIT } Command;

struct Node {
  char name[50];
  struct Node *prev;
  struct Node *next;
};


struct List {
  struct Node* head;
  struct Node* tail;
};
Command getUserCommand();


int free_list(struct List* list);
int add_abonent(struct List*,char*);
int find_abonent(struct List*,char * name);
int delete_abonent(struct List*,char *name);
int print_all_abonents(struct List*);
void printMenu();
int executeCommand(Command cmd, struct List*);


#endif  