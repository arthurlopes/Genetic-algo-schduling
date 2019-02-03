#include "stdio.h"
#include "params.h"
#include "genetic.h"

// Optimal solution is 193
int input_matrix[NUM_JOBS][NUM_MACHINES] = 
    {{34,  2, 54, 61}, 
     {15, 89, 70,  9}, 
     {38, 19, 28, 87}, 
     {95,  7, 34, 29}};

int main() {

    initialize();

    Chromo* best_solution;
    best_solution = genetic_algo(input_matrix);
    print_chromo(best_solution);
    print_solution(chromo_to_solution(best_solution, input_matrix));
    printf("Cost: %f\n", 1/best_solution->fitness);

    return 0;
}