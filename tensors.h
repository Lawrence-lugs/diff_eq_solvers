#ifndef __TENSORS_H__

#define __TENSORS_H__

#include <stdio.h>

typedef struct Vector {
    size_t length;
    float* elements; 
} Vector;

#define V(v,i) ((v).elements[(i)])

typedef struct Matrix {
    size_t rows;
    size_t cols;
    float* elements;
} Matrix;

#define M(m,i,j) (matrix_get_element(m,i,j))

typedef struct Tensor {
    size_t ndims;
    size_t* shape;
    float* elements;
} Tensor;


void tensor_scalar_add(Tensor* tsr, float scalar);

void tensor_add(Tensor* tsrA, Tensor* tsrB);

void tensor_copyto_index(Tensor* dst,const Tensor* src, size_t index);

Tensor* tensor_create(size_t ndims, size_t* shape);

Tensor* tensor_copy(const Tensor* src);

void tensor_copyto(Tensor* dst, Tensor* src);

void tensor_transpose(Tensor* dst, size_t* indices);

Tensor* tensor_slice(const Tensor* src, size_t index);

void tensor_sliceto(Tensor* dst, const Tensor* src, size_t index);


size_t tensor_length(const Tensor* tsr);

void tensor_destroy(Tensor* tsr);

void tensor_set_zero(Tensor* tsr);

void matrix_place_vector (Matrix* mat,const Vector* vec, size_t index);

float vector_dot_product(const Vector* veca, const Vector* vecb);

void vector_add (Vector* res,const Vector* veca,const Vector* vecb);

void vector_print (const Vector* vec,const char* name);

float matrix_get_element (const Matrix* mat, int i, int j);

void matrix_print (const Matrix* mat,const char* name);

void matrix_vector_multiply (Vector* res,const Matrix *mat,const Vector* vec);

int matrix_write_bin (const char* filename, const Matrix* mat);

void vector_scalar_multiply(const float scalar, Vector* vec);

void vector_scalar_multiply_copy(Vector* dst,const float scalar,const Vector* vec);

int tensor_write_bin (const char* filename, const Tensor* tensor);

#endif // ndef __TENSORS_H__
