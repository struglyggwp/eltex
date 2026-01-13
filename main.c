#include <stdio.h>
#include <windows.h>
#include "eltex.h"

int main(void) {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);

  /* // Задание №1

  int n = 124576;

  check((unsigned int)n);

  n = -4326;

  check_minus(n);

  n = 46536;
  print_units(n);

  unsigned int m;

  printf("Введите значение целого положительного числа\n");
  scanf_s("%u", &m);
  change_third(&m);

 //   */

  /* // Задание №2

quad_m();
swap_rows();
trungle_m();
snail();

// */

  /*  // 3адание №3

   int i = 18686743;

   third_bite(&i);
   two_eltex();
   print_massive();




   char str[100];
   char podstr[10];
   char* result;

   printf("Введите предложение\n");
   scanf_s("%99s", str, (unsigned)sizeof(str));

   printf("Введите подстроку\n");
   scanf_s("%9s", podstr, (unsigned)sizeof(podstr));

   result = find_podstring(str, podstr);

   if (result != NULL) {
       printf("Подстрока найдена\n");
       printf("Указатель: %p\n", (void*)result);
   }
   else {
       printf("Подстрока не найдена\n");
   }

 //  */

  // Задание №4-5 Справочник

    struct abonent a[100] = {0};

    Command cmd;

    do {
      printMenu();
      cmd = getUserCommand();
      executeCommand(cmd, a);
    } while (cmd != EXIT);

    return 0;
  }


