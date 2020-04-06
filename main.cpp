#include <iostream>
#include <cmath>
#include <iomanip>
#include <locale>
#include <chrono> 
#include <math.h>
//
#include <thread>
#include <mutex>
//

# define M_PI      3.14159265358979323846

using namespace std;
using namespace std::chrono;

double a, b, y, dy, In;
double timer_thread = 0;
double value_integral;
mutex gmutex;

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
void with_parall(double a, double b, int n)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	y = 0;
	dy = (b - a) / n;
	y += func(a) + func(b);
#pragma loop(hint_parallel(6))
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
void with_parall_without_vect(double a, double b, int n)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	y = 0;
	dy = (b - a) / n;
	y += func(a) + func(b);
#pragma loop(hint_parallel(6))
#pragma loop(no_vector)
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

void with_treads(double a, double b, int n)
{
	y = 0;
	dy = (b - a) / n;
	y += func(a) + func(b);

	for (int ii = 1; ii < n; ii++)
	{
		y += 2 * (func(a + dy * ii));
	}
	In = Integral(a, b, n, y);

	//	cout << "Интеграл: " << setprecision(10) << In << "   ";
	gmutex.lock();
	value_integral += In;

	gmutex.unlock();
}

int main()
{
	system("mode con cols=120 lines=50");
	setlocale(LC_ALL, "Rus");
	cout << "Задание 2" << endl;
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
	cout << "Выполнение с Qpar: " << endl;
	for (int k = 0; k < len_n; k++)
	{
		with_parall(a, b, n[k]);
	}

	cout << endl;
	cout << "Выполнение с отключением векторизации: " << endl;
	for (int k = 0; k < len_n; k++)
	{
		with_parall_without_vect(a, b, n[k]);
	}

	cout << endl;
	cout << "Выполнение при помощи thread: " << endl;
	for (int k = 0; k < len_n; k++)
	{

		cout << "При n = " << n[k] << "  ";
		value_integral = 0;
		timer_thread = 0;
		high_resolution_clock::time_point m1 = high_resolution_clock::now();

		thread t1(with_treads, 0, 0.25, n[k]);
		thread t2(with_treads, 0.25, 0.5, n[k]);
		thread t3(with_treads, 0.5, 0.75, n[k]);
		thread t4(with_treads, 0.75, 1, n[k]);
		//thread t5(with_treads, 0.8, 1, n[k]);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		//t5.join();
		high_resolution_clock::time_point m2 = high_resolution_clock::now();
		duration<double> duration = (m2 - m1);
		timer_thread += duration.count();
		cout << "Интеграл: " << setprecision(10) << value_integral << "   ";
		cout << "Дельта: " << M_PI - value_integral << "    ";
		cout << "Время выполнения:" << timer_thread << " seconds" << endl;
	}
}