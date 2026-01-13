#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "tensors.h"

void sim_step (Vector* state,const Vector* force, const float dt, const float drag_per_mass) {

    Vector state_slope;
    float state_slope_elements[4];
    state_slope.length = 4;
    state_slope.elements = state_slope_elements;

    float x = state->elements[0];
    float y = state->elements[1];
    float vx = state->elements[2];
    float vy = state->elements[3];

    float speed = sqrtf(vx*vx + vy*vy);
    
    state->elements[0] = x + vx*dt;
    state->elements[1] = y + vy*dt;
    state->elements[2] = dt*(vx*(1-speed*drag_per_mass) - force->elements[2]);
    state->elements[3] = dt*(vy*(1-speed*drag_per_mass) - force->elements[3]);

}

int main(void) {
    float dt=5e-3;
    float g=9.81;
    float drag_per_mass=0.001;
 
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

    vector_print(&force_vector, "Force Vector");
    vector_print(&state,"Initial State");

    for (size_t step=0;step<num_steps;step++) {
        sim_step(&state,&force_vector,dt,drag_per_mass);
        matrix_place_vector(&result_matrix,&state,step);
    }

    matrix_print(&result_matrix,"Results");
    tensor_write_bin("forwardEuler.bin",&result_matrix);

};
