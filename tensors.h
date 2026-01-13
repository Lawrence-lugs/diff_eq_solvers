#ifndef __TENSORS_H__

#define __TENSORS_H__

#include <stdio.h>

typedef struct Vector {
    size_t length;
    float* elements; 
} Vector;

typedef struct Matrix {
    size_t rows;
    size_t cols;
    float* elements;
} Matrix;

void matrix_place_vector (Matrix* mat,const Vector* vec, size_t index);

float vector_dot_product(const Vector* veca, const Vector* vecb);

void vector_add (Vector* res,const Vector* veca,const Vector* vecb);

void vector_print (const Vector* vec,const char* name);

float matrix_get_element (const Matrix* mat, int i, int j);

void matrix_print (const Matrix* mat,const char* name);

void matrix_vector_multiply (Vector* res,const Matrix *mat,const Vector* vec);

int tensor_write_bin (const char* filename, const Matrix* mat);

void vector_scalar_multiply(const float scalar, Vector* vec);

#endif // ndef __TENSORS_H__
