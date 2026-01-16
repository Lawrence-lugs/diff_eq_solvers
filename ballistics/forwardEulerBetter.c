#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "tensors.h"

void get_slope(const Vector* slope,Vector* state,const Vector* force, const float drag_per_mass) {

    float vx = state->elements[2];
    float vy = state->elements[3];

    float speed = sqrtf(vx*vx + vy*vy);

    slope->elements[0] = vx;
    slope->elements[1] = vy;
    slope->elements[2] = vx*(-speed*drag_per_mass) + force->elements[2];
    slope->elements[3] = vy*(-speed*drag_per_mass) + force->elements[3];
}

void sim_step(Vector* state,const Vector* force, const float dt, const float drag_per_mass) {

    Vector slope = {
        .elements = (float[4]){},
        .length = 4
    };

    get_slope(&slope, state, force, drag_per_mass);
    vector_scalar_multiply(dt, &slope);
    vector_add(state, state, &slope);
}

int main(void) {
    float dt=50e-3;
    float g=9.81;
    float drag_per_mass=0.02;
 
    #define NUM_STEPS 50

    Matrix result_matrix = {
        .elements = (float[NUM_STEPS * 4]){0},
        .cols = 4,
        .rows = NUM_STEPS,
    };
        
    Vector state = {
        .elements = (float[4]){0,0,10,10},
        .length = 4,
    };

    Vector force_vector = {
        .elements = (float[4]){0,0,0,-g},
        .length = 4,
    };

    vector_print(&force_vector, "Force Vector");
    vector_print(&state,"Initial State");

    for (size_t step=0;step<NUM_STEPS;step++) {
        sim_step(&state,&force_vector,dt,drag_per_mass);
        matrix_place_vector(&result_matrix,&state,step);
    }

    printf("\tx,\ty,\tvx,\tvy,\n");
    matrix_print(&result_matrix,"Results");
    matrix_write_bin("forwardEulerBetter.bin",&result_matrix);
};
