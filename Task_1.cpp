

#include <iostream>
#include <cmath>
#include <iomanip>
#include <locale>
#include <chrono> 

using namespace std;
using namespace std::chrono;

# define M_PI      3.14159265358979323846

double func(double x)
{
	return 4 / (1 + pow(x, 2));
}
double Integral(double a, double b, int n, double y)
{
	return ((b - a) / (2 * n) * y);
}
int main()
{
	system("mode con cols=120 lines=50");
	setlocale(LC_ALL, "Rus");
	cout << "Задание 1" << endl;
	double a, b, y = 0, dy, In;
	a = 0.0;
	b = 1.0;

	int n[5] = { 100, 1000, 10000, 100000, 1000000 };
	int len_n = sizeof(n) / sizeof(int);

	for (int k = 0; k < len_n; k++)
	{
		y = 0;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		dy = (b - a) / n[k];
		y += func(a) + func(b);
		for (int ii = 1; ii < n[k]; ii++)
		{
			y += 2 * (func(a + dy * ii));
		}
		In = Integral(a, b, n[k], y);
		cout << "При n = " << n[k] << "  ||  ";
		cout << "Интеграл: " << setprecision(10) << In << "  ||  ";
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> duration = (t2 - t1);
		cout << "Дельта: " << M_PI - In << "  ||  ";
		cout << "Время выполнения: " << setprecision(10) << duration.count() << " seconds" << endl;
	}

	system("pause");
	return 0;
}

