#include "eltex.h"
#include <stdio.h>
#include <string.h>


Command getUserCommand() {
  int input;
  scanf_s("%d", &input);

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
      printf("Íåâåðíàÿ êîìàíäà!\n");
      return getUserCommand(); 
  }
}


void printMenu() {
  printf("Ìåíþ:\n");
  printf("1. Äîáàâèòü àáîíåíòà\n");
  printf("2. Íàéòè àáîíåíòà\n");
  printf("3. Óäàëèòü àáîíåíòà\n");
  printf("4. Ïîêàçàòü âñåõ àáîíåíòîâ\n");
  printf("5. Âûéòè\n");
  printf("Âûáåðèòå êîìàíäó: ");
}

void executeCommand(Command cmd, struct abonent* a) {
  switch (cmd) {
    case ADD:
      printf("Äîáàâëÿåì àáîíåíòà...\n");
      input_abonent(a);
      break;
    case FIND:
      printf("Èùåì àáîíåíòà...\n");
      find_abonent(a);
      break;
    case DELETE_ABONENT:
      printf("Óäàëÿåì àáîíåíòà...\n");
      delete_abonent(a);
      break;
    case PRINT:
      printf("Ïå÷àòàåì âñåõ àáîíåíòîâ...\n");
      print_all_abonents(a);
      break;
    case EXIT:
      printf("Âûõîä èç ïðîãðàììû.\n");
      break;
  }
}


void input_abonent(struct abonent* a) {
  for (int i = 0; i < 100; i++) {
    if (a[i].name[0] == '\0') {
      printf("Ââåäèòå èìÿ àáîíåíòà\n");
      scanf_s("%9s", a[i].name, (unsigned)sizeof(a[i].name));
      printf("Ââåäèòå ôàìèëèþ àáîíåíòà\n");
      scanf_s("%9s", a[i].second_name, (unsigned)sizeof(a[i].second_name));
      printf("Ââåäèòå òåëåôîí àáîíåíòà\n");
      scanf_s("%9s", a[i].telephone, (unsigned)sizeof(a[i].telephone));
      return;
    } 
  }
    printf("Òåëåôîííàÿ êíèãà çàïîëíåíà!\n");
 }


void print_abonent(struct abonent a) {
  printf("Èìÿ: %s, Ôàìèëèÿ: %s, Òåëåôîí: %s\n", a.name, a.second_name,
         a.telephone);
}


void find_abonent(struct abonent* a) {
  char search_name[10];
  printf("Ââåäèòå èìÿ àáîíåíòà äëÿ ïîèñêà\n");
  scanf_s("%9s", search_name, (unsigned)sizeof(search_name));
  int found = 0;
  for (int i = 0; i < 100; i++) {
    if (strcmp(a[i].name, search_name) == 0) {
      print_abonent(a[i]);
      found = 1;
    }
  }
  if (!found) {
    printf("Àáîíåíò ñ èìåíåì %s íå íàéäåí\n", search_name);
  }
}

void delete_abonent(struct abonent* a) {
  char delete_name[10];
  printf("Ââåäèòå èìÿ àáîíåíòà äëÿ óäàëåíèÿ\n");
  scanf_s("%9s", delete_name, (unsigned)sizeof(delete_name));
  int found = 0;
  for (int i = 0; i < 100; i++) {
    if (strcmp(a[i].name, delete_name) == 0) {
      a[i].name[0] = '\0';
      a[i].second_name[0] = '\0';
      a[i].telephone[0] = '\0';
      
      
      found = 1;
      break;
    }
  }
  if (!found) {
    printf("Àáîíåíò ñ èìåíåì %s íå íàéäåí\n", delete_name);
  } else {
    printf("Àáîíåíò ñ èìåíåì %s óäàëåí\n", delete_name);
  }
}

void print_all_abonents(struct abonent* a) {
  for (int i = 0; i < 100; i++) {
    if (a[i].name[0] != '\0') {
      print_abonent(a[i]);
    }
  }

}
