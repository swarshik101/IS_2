#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <ctime>
#include <Windows.h>
#include <stdlib.h>
#include <math.h>
using namespace std;


HDC hDC = GetDC(GetConsoleWindow());//Создание окна для графика 
int smeshenie = 200;

double func(double x)
{
    return log(x + 1) - 2.25;
}

double mutation(double x0, double x1)  // мутация: генерация случайной величины
{
    const int NUM = 100000000;
    return fabs((double)((rand() * NUM) % (int)((x1 - x0) * NUM) + 1) / NUM) + x0;
}

double inversion(double x, double eps)  // инверсия: поиск в окрестностях точки
{
    static int sign = 0;
    sign++;
    sign %= 2;
    if (sign == 0) return x - eps;
    else return x + eps;
}

void crossover(double* x, double eps, double x0, double x1)  // кроссовер: среднее арифметическое
{
    int k = 99;
    for (int i = 0; i < 8; i++)
        for (int j = i + 1; j < 8; j++)
        {
            x[k] = (x[i] + x[j]) / 2;
            k--;
        }

    for (int i = 0; i < 8; i++)
    {
        x[k] = inversion(x[i], eps); k--;
        x[k] = inversion(x[i], eps); k--;
    }

    for (int i = 8; i < k; i++) x[i] = mutation(x0, x1);
}

void selection(double* x, double* y)  // селекция
{
    for (int i = 0; i < 100; i++)
        for (int j = i + 1; j < 100; j++)
            if (fabs(y[j]) < fabs(y[i]))
            {
                double temp = y[i];
                y[i] = y[j];
                y[j] = temp;
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
}

double genetic(double x0, double x1, double eps)  // поиск решения с использованием ГА
{
    double population[100];
    double f[100];
    int iter = 0;
    for (int i = 0; i < 100; i++)   // Формирование начальной популяции
    {
        population[i] = mutation(x0, x1);
        f[i] = func(population[i]);
    }

    selection(population, f);

    do
    {
        iter++;
        crossover(population, eps, x0, x1);
        for (int i = 0; i < 100; i++)
        {
            f[i] = func(population[i]);
            MoveToEx(hDC, 10 * f[i] + 100, -10 * (log(f[i] + 1) - 2.25) + 85 + smeshenie, NULL);//10 - scale //Постановка курсора
            LineTo(hDC, 10 * f[i] + 100, -10 * (log(f[i] + 1) - 2.25) + 85 + smeshenie);                     //Рисовка

            MoveToEx(hDC, 10 * population[i] + 100, -10 * (log(population[i] + 1) - 2.25) + 85 + smeshenie, NULL);//10 - scale
            LineTo(hDC, 10 * population[i] + 100, -10 * (log(population[i] + 1) - 2.25) + 85 + smeshenie);
        }
        selection(population, f);

    } while (fabs(f[0]) > eps && iter < 20000);

    cout << iter << " итераций" << endl;
    return population[0];
}

int main()
{

    HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));//Создание карандаша                             
    HPEN Pen1 = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hDC, Pen1);                              //Выбор карандаша
    MoveToEx(hDC, 0, 85 + smeshenie, NULL);               //Построение осей
    LineTo(hDC, 200, 85 + smeshenie);
    MoveToEx(hDC, 100, 0 + smeshenie, NULL);
    LineTo(hDC, 100, 170 + smeshenie);
    SelectObject(hDC, Pen);
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    cout << "Точка пересечения с ОХ: " << genetic(0, 100.0, 0.00001) << endl;
    cout << "Время работы программы: " << clock() << " мс\n";
    cin.get();
    return 0;
}