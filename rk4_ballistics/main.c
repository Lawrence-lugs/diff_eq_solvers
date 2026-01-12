#include <stdio.h>
#include <stdlib.h>

// Not sure if it's a good idea to name these such a generic name
// How do we handle other datatypes of vectors?
typedef struct vector {
    size_t length;
    float* elements; 
} vector_t;

typedef struct matrix {
    size_t rows;
    size_t cols;
    float* elements;
} matrix_t;

// Any naming scheme for this to keep it local? 
// There aren't any classes in C, but I've heard 
// significant criticism of C++ anyway.
float matrix_get_element (matrix_t* mat, int i, int j) {

    if (i > mat->rows || j > mat->cols) {
        // Don't know standard practice in raising error.
        fprintf(stderr, "[matrix_get_element] (%i,%i) out of range. Exiting.",i,j);
        exit(EXIT_FAILURE);
    };
    
    return mat->elements[mat->cols*i + j];
}

void vector_print (vector_t* vec, char* name) {
    int i;

    printf("%s <%zu> : [\n",name,vec->length);
    for (i=0;i<vec->length;i++) {
        printf("  %f,\n",vec->elements[i]);
    };
    printf("]\n");
}

void matrix_print (matrix_t* mat, char* name) {
    printf("%s <%zu,%zu>: [\n",name,mat->rows,mat->cols);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            printf("  %f,\t",matrix_get_element(mat,i,j));
        }
        printf("\n");
    }
    printf("]\n");

}

void matrix_vector_multiply (vector_t* res, matrix_t *mat, vector_t* vec) {
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

void vector_add (vector_t* res, vector_t* veca, vector_t* vecb) {

    if (veca->length != vecb->length) {
        fprintf(stderr,"vector length mismatch (%zu!=%zu)",veca->length,vecb->length);
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<veca->length; i++) {
        res->elements[i] = veca->elements[i] + vecb->elements[i];
    }

}

void sim_step (vector_t* next_state, matrix_t* update, vector_t* state, vector_t* force) {
    matrix_vector_multiply(next_state,update,state);
    vector_add(state,next_state,force);
    //vector_print(state,"State");
}

void matrix_place_vector (matrix_t* mat, vector_t* vec, int index) {
    
    if (mat->cols != vec->length) {
        fprintf(stderr,"mat cols != vector len! (%zu!=%zu)", mat->rows, vec->length);
        exit(EXIT_FAILURE);
    };

    for (int j=0; j < vec->length; j++) {
        mat->elements[index*mat->cols + j] = vec->elements[j];
    }
    
}

int main(void) {
    vector_t state;
    matrix_t update_matrix;
    vector_t force_vector;
    float dt=1e-3;
    float g=9.81;
    
    //float stop_time = 10; //seconds
    int num_steps = 500; 

    matrix_t result_matrix;
    float result_matrix_elements[num_steps*4];
    result_matrix.cols = 4;
    result_matrix.rows = num_steps;
    result_matrix.elements = result_matrix_elements;

    printf("Number of steps: %i\n",num_steps);
    if (num_steps > 1000) {
        printf("Warning: simulating more than 1000 steps.\n");
    }
        
    float state_elements[4] = {
        0.,
        0.,
        1.,
        1.
    };

    float update_matrix_elements[4*4] = {
        1,  0,  dt,  0 ,
        0,  1,  0 ,  dt,
        0,  0,  1 ,  0 ,
        0,  0,  0 ,  1 ,
    };

    float force_vector_elements[4] = {
        0,
        0,
        0,
        -g*dt
    };


    // Initial State
    state.length = 4;
    state.elements = state_elements;
    update_matrix.rows = 4;
    update_matrix.cols = 4;
    update_matrix.elements = update_matrix_elements;
    force_vector.length = 4;
    force_vector.elements = force_vector_elements;

    matrix_print(&update_matrix, "Update Matrix");
    vector_print(&force_vector, "Force Vector");
    vector_print(&state,"Initial State");

    vector_t next_state;
    float next_state_elements[4];
    next_state.elements = next_state_elements;
    next_state.length = 4;

    for (int step=0;step<num_steps;step++) {
        sim_step(&next_state,&update_matrix,&state,&force_vector);
        matrix_place_vector(&result_matrix,&state,step);
    }

    matrix_print(&result_matrix,"Results");

};
