#include <stdio.h>
#include <windows.h>
#define N 5

//  2.1


void quad_m() 
{
	int matrix[N][N];
	int m = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) 
		{
			m += 1;
			matrix[i][j] = m;
			printf("%4d", matrix[i][j]);

		}
		printf("\n");
	}


}

// 2.2

void swap_rows(){
	int n[N] = { 1, 2, 3, 4, 5 };
	int n1[N];
	int m = N;
	int i2 = N - 1;
	for (int i = 0; i < N; i++)
	{
		printf("%4d", n[i]);
	}
	printf("\n");
	for(int i = 0; i < N; i++) 
	{
		n1[N - 1 - i] = n[i];
		
	}
	for (int i = 0; i < N; i++)
	{
		n[i] = n1[i];
		printf("%4d", n[i]);
	}
	printf("\n");
}

// 2.3

void trungle_m()
{
	int m[N][N];
	for (int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			if (j >= i)
			{
				m[i][j] = 1;
			}
			else 
			{
				m[i][j] = 0;
			}
			printf("%4d", m[i][j]);
		}
		printf("\n");
	}
	
}

// 2.4

int snail() {
	int n = N;
	int m[N][N] = { 0 };
	int i = 0, j = 0, num = 1;
	int dir = 0; // 0-вправо, 1-вниз, 2-влево, 3-вверх

	while (num <= n * n) {
		m[i][j] = num++;

		// Пробуем двигаться в текущем направлении
		if (dir == 0) j++;      // вправо
		else if (dir == 1) i++; // вниз
		else if (dir == 2) j--; // влево
		else if (dir == 3) i--; // вверх

		// Если упёрлись в границу или заполненную ячейку - откатываем и поворачиваем
		if (i < 0 || i >= n || j < 0 || j >= n || m[i][j] != 0) {
			// Откатываем последний шаг
			if (dir == 0) j--;
			else if (dir == 1) i--;
			else if (dir == 2) j++;
			else if (dir == 3) i++;

			// Поворачиваем
			dir = (dir + 1) % 4;

			// Делаем шаг в новом направлении
			if (dir == 0) j++;
			else if (dir == 1) i++;
			else if (dir == 2) j--;
			else if (dir == 3) i--;
		}
	}

	// Вывод
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%3d ", m[i][j]);
		}
		printf("\n");
	}

	return 0;
}