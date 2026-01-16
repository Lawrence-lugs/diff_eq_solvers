#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct Vector {
    size_t length;
    float* elements; 
} Vector;

typedef struct Matrix {
    size_t rows;
    size_t cols;
    float* elements;
} Matrix;

typedef struct Tensor {
    size_t length;
    size_t ndims;
    size_t* shape;
    float* elements;
} Tensor;

Tensor* tensor_create(size_t ndims, size_t* shape) {

    Tensor* tsr = malloc(sizeof(Tensor));
    if (!tsr) return NULL;

    size_t numElements = 1;
    for (size_t i=0; i<ndims; i++) {
        numElements *= shape[i]; 
    }

    tsr->ndims = ndims;
    tsr->length = numElements;
    tsr->elements = malloc(numElements*sizeof(float));
    tsr->shape = malloc(ndims*sizeof(size_t));  

    memcpy(tsr->shape,shape,ndims*sizeof(size_t));

    if (!tsr->elements || !tsr->shape) {
        free(tsr->elements);
        free(tsr->shape);
        return NULL;
    }

    return tsr;
}

void tensor_destroy(Tensor* tsr) {
    free(tsr->shape);
    free(tsr->elements);
    free(tsr);
}

void tensor_set_zero(Tensor* tsr) {
    memset(tsr->elements,0,tsr->length * sizeof(*tsr->elements));
}

size_t tensor_length(const Tensor* tsr) {
    size_t length;
    for(size_t i=0;i<tsr->ndims;i++) {
        length *= tsr->shape[i]; 
    }
    return length;
}

void tensor_copyto_index(Tensor* dst,const Tensor* src, size_t index) {
    if (dst->ndims != src->ndims-1) {
        char errMsg[128];
        sprintf(errMsg,
                "Tensor copy ndim mismatch (dst %zu != src %zu)\n",
                dst->ndims,
                src->ndims); 
        perror(errMsg);
        exit(1);
    }

    size_t offsetBytes = sizeof(float);
    for(size_t i=0;i<dst->ndims-1;i++) {
        offsetBytes *= dst->shape[i]; 
    }

    size_t sourceBytes = sizeof(float)*tensor_length(src);
    memcpy(dst->elements+offsetBytes,src->elements,sourceBytes);
}

void tensor_copyto(Tensor* dst,const Tensor* src){

    if (dst->ndims != src->ndims) {
        char errMsg[128];
        sprintf(errMsg,
                "Tensor copy ndim mismatch (dst %zu != src %zu)\n",
                dst->ndims,
                src->ndims); 
        perror(errMsg);
        exit(1);
    }

    for(size_t i = 0; i<dst->ndims; i++) {
        if(dst->shape[i] != src->shape[i]) {
            char errMsg[128];
            sprintf(errMsg,
                    "Tensor copy dim %zu mismatch (dst %zu != src %zu)\n",
                    i,
                    dst->shape[i],
                    src->shape[i]); 
            perror(errMsg);
            exit(1);
        }
    }

    size_t sourceBytes = sizeof(float)*tensor_length(src);
    memcpy(dst->elements,src->elements,sourceBytes);

    for(size_t i=0;i<dst->ndims;i++) {
        dst->shape[i] = src->shape[i];
    }
}

Tensor* tensor_copy(const Tensor* src){
    Tensor* dst = tensor_create(src->ndims, src->shape); 
    tensor_copyto(dst,src);
    return dst;
}

// TODO
void tensor_transpose(Tensor* dst, size_t* indices);

void tensor_sliceto(Tensor* dst, const Tensor* src, size_t index){
    if (dst->ndims-1 != src->ndims) {
        perror("Dims incompatible.\n");
        exit(1);
    }

    size_t sourceBytes = sizeof(float)*tensor_length(src);
    memcpy(dst->elements,src->elements,sourceBytes);
    for(size_t i=0;i<dst->ndims;i++) {
        dst->shape[i] = src->shape[i];
    }
}

Tensor* tensor_slice(const Tensor* src,size_t index){
    Tensor* dst = tensor_create(src->ndims-1, src->shape); 
    tensor_sliceto(dst,src,index);
    return dst;
}

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

void vector_scalar_multiply_copy(Vector* dst,const float scalar,const Vector* vec) {

    for (int i=0; i<vec->length; i++) {
        dst->elements[i] = vec->elements[i]*scalar;
    }

}
// Nearly a general array
int matrix_write_bin (const char* filename, const Matrix* mat) {

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

int tensor_write_bin (const char* filename, const Tensor* tensor) {

    FILE* file = fopen(filename,"wb");
    if (!file) {
        fprintf(stderr, "Failed to open file.");
        return 1;
    }
    
    // Start file with ndims
    fwrite(&tensor->ndims, sizeof(size_t), 1, file);

    // File starts with the shape
    for (size_t i=0; i<tensor->ndims; i++) {
        fwrite(&tensor->shape[i], sizeof(size_t), 1, file);
    }

    // Then, the payload
    fwrite(tensor->elements, sizeof(float), tensor->length, file);

    return 0;
}
