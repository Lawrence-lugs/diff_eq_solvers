#include <stdio.h>
#include <stdint.h>
#include "tensors.h"

void sim_step (Vector* next_state, Matrix* update, Vector* state, Vector* force) {
    matrix_vector_multiply(next_state,update,state);
    vector_add(state,next_state,force);
    //vector_print(state,"State");
}

int main(void) {
    float dt=5e-3;
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
        10.,
        10.
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
    tensor_write_bin("forwardEuler.bin",&result_matrix);

};
