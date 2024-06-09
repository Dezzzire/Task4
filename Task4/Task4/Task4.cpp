#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <cmath>      // Для std::exp
#include <limits>     // Для std::numeric_limits

#ifndef NOMINMAX      // Проверяем, не определен ли уже NOMINMAX
#define NOMINMAX      // Отключает макросы min и max из Windows.h
#endif

#include <Windows.h>  // Для SetConsoleCP и SetConsoleOutputCP

// Константы
const double mu = 2.12; // Энергия Ферми, K
const double k = 1.69e-23; // постоянная Больцмана
const double eV_to_J = 1.6e-19; // переменная для перевода из эВ в Дж

// Функция f(x)
double f(double E, double mu, double T) {
    return 1 / (std::exp((E * eV_to_J - mu * eV_to_J) / (k * T)) + 1);
}

int main(int argc, char* argv[]) {
    // Проверка количества переданных аргументов командной строки
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " a b N" << std::endl;
        return 1;
    }
    SetConsoleCP(1251); // Установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // Установка кодовой страницы win-cp 1251 в поток вывода

    // Инициализация переменных из аргументов командной строки
    double a = std::stod(argv[1]);
    double b = std::stod(argv[2]);
    int N = std::stoi(argv[3]);
    double h = (b - a) / 1e3;

    // Поиск минимального и максимального значений y в процессе вычислений
    double min_y = std::numeric_limits<double>::max();
    double max_y = std::numeric_limits<double>::lowest();

    std::ofstream gnuplotScript("plot_script.gp");

    if (!gnuplotScript) {
        std::cerr << "Failed to create GNUplot script file." << std::endl;
        return 1;
    }

    gnuplotScript << "set terminal pngcairo enhanced\n";
    gnuplotScript << "set output 'plot.png'\n";
    gnuplotScript << "set xlabel 'Energy (eV)'\n";
    gnuplotScript << "set ylabel 'f(E)'\n";
    gnuplotScript << "set title 'Fermi-Dirac Distribution'\n";
    gnuplotScript << "plot \\\n";

    // Цикл по введенным значениям z
    for (int i = 0; i < N; ++i) {
        double z;
        std::cout << "Enter the value of z" << i + 1 << ": ";
        while (!(std::cin >> z)) {
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка флага ошибки
            std::cout << "Invalid input. Enter the value of z" << i + 1 << ": ";
        }

        std::vector<double> y_values;
        std::vector<double> x_values;

        // Вычисление значений y для каждого x в диапазоне от a до b
        for (double x = a; x <= b; x += h) {
            double y = f(x, mu, z);
            y_values.push_back(y);
            x_values.push_back(x);
        }

        // Поиск локальных минимального и максимального значений y
        double local_min_y = *std::min_element(y_values.begin(), y_values.end());
        double local_max_y = *std::max_element(y_values.begin(), y_values.end());

        min_y = std::min(min_y, local_min_y); // Обновляем min_y по мере прохождения цикла
        max_y = std::max(max_y, local_max_y); // Обновляем max_y по мере прохождения цикла

        // Запись результатов вычислений в файл
        std::string filename = std::to_string(i + 1) + ".txt";
        std::ofstream file(filename);

        // Проверка успешности открытия файла
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return 1;
        }

        file << "#z = " << z << "\n";
        for (size_t j = 0; j < x_values.size(); ++j) {
            file << std::fixed << std::setprecision(3) << x_values[j] << " " << y_values[j] << "\n";
        }

        gnuplotScript << "    '" << filename << "' using 1:2 with lines title 'z=" << z << "'";
        if (i < N - 1) {
            gnuplotScript << ", \\\n";
        }
        else {
            gnuplotScript << "\n";
        }
    }

    // Вывод общих минимального и максимального значений y
    std::cout << "Overall minimum value of y: " << std::fixed << std::setprecision(3) << min_y << "\n";
    std::cout << "Overall maximum value of y: " << std::fixed << std::setprecision(3) << max_y << "\n";

    gnuplotScript.close();

    // Запуск скрипта GNUplot
    system("gnuplot plot_script.gp");

    return 0;
}
