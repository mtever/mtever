#include <iostream>
#include <cmath>
#include <iomanip>
#include <locale>
#include <chrono> 

# define M_PI      3.14159265358979323846

using namespace std;
using namespace std::chrono;

double a, b, y, dy, In;
double value_integral;

double func(double x)
{
	return 4 / (1 + pow(x, 2));
}
double Integral(double a, double b, int n, double y)
{
	return ((b - a) / (2 * n) * y);
}

void without_parall(double a, double b, int n)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	y = 0;
	dy = (b - a) / n;
	y += func(a) + func(b);
	for (int ii = 1; ii < n; ii++)
	{
		y += 2 * (func(a + dy * ii));
	}

	In = Integral(a, b, n, y);
	cout << "При n = " << n << "  ";
	cout << "Интеграл: " << setprecision(10) << In << "   ";
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> duration = (t2 - t1);
	cout << "Дельта: " << M_PI - In << "    ";
	cout << "Время выполнения: " << duration.count() << " seconds" << endl;
}

int main()
{
	system("mode con cols=120 lines=50");
	setlocale(LC_ALL, "Rus");
	cout << "Задание 1" << endl;
	int n[5] = { 100, 1000, 10000, 100000, 1000000 };

	a = 0.0;
	b = 1.0;
	int len_n = sizeof(n) / sizeof(int);

	cout << "Линейное выполнение: " << endl;
	for (int k = 0; k < len_n; k++)
	{
		without_parall(a, b, n[k]);
	}

	cout << endl;
	return 0;
}