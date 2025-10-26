#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double f1(double x) {
    return -pow(x, 22) + 258 * pow(x, 11) - 1.85 * pow(x, 5) + 2025;
}

double f2(double x) {
    return tan(M_PI/8.0 * x) * cos(19.0*M_PI/4.0 * x - 3);
}

// Метод левых прямоугольников
double left_rectangles(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    
    return sum * h;
}

// Метод правых прямоугольников
double right_rectangles(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (int i = 1; i <= n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    
    return sum * h;
}

// Метод средних прямоугольников
double middle_rectangles(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;
    
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    
    return sum * h;
}

// Метод трапеций
double trapezoid(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }
    
    return sum * h;
}

// Метод Симпсона
double simpson(double (*f)(double), double a, double b, int n) {
    if (n % 2 != 0) n++;
    
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0) {
            sum += 2 * f(x);
        } else {
            sum += 4 * f(x);
        }
    }
    
    return sum * h / 3.0;
}

// Вычисление порядка сходимости по методу Рунге
// I_n - значение на сетке с n шагами
// I_2n - значение на сетке с 2n шагами
// I_4n - значение на сетке с 4n шагами
double runge_order(double I_n, double I_2n, double I_4n) {
    // p = log2(|I_n - I_2n| / |I_2n - I_4n|)
    double numerator = fabs(I_n - I_2n);
    double denominator = fabs(I_2n - I_4n);
    
    if (denominator < 1e-15) {
        return 0.0; // Избегаем деления на очень малое число
    }
    
    return log2(numerator / denominator);
}

// Уточненное значение интеграла по методу Рунге
double runge_refined(double I_2n, double I_4n, double p) {
    // I_уточн = I_4n + (I_4n - I_2n) / (2^p - 1)
    return I_4n + (I_4n - I_2n) / (pow(2, p) - 1);
}

void analyze_method(double (*method)(double (*)(double), double, double, int),
                    double (*f)(double), double a, double b,
                    const char* method_name, const char* func_name) {
    
    printf("\n--- %s ---\n", method_name);
    
    // Три сетки: n, 2n, 4n
    int n1 = 100;
    int n2 = 200;
    int n3 = 400;
    
    double I_n = method(f, a, b, n1);
    double I_2n = method(f, a, b, n2);
    double I_4n = method(f, a, b, n3);
    
    printf("I(n=%d)   = %.17g\n", n1, I_n);
    printf("I(n=%d)  = %.17g\n", n2, I_2n);
    printf("I(n=%d)  = %.17g\n", n3, I_4n);
    
    double p = runge_order(I_n, I_2n, I_4n);
    printf("Порядок точности p = %.17g\n", p);
    
    double I_refined = runge_refined(I_2n, I_4n, p);
    printf("Уточненное значение интеграла = %.17g\n", I_refined);
    
    // Оценка погрешности
    double error_estimate = fabs(I_4n - I_2n) / (pow(2, p) - 1);
    printf("Оценка погрешности = %.17g\n", error_estimate);
}

void analyze_function(double (*f)(double), double a, double b, const char* func_name) {
    printf("\n========================================\n");
    printf("Функция: %s\n", func_name);
    printf("Пределы интегрирования: [%.10g, %.10g]\n", a, b);
    printf("========================================\n");
    
    analyze_method(left_rectangles, f, a, b, "Метод левых прямоугольников", func_name);
    analyze_method(right_rectangles, f, a, b, "Метод правых прямоугольников", func_name);
    analyze_method(middle_rectangles, f, a, b, "Метод средних прямоугольников", func_name);
    analyze_method(trapezoid, f, a, b, "Метод трапеций", func_name);
    analyze_method(simpson, f, a, b, "Метод Симпсона", func_name);
}

int main() {
    printf("АНАЛИЗ ПОРЯДКОВ МЕТОДОВ ЧИСЛЕННОГО ИНТЕГРИРОВАНИЯ\n");
    printf("Метод Рунге на трех сетках (n=100, n=200, n=400)\n");
    
    // f(x) = -x^22 + 258*x^11 - 1.85*x^5 + 2025 от -2 до 1
    analyze_function(f1, -2, 1, "f(x) = -x^22 + 258*x^11 - 1.85*x^5 + 2025");
    
    // f(x) = tg(Pi/8*x)*cos(19*Pi/4*x - 3) от -3*Pi/7 до 11*Pi/2
    analyze_function(f2, -3.0*M_PI/7.0, 11.0*M_PI/2.0, 
                     "f(x) = tg(Pi/8*x)*cos(19*Pi/4*x - 3)");
    
    printf("\n========================================\n");
    printf("ТЕОРЕТИЧЕСКИЕ ПОРЯДКИ ТОЧНОСТИ:\n");
    printf("Левые/правые прямоугольники: p = 1\n");
    printf("Средние прямоугольники: p = 2\n");
    printf("Трапеции: p = 2\n");
    printf("Симпсон: p = 4\n");
    printf("========================================\n");
    
    return 0;
}