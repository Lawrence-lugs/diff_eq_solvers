#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "tensors.h"

#define NUM_STEPS 50
#define NUM_GRID_POINTS 50

#define DT (5e-3) // 5 ms
#define DX (0.01) // 1 cm 100 to 1cm

#define DIFFN (35) // cm^2/s
#define TAU (1e-3) // s 

typedef struct State {
    float n;
    float dn;
} State;

State get_slope(const State point, const State leftPoint, const State rightPoint) {

    State slope;
    float diffusion    = -DIFFN*(2*point.n-leftPoint.n-rightPoint.n)/(DX*DX);
    float rg           = - point.n/TAU;
    slope.n = point.dn;
    slope.dn = rg; 
    return slope;

}

State get_point_from_grid (const Tensor* grid, size_t point) {
    State u;

    Tensor* slice = tensor_slice(grid, point);
    
    u.n  = slice->elements[0];
    u.dn = slice->elements[1];

    tensor_destroy(slice);

    return u;
}

void insert_state_on_grid (Tensor* grid, size_t point, const State u) {

    Tensor* slice = tensor_slice(grid, point);
    
    slice->elements[0] = u.n;
    slice->elements[1] = u.dn;

    tensor_copyto_index(grid, slice, point);
}

void step_point(Tensor* grid,Tensor* grid_old, size_t point) {

    // Boundary points are pinned- we do not update the state.
    if (point == 0 || point == NUM_GRID_POINTS-1) {
        return;
    }

    State currPointState = get_point_from_grid(grid_old, point);
    State prevPointState = get_point_from_grid(grid_old, point-1);
    State nextPointState = get_point_from_grid(grid_old, point+1);

    State slope = get_slope(currPointState, prevPointState, nextPointState);
    
    char name[32];
    char slope_name[32];
    sprintf(name, "Point %zu", point);
    sprintf(slope_name, "Slope %zu", point);

    printf("State %zu : (%f,%f)\n",point,currPointState.n,currPointState.dn);
    printf("Slope %zu : (%f,%f)\n",point,slope.n,slope.dn);

    currPointState.n = currPointState.n + DT*slope.n;
    currPointState.dn = currPointState.dn + DT*slope.dn;

    insert_state_on_grid(grid, point, currPointState);
}

void sim_step(Tensor* grid) {

    Tensor* grid_old = tensor_copy(grid);

    // Step all points in the grid
    for (int point = 0; point < NUM_GRID_POINTS; point++) {
        step_point(grid,grid_old,point);
    };

}

int main(void) {
    float n_0 = 10e4; //Minority carrier injection pin

    size_t tensorShape[3] = {2, NUM_GRID_POINTS, NUM_STEPS};
    Tensor* results = tensor_create(3, tensorShape);

    size_t gridShape[2] = {2, NUM_GRID_POINTS};
    Tensor* grid = tensor_create(2,gridShape);
    tensor_set_zero(grid);
    
    State initialState = {
        .n = n_0,
        .dn = 0
    };

    for (size_t point=0;point<NUM_GRID_POINTS-1;point++){
        insert_state_on_grid(grid, point, initialState);
    }

    // Iterate steps for all grid points
    for (size_t step=0;step<NUM_STEPS-1;step++) {
        tensor_copyto_index(results, grid, step);
        sim_step(grid);
    }
    tensor_copyto_index(results, grid, NUM_STEPS-1);
    tensor_write_bin("mcd.bin",results);

    tensor_destroy(results);
    tensor_destroy(grid);

};
