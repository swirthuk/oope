﻿#include <iostream>
#include <stdio.h>

int main()
{
    setlocale(LC_ALL, "RU");
    float h = 0.5;
    float x = 0;
    int number = 1;
    while (x >= 0 && x <= 3) {
        float ans = 2 * x + 3;
        printf("| %i. | %.1f | %.3f |\n", number, x, ans);
        printf("--------------------\n");
        x += h;
        number++;
    }
    return 0;
    system("pause");
}