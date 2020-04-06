#define _HAS_CONDITIONAL_EXPLICIT 0
#include <iostream>
#include <cmath>
#include <iomanip>
#include <locale>
#include <chrono> 

#include <math.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <tbb/tbb.h>
#include <mutex>   

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

void compute(double a, double b, int n)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	dy = (b - a) / n;
	//y += func(a) + func(b);
	cilk::reducer_opadd <double> y(func(a) + func(b));
	tbb::parallel_for(int(0), n, [&](size_t ii)
	{
		gmutex.lock();
		y += 2 * (func(a + dy * ii));
		gmutex.unlock();
	});
	double res = y.get_value();
	In = Integral(a, b, n, res);
	
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
	cout << "Задание 4" << endl;
	int n[5] = { 100, 1000, 10000, 100000, 1000000 };

	a = 0.0;
	b = 1.0;
	int len_n = sizeof(n) / sizeof(int);

	cout << "\nВыполнение при помощи TBB: " << endl;
	for (int k = 0; k < len_n; k++)
	{
		compute(a, b, n[k]);
	}

	cout << endl;
	return 0;
}

