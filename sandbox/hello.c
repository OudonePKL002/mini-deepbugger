#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int add_similar(int x, int y)
{
    return x + y;
}

int multiply(int a, int b)
{
    return a * b;
}

int subtract(int a, int b)
{
    return a - b;
}

int maximum(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int minimum(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int is_positive(int x)
{
    if (x > 0)
        return 1;
    return 0;
}

int is_negative(int x)
{
    if (x < 0)
        return 1;
    return 0;
}

int identity(int x)
{
    return x;
}

int subtract_similar(int x, int y)
{
    return x - y;
}

int multiply_similar(int x, int y)
{
    return x * y;
}

int maximum_similar(int x, int y)
{
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

int minimum_similar(int x, int y)
{
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

int is_positive_similar(int value)
{
    return value > 0;
}

int is_negative_similar(int value)
{
    return value < 0;
}

int identity_similar(int value)
{
    return value;
}

int is_zero(int x)
{
    return x == 0;
}


int is_nonzero(int x)
{
    return x != 0;
}


int greater_than_ten(int x)
{
    return x > 10;
}


int sign_bit(int x)
{
    unsigned int ux = (unsigned int)x;

    return (ux >> 31) & 1;
}

int is_sum_positive(int a, int b)
{
    int sum = a + b;

    if (sum > 0)
        return 1;

    return 0;
}

int absolute_value(int x)
{
    return x < 0 ? -x : x;
}

int square(int x)
{
    return x * x;
}

int is_even(int x)
{
    return (x % 2) == 0;
}

int is_odd(int x)
{
    return (x % 2) != 0;
}

int average_two(int a, int b)
{
    return (a + b) / 2;
}

int max_three(int a, int b, int c)
{
    int m = a > b ? a : b;
    return m > c ? m : c;
}

int min_three(int a, int b, int c)
{
    int m = a < b ? a : b;
    return m < c ? m : c;
}

int in_range(int x, int low, int high)
{
    return x >= low && x <= high;
}

int clamp_zero_hundred(int x)
{
    if (x < 0)
        return 0;

    if (x > 100)
        return 100;

    return x;
}

int sum_three(int a, int b, int c)
{
    return a + b + c;
}

int main(void)
{
    int r1 = add(10, 20);
    int r2 = add_similar(10, 20);
    int r3 = multiply(10, 20);
    int r4 = subtract(10, 20);
    int r5 = maximum(10, 20);
    int r6 = minimum(10, 20);
    int r7 = is_positive(10);
    int r8 = is_negative(10);
    int r9 = identity(10);
    int r10 = subtract_similar(10, 20);
    int r11 = multiply_similar(10, 20);
    int r12 = maximum_similar(10, 20);
    int r13 = minimum_similar(10, 20);
    int r14 = is_positive_similar(10);
    int r15 = is_negative_similar(10);
    int r16 = identity_similar(10);
    int r17 = is_zero(10);
    int r18 = is_nonzero(10);
    int r19 = greater_than_ten(10);
    int r20 = sign_bit(10);
    int r21 = is_sum_positive(10, 20);
    int r22 = absolute_value(10);
    int r23 = square(10);
    int r24 = is_even(10);
    int r25 = is_odd(10);
    int r26 = average_two(10, 20);
    int r27 = max_three(10, 20, 30);
    int r28 = min_three(10, 20, 30);
    int r29 = in_range(10, 20, 30);
    int r30 = clamp_zero_hundred(10);
    int r31 = sum_three(10, 20, 30);

    printf("add: %d, add_similar: %d, multiply=%d\n", r1, r2, r3);

    return 0;
}