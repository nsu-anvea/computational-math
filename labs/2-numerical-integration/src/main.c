#include <stdio.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

double f1(double x) {
    return -7.23 * x - 15;
}

double f2(double x) {
    return 1.6 * pow(x, 13) - 5 * pow(x, 8) - 102 * x * x + 23;
}

double f3(double x) {
    return sqrt(10 * x - 1);
}

double f4(double x) {
    return 519.0 / pow(41 * x + 1, 3);
}

double f5(double x) {
    return log(pow(x, 5) + 12);
}

double f6(double x) {
    return sin(2.0/3.0 * PI * x * x - 9.0/5.0 * PI * x + 14.7);
}

double f7(double x) {
    return 1.0 / (1.0 + exp(x));
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
    // Для метода Симпсона n должно быть четным
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

void calculate_integral(double (*f)(double), double a, double b, const char* func_name) {
    int n_values[] = {10, 20, 50, 100, 1000};
    int num_n = 5;
    
    printf("\n========================================\n");
    printf("Функция: %s\n", func_name);
    printf("Пределы интегрирования: [%.10g, %.10g]\n", a, b);
    printf("========================================\n\n");
    
    for (int i = 0; i < num_n; i++) {
        int n = n_values[i];
        
        printf("n = %d:\n", n);
        printf("  Левые прямоугольники:   %.17g\n", left_rectangles(f, a, b, n));
        printf("  Правые прямоугольники:  %.17g\n", right_rectangles(f, a, b, n));
        printf("  Средние прямоугольники: %.17g\n", middle_rectangles(f, a, b, n));
        printf("  Трапеции:               %.17g\n", trapezoid(f, a, b, n));
        printf("  Симпсон:                %.17g\n", simpson(f, a, b, n));
        printf("\n");
    }
}

int main() {
    // f(x) = -7.23*x - 15 от -12 до -9
    calculate_integral(f1, -12, -9, "f(x) = -7.23*x - 15");
    
    // f(x) = 1.6*x^13 - 5*x^8 - 102*x^2 + 23 от 5 до 9
    calculate_integral(f2, 5, 9, "f(x) = 1.6*x^13 - 5*x^8 - 102*x^2 + 23");
    
    // f(x) = sqrt(10*x - 1) от 1 до 2
    calculate_integral(f3, 1, 2, "f(x) = sqrt(10*x - 1)");
    
    // f(x) = 519/(41*x + 1)^3 от 0 до 3
    calculate_integral(f4, 0, 3, "f(x) = 519/(41*x + 1)^3");
    
    // f(x) = ln(x^5 + 12) от 3 до 5
    calculate_integral(f5, 3, 5, "f(x) = ln(x^5 + 12)");
    
    // f(x) = sin(2/3*Pi*x^2 - 9/5*Pi*x + 14.7) от -28*Pi/11 до 7*Pi
    calculate_integral(f6, -28.0 * PI / 11.0, 7.0 * PI, "f(x) = sin(2/3*Pi*x^2 - 9/5*Pi*x + 14.7)");
    
    // f(x) = 1/(1 + exp(x)) от 0 до 1
    calculate_integral(f7, 0, 1, "f(x) = 1/(1 + exp(x))");
    
    return 0;
}
