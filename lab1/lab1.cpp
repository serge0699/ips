#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <locale.h>
#include <cstdlib>

using namespace std;

double Func(double x)
{
	double F;
	F = 4 / (1 + pow(x, 2));
	return F;
}

double rect(double (*Function)(double), double a, double b, int n)
{
	double sum = 0;
	double h = (b - a) / n;
	for (int i = 0; i < n; i++) {
		sum += Function(a + h * (i + 0.5));
	}
	sum *= h;
	return sum;
}

double rect_nov(double (*Function)(double), double a, double b, int n)
{
	double sum = 0;
	double h = (b - a) / n;
#pragma loop(no_vector)
	for (int i = 0; i < n; i++) {
		sum += Function(a + h * (i + 0.5));
	}
	sum *= h;
	return sum;
}

double rect_qpar(double (*Function)(double), double a, double b, int n)
{
	double sum = 0;
	double h = (b - a) / n;
#pragma loop(hint_parallel(4))
	for (int i = 0; i < n; i++) {
		sum += Function(a + h * (i + 0.5));
	}
	sum *= h;
	return sum;
}

void rect_thr(double (*Function)(double), double a, double b, int n, double& sum)
{
	sum = 0;
	double h = (b - a) / n;
	for (int i = 0; i < n; i++) {
		sum += Function(a + h * (i + 0.5));
	}
	sum *= h;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	system("chcp 1251");

	// Границы интегрирования.
	double a = 0;
	double b = 1;


	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
	cout << "Метод центральных прямоугольников:" << endl;
	cout << std::setprecision(10) << rect(&Func, a, b, 100) << endl;
	cout << rect(&Func, a, b, 1000) << endl;
	cout << rect(&Func, a, b, 10000) << endl;
	cout << rect(&Func, a, b, 100000) << endl;
	cout << rect(&Func, a, b, 1000000) << endl;
	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();

	chrono::duration <double> duration = (t2 - t1);
	cout << "Время подсчёта методом центральных прямоугольников: " << duration.count() << " секунд." << endl << endl;



	t1 = chrono::high_resolution_clock::now();
	cout << "Метод центральных прямоугольников c отключённой векторизацией:" << endl;
	cout << rect_nov(&Func, a, b, 100) << endl;
	cout << rect_nov(&Func, a, b, 1000) << endl;
	cout << rect_nov(&Func, a, b, 10000) << endl;
	cout << rect_nov(&Func, a, b, 100000) << endl;
	cout << rect_nov(&Func, a, b, 1000000) << endl;
	t2 = chrono::high_resolution_clock::now();

	duration = (t2 - t1);
	cout << "Время подсчёта методом центральных прямоугольников с отключённой векторизацией: " << duration.count() << " секунд." << endl << endl;



	t1 = chrono::high_resolution_clock::now();
	cout << "Метод центральных прямоугольников с параллелизацией на 4-х потоках с автоматическим параллелизатором:" << endl;
	cout << rect_qpar(&Func, a, b, 100) << endl;
	cout << rect_qpar(&Func, a, b, 1000) << endl;
	cout << rect_qpar(&Func, a, b, 10000) << endl;
	cout << rect_qpar(&Func, a, b, 100000) << endl;
	cout << rect_qpar(&Func, a, b, 1000000) << endl;
	t2 = chrono::high_resolution_clock::now();

	duration = (t2 - t1);
	cout << "Время подсчёта методом центральных прямоугольников с параллелизацией на 4-х потоках с автоматическим параллелизатором: " << duration.count() << " секунд." << endl << endl;



	cout << "Метод центральных прямоугольников на 4-х потоках:" << endl;
	double area1 = 0;
	double area2 = 0;
	double area3 = 0;
	double area4 = 0;
	t1 = chrono::high_resolution_clock::now();
	std::thread thr51(rect_thr, &Func, 0.00, 0.25, 1000000, std::ref(area1));
	std::thread thr52(rect_thr, &Func, 0.25, 0.50, 1000000, std::ref(area2));
	std::thread thr53(rect_thr, &Func, 0.50, 0.75, 1000000, std::ref(area3));
	std::thread thr54(rect_thr, &Func, 0.75, 1.00, 1000000, std::ref(area4));
	thr51.join();
	thr52.join();
	thr53.join();
	thr54.join();
	double area = area1 + area2 + area3 + area4;
	cout << area << endl;
	t2 = chrono::high_resolution_clock::now();

	duration = (t2 - t1);
	cout << "Время подсчёта методом центральных прямоугольников на 4-х потоках: " << duration.count() << " секунд." << endl << endl;

	return 0;
}