#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <limits.h>

typedef enum {
    ok,
    overflow,
    invalid_parameter,
    invalid_flag,
    invalid_eps,
    invalid_x
} status_code;

status_code check_parameters(int argc, char* argv[]) {
    if (argc != 4 || (argv[3][0] != '-')) {
        return invalid_parameter;
    }
    if (argv[3][2] || !argv[3][1]) {
        return invalid_flag;
    }

    double EPS = strtod(argv[1], NULL);
    if (EPS < 1E-15) {
        return invalid_eps;
    }

    double x = strtod(argv[2], NULL);
    if (!x && argv[2][0] != '0') {
        return invalid_x;
    }
}

status_code sum_a (double EPS, double x, double *res) {
    double sum = 1, cur = 1;
    int n = 1;
    do {
        cur *= x / n;
        if (DBL_MAX - cur < sum || INT_MAX - 1 == n) {
            return overflow;
        }
        sum += cur;
        n++;
    } while (fabs(cur) >= EPS);
    
    *res = sum;
    return ok;
}

status_code sum_b (double EPS, double x, double *res) {
    double sum = 1, cur = 1;
    int n = 0;
    do {
        n += 2;
        cur *= x * x / (n * (n - 1));
        if (-(DBL_MAX - cur) > sum) {
            return overflow;
        }
        sum -= cur;

        n += 2;
        cur *= x * x / (n * (n - 1));
        if (DBL_MAX - cur < sum || INT_MAX - 4 <= n) {
            return overflow;
        }
        sum += cur;
    } while (fabs(cur) >= EPS);
    
    *res = sum;
    return ok;
}

status_code sum_c (double EPS, double x, double *res) {
    double sum = 1, cur = 1;
    int n = 0;
    do {
        n++;
        cur *= 9 * pow(x, 2) * pow(n, 2) / (3 * n - 1) / (3 * n - 2);
        if (DBL_MAX - cur < sum || INT_MAX == n) {
            return overflow;
        }
        sum += cur;
    } while (fabs(cur) >= EPS);
    
    *res = sum;
    return ok;
}

status_code sum_d (double EPS, double x, double *res) {
    double sum, prev_sum, cur = 0.5 * pow(x, 2);
    sum = -cur;
    int n = 1;
    do {
        prev_sum = sum;
        n++;
        cur *= pow(x, 2) / (2 * n) * (2 * n - 1);
        if (DBL_MAX - cur < sum || INT_MAX - 1 == n) {
            return overflow;
        }
        sum += cur;
        n++;
        cur *= pow(x, 2) / (2 * n) * (2 * n - 1);
        if (-(DBL_MAX - cur) > sum) {
            return overflow;
        }
        sum -= cur;
    } while (fabs(sum - prev_sum) >= EPS);
    
    *res = sum;
    return ok;
}

int main(int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    switch (check_parameters(argc, argv)){
        case invalid_parameter:
            printf("Неверный ввод аргументов!\nВведите EPS x -ключ\n");
            return invalid_parameter;

        case invalid_flag:
            printf("Неверный ввод флага!\n");
            return invalid_flag;

        case invalid_eps:
            printf("Неверный ввод EPS\n");
            return invalid_eps;

        case invalid_x:
            printf("Неверный ввод x\n");
            return invalid_x;
    }
    double EPS = strtod(argv[1], NULL);
    double x = strtod(argv[2], NULL);
    double sum;

    switch (argv[3][1]) {
        case 'a':
            if (sum_a(EPS, x, &sum) == ok) {
                printf("%.15f", sum);
            } else {
                printf("Переполнение\n");
            }
            break;

        case 'b':
            if (sum_b(EPS, x, &sum) == ok) {
                printf("%.15f", sum);
            } else {
                printf("Переполнение\n");
            }
            break;

        case 'c':
            if (sum_c(EPS, x, &sum) == ok) {
                printf("%.15f", sum);
            } else {
                printf("Переполнение\n");
            }
            break;

        case 'd':
            if (sum_d(EPS, x, &sum) == ok) {
                printf("%.15f", sum);
            } else {
                printf("Переполнение\n");
            }
            break;
        
        default:
            printf("Неверный ввод флага!\n");
            break;
    }

    return 0;
}
