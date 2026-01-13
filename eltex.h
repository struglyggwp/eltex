#ifndef _ELTEX_H_
#define _ELTEX_H_

struct abonent {
  char name[10];
  char second_name[10];
  char telephone[10];
};

typedef enum { ADD = 1, FIND, DELETE_ABONENT, PRINT, EXIT } Command;


void check(unsigned int);
void check_minus(int);
void print_units(int);
void change_third(int*);
void quad_m();
void swap_rows();
void trungle_m();
int snail();
void two_eltex();
void print_massive();
char* find_podstring(char*, char*);
Command getUserCommand();
void printMenu();
void executeCommand(Command, struct abonent*);
void input_abonent(struct abonent*);
void print_abonent(struct abonent);
void find_abonent(struct abonent*);
void delete_abonent(struct abonent*);
void print_all_abonents(struct abonent*);

#endif  // _ELTEX_H_
