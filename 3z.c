#include <stdio.h>

// 3.1

void third_bite(int* i) {
  check(*i);
  unsigned char bite;
  printf("Введите третий байт который надо изменить в заданном числе\n");
  scanf_s("%hhu", &bite);
  unsigned char* u = (unsigned char*)i;
  u = u + 2;
  *u = bite;
  check(*i);
}

// 3.2

int two_eltex() {
  float x = 5.0;
  printf("x = %f, ", x);
  float y = 6.0;
  printf("y = %f\n", y);
  float* xp = &(float){x + 1};
  float* yp = &y;
  printf("Результат: %f\n", *xp + *yp);
}

// 3.3

void print_massive() {
  int m[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int* p = m;
  for (; p != m + 10; p++) {
    printf("%4d", *p);
  }
  printf("\n");
}

// 3.4

char* find_podstring(char* str, char* podstr) {
  char* i;
  char* j;

  for (i = str; *i != '\0'; i++) {
    j = podstr;

    while (*j != '\0' && *(i + (j - podstr)) == *j) {
      j++;
    }

    if (*j == '\0') {
      return i;  // указатель на начало подстроки
    }
  }

  return NULL;  // не найдена
}
