#include <stdio.h>
#include <string.h>
#include <limits.h>


/* =========================================================
   W2-1. Alternative division guard
   ========================================================= */

int safe_divide_alt_bad(
    int numerator,
    int denominator
) {
    return numerator / denominator;
}


int safe_divide_alt_good(
    int numerator,
    int denominator
) {
    if (denominator == 0) {
        return -1;
    }

    return numerator / denominator;
}


/* =========================================================
   W2-2. Pointer write
   ========================================================= */

void pointer_write_bad(
    int *ptr,
    int value
) {
    *ptr = value;
}


void pointer_write_good(
    int *ptr,
    int value
) {
    if (ptr == NULL) {
        return;
    }

    *ptr = value;
}


/* =========================================================
   W2-3. Indexed array write
   ========================================================= */

void index_write_bad(
    int *array,
    int index,
    int value
) {
    array[index] = value;
}


void index_write_good(
    int *array,
    int index,
    int length,
    int value
) {
    if (
        index < 0 ||
        index >= length
    ) {
        return;
    }

    array[index] = value;
}


/* =========================================================
   W2-4. Length-controlled copy
   ========================================================= */

void length_copy_bad(
    char *destination,
    const char *source
) {
    strcpy(
        destination,
        source
    );
}


void length_copy_good(
    char *destination,
    const char *source,
    size_t destination_size
) {
    if (destination_size == 0) {
        return;
    }

    strncpy(
        destination,
        source,
        destination_size - 1
    );

    destination[
        destination_size - 1
    ] = '\0';
}


/* =========================================================
   W2-5. Multiplication range guard
   ========================================================= */

int multiplication_guard_bad(
    int value
) {
    return value * 5000;
}


int multiplication_guard_good(
    int value
) {
    if (
        value > INT_MAX / 5000 ||
        value < INT_MIN / 5000
    ) {
        return 0;
    }

    return value * 5000;
}


/* =========================================================
   W2-6. Format wrapper
   ========================================================= */

void format_wrapper_bad(
    const char *message
) {
    printf(message);
}


void format_wrapper_good(
    const char *message
) {
    printf(
        "%s",
        message
    );
}


/* =========================================================
   Main
   ========================================================= */

int main(void) {

    int value = 10;

    int array[8] = {
        0, 1, 2, 3,
        4, 5, 6, 7
    };

    char destination[32] = {0};

    const char *source = "hello";


    safe_divide_alt_bad(
        20,
        2
    );

    safe_divide_alt_good(
        20,
        2
    );


    pointer_write_bad(
        &value,
        100
    );

    pointer_write_good(
        &value,
        100
    );


    index_write_bad(
        array,
        2,
        99
    );

    index_write_good(
        array,
        2,
        8,
        99
    );


    length_copy_bad(
        destination,
        source
    );

    length_copy_good(
        destination,
        source,
        sizeof(destination)
    );


    multiplication_guard_bad(
        100
    );

    multiplication_guard_good(
        100
    );


    format_wrapper_bad(
        "hello"
    );

    format_wrapper_good(
        "hello"
    );


    return 0;
}