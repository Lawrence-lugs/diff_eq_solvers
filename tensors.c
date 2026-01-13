#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Vector {
    size_t length;
    float* elements; 
} Vector;

typedef struct Matrix {
    size_t rows;
    size_t cols;
    float* elements;
} Matrix;

float matrix_get_element (const Matrix* mat, int i, int j) {

    if (i > mat->rows || j > mat->cols) {
        // Don't know standard practice in raising error.
        fprintf(stderr, "[matrix_get_element] (%i,%i) out of range. Exiting.",i,j);
        exit(EXIT_FAILURE);
    };
    
    return mat->elements[mat->cols*i + j];
}

void vector_print (const Vector* vec,const char* name) {
    int i;

    printf("%s <%zu> : [\n",name,vec->length);
    for (i=0;i<vec->length;i++) {
        printf("  %f,\n",vec->elements[i]);
    };
    printf("]\n");
}

void matrix_print (const Matrix* mat,const char* name) {
    printf("%s <%zu,%zu>: [\n",name,mat->rows,mat->cols);
    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("  %f,\t",matrix_get_element(mat,i,j));
        }
        printf("\n");
    }
    printf("]\n");
}

void matrix_vector_multiply (Vector* res,const Matrix *mat,const Vector* vec) {
    if (mat->rows != vec->length) {
        fprintf(stderr,"mat rows != vector len! (%zu!=%zu)", mat->rows, vec->length);
        exit(EXIT_FAILURE);
    };

    int i,j;
    for (i = 0;i < res->length;i++) {
        res->elements[i] = 0.;
        for (j = 0;j < vec->length;j++) {
            res->elements[i] += vec->elements[j] * matrix_get_element(mat, i, j);
            //printf("[%f,%f]\n",vec->elements[j],matrix_get_element(mat, i, j));
        };
    };
};

void vector_add (Vector* res,const Vector* veca,const Vector* vecb) {

    if (veca->length != vecb->length) {
        fprintf(stderr,"vector length mismatch (%zu!=%zu)",veca->length,vecb->length);
        exit(EXIT_FAILURE);
    }

    for (size_t i=0; i<veca->length; i++) {
        res->elements[i] = veca->elements[i] + vecb->elements[i];
    }

}

void matrix_place_vector (Matrix* mat,const Vector* vec, size_t index) {
    
    if (mat->cols != vec->length) {
        fprintf(stderr,"mat cols != vector len! (%zu!=%zu)", mat->rows, vec->length);
        exit(EXIT_FAILURE);
    };

    for (size_t j=0; j < vec->length; j++) {
        mat->elements[index*mat->cols + j] = vec->elements[j];
    }
    
}

void vector_scalar_multiply(const float scalar, Vector* vec) {

    for (int i=0; i<vec->length; i++) {
        vec->elements[i] = vec->elements[i]*scalar;
    }

}

// Nearly a general array
int tensor_write_bin (const char* filename, const Matrix* mat) {

    FILE* file = fopen(filename,"wb");
    if (!file) {
        fprintf(stderr, "Failed to open file.");
        return 1;
    }
    
    // Start file with ndims
    size_t ndims = 2;
    fwrite(&ndims, sizeof(size_t), 1, file);

    // File starts with rows and cols
    fwrite(&mat->rows, sizeof(size_t), 1, file);
    fwrite(&mat->cols, sizeof(size_t), 1, file);

    // Then, the payload
    size_t matBufferLength = mat->rows*mat->cols;
    fwrite(mat->elements, sizeof(float), matBufferLength, file);

    return 0;
}

float vector_dot_product(const Vector* veca, const Vector* vecb) {

    if (veca->length != vecb->length) {
        fprintf(stderr,"vector length mismatch (%zu!=%zu)",veca->length,vecb->length);
        exit(EXIT_FAILURE);
    }

    float res = 0.;
    for (size_t i=0; i<veca->length; i++) {
        res += veca->elements[i]*vecb->elements[i];
    }

    return res;
}
    
float vector_norm(const Vector* vec) {

    return vector_dot_product(vec, vec); 

};
