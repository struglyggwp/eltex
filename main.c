#include <stdio.h>
#include <windows.h>
//#include "2z.c"



// 1.1

void check()
{
    unsigned int m;
    printf("Введите положительное целое число для представления его в двоичном виде\n");
    scanf_s("%u", &m);
    for (int i = (int)(sizeof(m)) * 8 - 1; i >= 0; i--)
    {
        printf("%u", (m >> i) & 1);
    }
    printf("\n");
    return;
}

// 1.2

void check_minus()
{
    int d;
    printf("Введите отрицательное целое число для представления его в двоичном виде\n");
    scanf_s("%d", &d);
    unsigned int m = (unsigned int)d;
    for (int i = (int)(sizeof(m)) * 8 - 1; i >= 0; i--)
    {
        printf("%d", ((m >> i) & 1)); 
    }
    
    printf("\n");
    return;
}

// 1.3

void print_units() 
{
    int d;
    printf("Введите целое число для вывода кол-ва единиц в двоичном виде\n");
    scanf_s("%d", &d);
    unsigned int m = (unsigned int)d;
    int count = 0;
    for (int i = 0; i < (int)(sizeof(m) * 8); i++)
    {
        if ((m >> i) & 1) 
        {
            count++;
        }
    }
  
    printf("%d\n", count);
    return;
}

// 1.4

void change_third(int *m)
{
    unsigned int mask = 0x00FF0000;
    unsigned int u, m1;
    printf("Введите новое значение третьего байта в положительном целом числе\n");
    scanf_s("%u", &u);
    m1 = (unsigned int)*m;
    printf("Изначальное значение числа в двоичной форме - ");
    for (int i = (int)(sizeof(m1)) * 8 - 1; i >= 0; i--)
    {
        printf("%u", (m1 >> i) & 1u);
    }
    printf("\n");
    m1 = m1 & ~mask;
    m1 = m1 | (u << 16);
    *m = m1;
    printf("Измененное значение числа в двоичной форме - ");
    for (int i = (int)(sizeof(*m)) * 8 - 1; i >= 0; i--)
    {
        printf("%u", (*m >> i) & 1u);
    }
}




int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    check();              // Задание 1.1
    check_minus();        // Задание 1.2
    print_units();        // Задание 1.3
    unsigned int m;       // Задание 1.4
    printf("Введите значение целого положительного числа\n");    // Задание 1.4
    scanf_s("%u", &m);      // Задание 1.4
    change_third(&m);       // Задание 1.4
   // quad_m();             // Задание 2.1
   // swap_rows();          // Задание 2.2
   // trungle_m();          // Задание 2.3
   // snail();              // Задание 2.4
}