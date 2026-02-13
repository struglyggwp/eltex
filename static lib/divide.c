#include "libcalc.h"

float divide(int a, int b)
{
	if (b == 0) {
		return 0;  // Ошибка
	}
	return (float)a / (float)b;
}