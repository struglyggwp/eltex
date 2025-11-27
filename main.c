#include <stdio.h>
#include <windows.h>


// Функция для представления положительного десятичного числа в двоичном виде.

void check()
{
    unsigned int m;
    printf("Введите положительное целое число для представления его в двоичном виде\n");
    scanf_s("%u", &m);
    for (int i = (int)(sizeof(m)) * 8 - 1; i >= 0; i--)
    {
        printf("%u", (m >> i) & 1u);
    }
    printf("\n");
    return;
}

void check_minus()
{
    int u;
    printf("Введите отрицательное целое число для представления его в двоичном виде\n");
    scanf_s("%d", &u);
    unsigned int m = (unsigned int)u;
    for (int i = (int)(sizeof(m)) * 8 - 1; i >= 0; i--)
    {
        printf("%d", ((m >> i) & 1));
    }
    printf("\n");
    return;
}

void print_units() 
{
    int u;
    printf("Введите целое число для вывода кол-ва единиц в двоичном виде\n");
    scanf_s("%d", &u);
    unsigned int m = (unsigned int)u;
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

void change_third(int *m)
{
    unsigned int mask = 0x00FF0000;
    unsigned int u, m1;
    printf("Введите новое значение третьего байта в положительном целом числе\n");
    scanf_s("%u", &u);
    m1 = (unsigned int)*m;
    for (int i = (int)(sizeof(m1)) * 8 - 1; i >= 0; i--)
    {
        printf("%u", (m1 >> i) & 1u);
    }
    printf("\n");
    m1 = m1 & ~mask;
    m1 = m1 | (u << 16);
    *m = m1;
    for (int i = (int)(sizeof(*m)) * 8 - 1; i >= 0; i--)
    {
    
        printf("%u", (*m >> i) & 1u);
    }
}




int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    check();    // Задание №1
    check_minus();      // Задание №2
    print_units();      // Задание №3
    // Задание №4


    int m;
    change_third(&m);
}