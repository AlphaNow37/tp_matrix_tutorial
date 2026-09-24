#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "matrix.h"

#define PRINT_PRECISION "6"
#define PRINT_DECIMAL_PRECISION "2"

matrix matrix_create(unsigned n1, unsigned n2, scalar v)
{
  matrix m={n1,n2,true,NULL};
  if(n1==0 || n2==0)
    return m;
  else if(SIZE_MAX / n1 <= n2)
    m.ok = false;
  else if (!(m.data=calloc(((size_t) n1)*n2, sizeof(scalar))))
    m.ok = false;
  else {
    for(unsigned i=0; i<n1; ++i)
      for(unsigned j=0; j<n2; ++j)
        *matrix_get(m,i,j) = v;
  }
 
  return m;
}

matrix matrix_identity(unsigned n)
{
  matrix m = matrix_create(n, n, 0.);
  if(!m.ok)
    return m;

  for(unsigned i=0; i<n; ++i)
    *matrix_get(m, i, i) = 1.;
  return m;
}

void matrix_destroy(matrix m)
{
  if(m.ok) {
    m.ok = false;
    free(m.data);
  }
}

scalar *matrix_get(matrix m, unsigned i, unsigned j)
{
  if(!m.ok || i>m.n1 || i>m.n2)
    return NULL;

  return &m.data[i*m.n2+j];
}

matrix matrix_add(matrix m, matrix n)
{
  matrix res={0,0,false,NULL};

  if(m.n1!=n.n1 || m.n2!=n.n2 || !m.ok || !n.ok)
    return res;

  res=matrix_create(m.n1, m.n2, 0.);
  for(unsigned i=0; i<m.n1; ++i)
    for(unsigned j=0; j<m.n2; ++j)
      *matrix_get(res, i, j) = *matrix_get(m, i, j) + *matrix_get(n, i, j);

  return res;
}

void matrix_print(FILE *f, matrix m)
{
  if(!m.ok)
    fprintf(f, "Invalid matrix\n");
  else {
    for(unsigned i=0; i<m.n1; ++i) {
      for(unsigned j=0; j<m.n2; ++j)
        fprintf(
            f, 
            "%"PRINT_PRECISION"."PRINT_DECIMAL_PRECISION"f ",
            *matrix_get(m, i, j));
      fprintf(f, "\n");
    }
  }
}

matrix matrix_mul_scalar(matrix m, scalar lambda) {
    matrix res = matrix_create(m.n1, m.n2, 0.);
    for (unsigned i = 0; i < m.n1; ++i)
        for (unsigned j = 0; j < m.n2; ++j)
            *matrix_get(res, i, j) = *matrix_get(m, i, j) * lambda;

    return res;
}
matrix transposition(matrix m) {
  matrix res = matrix_create(m.n2, m.n1, 0);
  for(int i = 0; i<m.n2; i++) {
    for(int j=0; j<m.n1; j++) {
      *matrix_get(res, i, j) = *matrix_get(m, j, i);
    }
  }
  return res;
}

matrix mult_scalar(matrix m, scalar l) {
  matrix res = matrix_create(m.n1, m.n2, 0);
  for(int i = 0; i<m.n1; i++) {
    for(int j = 0; j<m.n2; j++) {
      *matrix_get(res, i, j) = l*(*matrix_get(m, i, j));
    }
  }
  return res;
}

void matrix_pretty_print(FILE *f, matrix m) {
	fprintf(f, "┌ ");
	for (int i = 0; i < m.n1; i++) {
		fprintf(f, "       ");
	}
	fprintf(f, "┐\n");
	for (int j = 0; j < m.n2; j++) {
		fprintf(f, "│ ");
		for (int i = 0; i < m.n1; i++) {
			fprintf(
				f, 
				"%6.1f ",
				*matrix_get(m, i, j)
			);
		}
		fprintf(f, "│\n");
	}
	fprintf(f, "└ ");
	for (int i = 0; i < m.n1; i++) {
		fprintf(f, "       ");
	}
	fprintf(f, "┘\n");
}

matrix mult_matrix(matrix m, matrix n) {
  matrix res = matrix_create(m.n1, n.n2, 0);
  if(m.n2 != n.n1) {
    res.ok = false;
  } else {
    for(int i = 0; i<m.n1; i++) {
      for(int j =0; j<n.n2; j++) {
        for(int k = 0; k<m.n2; k++) {
          *matrix_get(res, i, j) += *matrix_get(m, i, k)*(*matrix_get(n, k, j));
        }
      }
    }
  }
  return res;
}

void test_mult_scalar() {
  matrix m = matrix_create(4, 5, 1);
  matrix res = mult_scalar(m, 25);

  for(int i = 0; i<m.n1; i++) {
    for(int j = 0; j<m.n2; j++) {
      if(*matrix_get(res, i, j) != 25) {
        printf("Test échoué :\nValeur attendue : 25\nValeur reçue : %d", *matrix_get(res, i, j));
      }
    }
  }
  matrix_destroy(res);
  matrix_destroy(m);
}

matrix fast_expon(matrix m, unsigned n) { //n>=1
  if(n <= 1)
    return m;
  if(n%2) {
    matrix res = fast_expon(m, n-1);
    matrix res2 = mult_matrix(res, m);
    matrix_destroy(m);
    matrix_destroy(res);
    return res2;
  } else {
    matrix res = mult_matrix(m, m);
    matrix_destroy(m);
    return fast_expon(res, n/2);
  }
}
