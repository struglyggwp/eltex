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
      printf("Неверная команда!\n");
      return getUserCommand();  // рекурсия до корректного ввода
  }
}


void printMenu() {
  printf("Меню:\n");
  printf("1. Добавить абонента\n");
  printf("2. Найти абонента\n");
  printf("3. Удалить абонента\n");
  printf("4. Показать всех абонентов\n");
  printf("5. Выйти\n");
  printf("Выберите команду: ");
}

void executeCommand(Command cmd, struct abonent* a) {
  switch (cmd) {
    case ADD:
      printf("Добавляем абонента...\n");
      input_abonent(a);
      break;
    case FIND:
      printf("Ищем абонента...\n");
      find_abonent(a);
      break;
    case DELETE_ABONENT:
      printf("Удаляем абонента...\n");
      delete_abonent(a);
      break;
    case PRINT:
      printf("Печатаем всех абонентов...\n");
      print_all_abonents(a);
      break;
    case EXIT:
      printf("Выход из программы.\n");
      break;
  }
}


void input_abonent(struct abonent* a) {
  for (int i = 0; i < 100; i++) {
    if (a[i].name[0] == '\0') {
      printf("Введите имя абонента\n");
      scanf_s("%9s", a[i].name, (unsigned)sizeof(a[i].name));
      printf("Введите фамилию абонента\n");
      scanf_s("%9s", a[i].second_name, (unsigned)sizeof(a[i].second_name));
      printf("Введите телефон абонента\n");
      scanf_s("%9s", a[i].telephone, (unsigned)sizeof(a[i].telephone));
      return;
    } 
  }
    printf("Телефонная книга заполнена!\n");
 }


void print_abonent(struct abonent a) {
  printf("Имя: %s, Фамилия: %s, Телефон: %s\n", a.name, a.second_name,
         a.telephone);
}


void find_abonent(struct abonent* a) {
  char search_name[10];
  printf("Введите имя абонента для поиска\n");
  scanf_s("%9s", search_name, (unsigned)sizeof(search_name));
  int found = 0;
  for (int i = 0; i < 100; i++) {
    if (strcmp(a[i].name, search_name) == 0) {
      print_abonent(a[i]);
      found = 1;
    }
  }
  if (!found) {
    printf("Абонент с именем %s не найден\n", search_name);
  }
}

void delete_abonent(struct abonent* a) {
  char delete_name[10];
  printf("Введите имя абонента для удаления\n");
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
    printf("Абонент с именем %s не найден\n", delete_name);
  } else {
    printf("Абонент с именем %s удален\n", delete_name);
  }
}

void print_all_abonents(struct abonent* a) {
  for (int i = 0; i < 100; i++) {
    if (a[i].name[0] != '\0') {
      print_abonent(a[i]);
    }
  }
}