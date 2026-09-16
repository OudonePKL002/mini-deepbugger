#include <stdio.h>
#include <string.h>
#include <limits.h>


/* =========================================================
   1. Unsafe buffer copy
   ========================================================= */

void buffer_copy_bad(char *dst, const char *src) {
    strcpy(dst, src);
}

void buffer_copy_good(
    char *dst,
    const char *src,
    size_t size
) {
    if (size == 0) {
        return;
    }

    strncpy(dst, src, size - 1);
    dst[size - 1] = '\0';
}


/* =========================================================
   2. Array index
   ========================================================= */

int buffer_index_bad(
    int *arr,
    int index
) {
    return arr[index];
}

int buffer_index_good(
    int *arr,
    int index,
    int length
) {
    if (index < 0 || index >= length) {
        return -1;
    }

    return arr[index];
}


/* =========================================================
   3. Integer overflow
   ========================================================= */

int integer_overflow_bad(
    int a,
    int b
) {
    return a + b;
}

int integer_overflow_good(
    int a,
    int b
) {
    if (
        (b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)
    ) {
        return 0;
    }

    return a + b;
}


/* =========================================================
   4. Null pointer
   ========================================================= */

int null_pointer_bad(
    int *ptr
) {
    return *ptr;
}

int null_pointer_good(
    int *ptr
) {
    if (ptr == NULL) {
        return 0;
    }

    return *ptr;
}


/* =========================================================
   5. Division by zero
   ========================================================= */

int division_bad(
    int a,
    int b
) {
    return a / b;
}

int division_good(
    int a,
    int b
) {
    if (b == 0) {
        return 0;
    }

    return a / b;
}


/* =========================================================
   6. Unsafe format string
   ========================================================= */

void format_bad(
    const char *input
) {
    printf(input);
}

void format_good(
    const char *input
) {
    printf("%s", input);
}


/* =========================================================
   7. Signed range
   ========================================================= */

int signed_range_bad(
    int value
) {
    return value * 1000;
}

int signed_range_good(
    int value
) {
    if (
        value > INT_MAX / 1000 ||
        value < INT_MIN / 1000
    ) {
        return 0;
    }

    return value * 1000;
}


/* =========================================================
   8. Fixed-size array access
   ========================================================= */

int array_bounds_bad(
    int *arr,
    int index
) {
    return arr[index];
}

int array_bounds_good(
    int *arr,
    int index
) {
    if (index < 0 || index >= 10) {
        return -1;
    }

    return arr[index];
}

/* =========================================================
   9. Text copy
   Training analogue for copy semantics
   ========================================================= */

void text_copy_bad(
    char *output,
    const char *text
) {
    strcpy(output, text);
}


void text_copy_good(
    char *output,
    const char *text,
    size_t capacity
) {
    if (capacity == 0) {
        return;
    }

    strncpy(
        output,
        text,
        capacity - 1
    );

    output[
        capacity - 1
    ] = '\0';
}


/* =========================================================
   Main
   ========================================================= */

int main(void) {

    char dst[16] = {0};
    const char *src = "hello";

    int arr[10] = {
        0,1,2,3,4,5,6,7,8,9
    };

    int value = 10;

    buffer_copy_bad(dst, src);
    buffer_copy_good(
        dst,
        src,
        sizeof(dst)
    );

    buffer_index_bad(
        arr,
        2
    );

    buffer_index_good(
        arr,
        2,
        10
    );

    integer_overflow_bad(
        10,
        20
    );

    integer_overflow_good(
        10,
        20
    );

    null_pointer_bad(
        &value
    );

    null_pointer_good(
        &value
    );

    division_bad(
        10,
        2
    );

    division_good(
        10,
        2
    );

    format_bad(
        "hello"
    );

    format_good(
        "hello"
    );

    signed_range_bad(
        100
    );

    signed_range_good(
        100
    );

    array_bounds_bad(
        arr,
        2
    );

    array_bounds_good(
        arr,
        2
    );

    text_copy_bad(
        dst,
        src
    );

    text_copy_good(
        dst,
        src,
        sizeof(dst)
    );

    return 0;
}