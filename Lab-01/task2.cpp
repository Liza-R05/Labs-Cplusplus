#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    setlocale(LC_ALL, "RUS");
    const double EPSILON = 0.001;  // по условию
    double x1 = 1.0;
    double x2 = 17.5;
    double x3 = -0.35;

    double x, sum, term, pr_term;
    int n;

    cout << "Вычисление функции, заданной рядом: 2x + (2x)^2/2! + (2x)^3/3! + ...\n\n";

    // вычисление для x1 = 1
    x = x1;
    cout << "=== x = " << x << " ===\n";
    // первый член ряда: 2x / 1! = 2x
    term = 2 * x;
    sum = term;
    n = 1;

    cout << "Член 1 ряда равен " << term << "\n";
    cout << "Сумма после 1 члена равна " << sum << "\n";

    // следующие члены ряда
    do {
        pr_term = term;  // запоминаем предыдущий член
        n = n + 1;

        // вычисляем следующий член через предыдущий по формуле
        term = pr_term * (2 * x) / n;
        sum += term;

        double mod = term - pr_term;
        if (mod < 0) mod = -mod;  // модуль разности

        cout << "Член " << n << " ряда равен " << term << "\n";
        cout << "Сумма после " << n << " членов равна " << sum << "\n";
        cout << "Модуль разности между членом " << n << " и членом " << (n - 1) << " равен " << mod << "\n";

    } while ((term >= 0 ? term : -term) > EPSILON);  // пока |term| >= epsilon

    cout << "Итоговая сумма ряда для x = " << x << " равна " << sum << "\n\n";

    // вычисление для x2 = 17.5
    x = x2;
    cout << "=== x = " << x << " ===\n";

    term = 2 * x;
    sum = term;
    n = 1;

    cout << "Член 1 ряда равен " << term << "\n";
    cout << "Сумма после 1 члена равна " << sum << "\n";

    do {
        pr_term = term;
        n = n + 1;

        term = pr_term * (2 * x) / n;
        sum += term;

        double mod = term - pr_term;
        if (mod < 0) mod = -mod;

        cout << "Член " << n << " ряда равен " << term << "\n";
        cout << "Сумма после " << n << " членов равна " << sum << "\n";
        cout << "Модуль разности между членом " << n << " и членом " << (n - 1) << " равен " << mod << "\n";

    } while ((term >= 0 ? term : -term) > EPSILON);

    cout << "Итоговая сумма ряда для x = " << x << " равна " << sum << "\n\n";

    // вычисление для x3 = -0.35
    x = x3;
    cout << "=== x = " << x << " ===\n";

    term = 2 * x;
    sum = term;
    n = 1;

    cout << "Член 1 ряда равен " << term << "\n";
    cout << "Сумма после 1 члена равна " << sum << "\n";

    do {
        pr_term = term;
        n = n + 1;

        term = pr_term * (2 * x) / n;
        sum += term;

        double mod = term - pr_term;
        if (mod < 0) mod = -mod;

        cout << "Член " << n << " ряда равен " << term << "\n";
        cout << "Сумма после " << n << " членов равна " << sum << "\n";
        cout << "Модуль разности между членом " << n << " и членом " << (n - 1) << " равен " << mod << "\n";

    } while ((term >= 0 ? term : -term) > EPSILON);

    cout << "Итоговая сумма ряда для x = " << x << " равна " << sum << "\n\n";

    return 0;
}