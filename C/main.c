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

    Chromo chromo1 = generate_chromo();
    print_chromo(chromo1);
    Chromo chromo2 = generate_chromo();
    print_chromo(chromo2);

    Solution* sol = chromo_to_solution(chromo1, input_matrix);
    print_solution(sol);

    Population* pop = generate_initial_population();
    print_chromo(*pop[22]);
    print_chromo(*pop[26]);
    print_chromo(*pop[21]);

    printf("Mutation\n");
    print_chromo(chromo1);
    mutation(chromo1);
    print_chromo(chromo1);

    printf("Crossover\n");
    print_chromo(chromo1);
    print_chromo(chromo2);
    Chromo chromo3 = crossover(chromo1, chromo2);
    print_chromo(chromo3);

    return 0;
}