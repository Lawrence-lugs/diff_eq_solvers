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

void sim_step (Vector* next_state, Matrix* update, Vector* state, Vector* force) {
    matrix_vector_multiply(next_state,update,state);
    vector_add(state,next_state,force);
    //vector_print(state,"State");
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

// Nearly a general array
int tensor_write_bin (const char* filename, const Matrix* mat) {

    FILE* file = fopen(filename,"wb");
    if (!file) {
        fprintf(stderr, "Failed to open file.");
        return 1;
    }
    
    // Start file with ndims
    uint32_t ndims = 1;
    fwrite(&ndims, sizeof(size_t), 1, file);

    // File starts with rows and cols
    fwrite(&mat->rows, sizeof(size_t), 1, file);
    fwrite(&mat->cols, sizeof(size_t), 1, file);

    // Then, the payload
    size_t matBufferLength = mat->rows*mat->cols;
    fwrite(mat->elements, sizeof(float), matBufferLength, file);

    return 0;
}

int main(void) {
    float dt=0.5e-3;
    float g=9.81;
    
    const int num_steps = 500; 
    printf("Number of steps: %i\n",num_steps);
    if (num_steps > 1000) {
        printf("Warning: simulating more than 1000 steps.\n");
    }

    Matrix result_matrix;
    float result_matrix_elements[num_steps*4];
    result_matrix.cols = 4;
    result_matrix.rows = num_steps;
    result_matrix.elements = result_matrix_elements;
        
    Vector state;
    float state_elements[4] = {
        0.,
        0.,
        1.,
        1.
    };
    state.length = 4;
    state.elements = state_elements;

    Matrix update_matrix;
    float update_matrix_elements[4*4] = {
        1,  0,  dt,  0 ,
        0,  1,  0 ,  dt,
        0,  0,  1 ,  0 ,
        0,  0,  0 ,  1 ,
    };
    update_matrix.rows = 4;
    update_matrix.cols = 4;
    update_matrix.elements = update_matrix_elements;

    Vector force_vector;
    float force_vector_elements[4] = {
        0,
        0,
        0,
        -g*dt
    };
    force_vector.length = 4;
    force_vector.elements = force_vector_elements;

    Vector next_state;
    float next_state_elements[4];
    next_state.elements = next_state_elements;
    next_state.length = 4;

    matrix_print(&update_matrix, "Update Matrix");
    vector_print(&force_vector, "Force Vector");
    vector_print(&state,"Initial State");

    for (size_t step=0;step<num_steps;step++) {
        sim_step(&next_state,&update_matrix,&state,&force_vector);
        matrix_place_vector(&result_matrix,&state,step);
    }

    matrix_print(&result_matrix,"Results");
    tensor_write_bin("forwardEulerResults.bin",&result_matrix);

};
