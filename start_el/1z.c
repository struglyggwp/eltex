#include <stdio.h>

// 1.1

void check(unsigned int m) {
  printf("ѕредставленное число в двоичном ввиде - ");
  for (int i = (int)(sizeof(m)) * 8 - 1; i >= 0; i--) {
    printf("%u", (m >> i) & 1);
  }
  printf("\n");
  return;
}

// 1.2

void check_minus(int d) {
  printf(
      "¬ведите отрицательное целое число дл€ представлени€ его в двоичном "
      "виде\n");
  scanf_s("%d", &d);
  unsigned int m = (unsigned int)d;
  for (int i = (int)(sizeof(m)) * 8 - 1; i >= 0; i--) {
    printf("%d", ((m >> i) & 1));
  }

  printf("\n");
  return;
}

// 1.3

void print_units(int d) {
  printf("¬ведите целое число дл€ вывода кол-ва единиц в двоичном виде\n");
  scanf_s("%d", &d);
  unsigned int m = (unsigned int)d;
  int count = 0;
  for (int i = 0; i < (int)(sizeof(m) * 8); i++) {
    if ((m >> i) & 1) {
      count++;
    }
  }

  printf("%d\n", count);
  return;
}

// 1.4

void change_third(int* m) {
  unsigned int mask = 0x00FF0000;
  unsigned int u, m1;
  printf("¬ведите новое значение третьего байта в положительном целом числе\n");
  scanf_s("%u", &u);
  m1 = (unsigned int)*m;
  printf("»значальное значение числа в двоичной форме - ");
  for (int i = (int)(sizeof(m1)) * 8 - 1; i >= 0; i--) {
    printf("%u", (m1 >> i) & 1);
  }
  printf("\n");
  m1 = m1 & ~mask;
  m1 = m1 | (u << 16);
  *m = m1;
  printf("»змененное значение числа в двоичной форме - ");
  for (int i = (int)(sizeof(*m)) * 8 - 1; i >= 0; i--) {
    printf("%u", (*m >> i) & 1u);
  }
  printf("\n");
}
