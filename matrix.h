#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdio.h>

typedef double scalar;

typedef struct matrix {
    unsigned n1, n2;
    bool ok;
    scalar *data;
} matrix;

matrix matrix_create(unsigned n1, unsigned n2, scalar v);
void matrix_destroy(matrix m);
matrix matrix_identity(unsigned n);
scalar *matrix_get(matrix m, unsigned i, unsigned j);
matrix matrix_add(matrix m, matrix n);
void matrix_print(FILE *f, matrix m);
matrix matrix_mul_scalar(matrix m, scalar lambda);
matrix transposition(matrix m);
void matrix_pretty_print(FILE *f, matrix m);
matrix mult_scalar(matrix m, scalar l);
matrix mult_matrix(matrix m, matrix n);
void test_mult_scalar();
matrix fast_expon(matrix m, unsigned n);

#endif /* MATRIX_H */
