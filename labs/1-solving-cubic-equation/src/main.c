#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SEARCH_LIMIT 100000
#define BISECTION_LIMIT 1000

typedef struct {
    double a;
    double b;
    double c;
    double delta;
    double epsilon;
} CubicParams;

double f(double x, const CubicParams *p) {
    return x * x * x + p->a * x * x + p->b * x + p->c;
}

double compute_discriminant(const CubicParams *p) {
    return 4.0 * (p->a * p->a - 3.0 * p->b);
}

double bisection_method(double left, double right, const CubicParams *p) {
    if (left > right) {
        double tmp = left;
        left = right;
        right = tmp;
    }

    if (f(left, p) * f(right, p) > 0) {
        fprintf(stderr, "Error: no sign change on interval [%.3f, %.3f]\n", left, right);
        exit(EXIT_FAILURE);
    }

    double mid = 0.0;
    for (int i = 0; i < BISECTION_LIMIT; i++) {
        mid = (left + right) / 2.0;
        double fmid = f(mid, p);
        if (fabs(fmid) < p->epsilon)
            break;

        if (f(left, p) * fmid <= 0)
            right = mid;
        else
            left = mid;
    }

    return mid;
}

void find_interval_right(double start, double *left, double *right, const CubicParams *p) {
    double prev = start;
    for (int i = 1; i < SEARCH_LIMIT; i++) {
        double curr = start + i * p->delta;
        if (f(prev, p) * f(curr, p) <= 0) {
            *left = prev;
            *right = curr;
            return;
        }
        prev = curr;
    }
    fprintf(stderr, "Error: could not find right interval boundary.\n");
    exit(EXIT_FAILURE);
}

void find_interval_left(double start, double *left, double *right, const CubicParams *p) {
    double prev = start;
    for (int i = 1; i < SEARCH_LIMIT; i++) {
        double curr = start - i * p->delta;
        if (f(prev, p) * f(curr, p) <= 0) {
            *left = curr;
            *right = prev;
            return;
        }
        prev = curr;
    }
    fprintf(stderr, "Error: could not find left interval boundary.\n");
    exit(EXIT_FAILURE);
}

int solve_cubic(double *roots, const CubicParams *p) {
    int count = 0;
    double discriminant = compute_discriminant(p);

    if (discriminant <= 0) {
        if (fabs(f(0, p)) < p->epsilon) {
            roots[count++] = 0;
        } else if (f(0, p) < -p->epsilon) {
            double left, right;
            find_interval_right(0, &left, &right, p);
            roots[count++] = bisection_method(left, right, p);
        } else {
            double left, right;
            find_interval_left(0, &left, &right, p);
            roots[count++] = bisection_method(left, right, p);
        }
    } else {
        double sqrt_disc = sqrt(discriminant) / 2.0;
        double alpha = (-p->a - sqrt_disc) / 3.0;
        double beta = (-p->a + sqrt_disc) / 3.0;

        double f_alpha = f(alpha, p);
        double f_beta = f(beta, p);

        if (f_alpha > p->epsilon && f_beta < -p->epsilon) {
            double l1, r1, l2, r2;
            find_interval_left(alpha, &l1, &r1, p);
            roots[count++] = bisection_method(l1, r1, p);
            roots[count++] = bisection_method(alpha, beta, p);
            find_interval_right(beta, &l2, &r2, p);
            roots[count++] = bisection_method(l2, r2, p);
        } else if (f_alpha > p->epsilon && fabs(f_beta) < p->epsilon) {
            roots[count++] = beta;
            double left, right;
            find_interval_left(alpha, &left, &right, p);
            roots[count++] = bisection_method(left, right, p);
        } else if (fabs(f_alpha) < p->epsilon && f_beta < -p->epsilon) {
            roots[count++] = alpha;
            double left, right;
            find_interval_right(beta, &left, &right, p);
            roots[count++] = bisection_method(left, right, p);
        } else {
            double left, right;
            if (f_alpha > 0 && f_beta > 0)
                find_interval_left(alpha, &left, &right, p);
            else
                find_interval_right(beta, &left, &right, p);
            roots[count++] = bisection_method(left, right, p);
        }
    }

    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s a b c delta epsilon\n", argv[0]);
        return EXIT_FAILURE;
    }

    CubicParams params = {
        .a = atof(argv[1]),
        .b = atof(argv[2]),
        .c = atof(argv[3]),
        .delta = atof(argv[4]),
        .epsilon = atof(argv[5])
    };

    double roots[3];
    int num_roots = solve_cubic(roots, &params);

    printf("+---------------------------------------------------------------+\n");
    printf("|Roots found: %d\n", num_roots);
    for (int i = 0; i < num_roots; i++) {
        printf("|\tx = %.12f\t|f(x)| = %.12e\n", roots[i], fabs(f(roots[i], &params)));
    }
    printf("+---------------------------------------------------------------+\n\n\n");

    return EXIT_SUCCESS;
}
