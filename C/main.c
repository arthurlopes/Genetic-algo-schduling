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

    // Chromo* chromo1 = generate_chromo(input_matrix);
    // print_chromo(chromo1);
    // Chromo* chromo2 = generate_chromo(input_matrix);
    // print_chromo(chromo2);

    // Solution* sol = chromo_to_solution(chromo1, input_matrix);
    // print_solution(sol);

    // printf("Population\n");
    // Population pop = generate_initial_population(input_matrix);
    // print_chromo(pop[0]);
    // // print_chromo(pop[50]);
    // // print_chromo(pop[99]);

    // printf("Mutation\n");
    // print_chromo(chromo1);
    // mutation(chromo1);
    // print_chromo(chromo1);

    // printf("Crossover\n");
    // print_chromo(chromo1);
    // print_chromo(chromo2);
    // Chromo* chromo3 = crossover(chromo1, chromo2, input_matrix);
    // print_chromo(chromo3);

    // printf("Fittest\n");
    // print_chromo(find_fittest(pop, input_matrix));

    // // printf("Population\n");
    // // print_population(pop);
    // printf("Sorted Population\n");
    // sort_population(pop);
    // print_population(pop);

    // printf("Offspring\n");
    // generate_offspring(pop, input_matrix);
    // print_population(pop);


    Chromo* best_solution;
    best_solution = genetic_algo(input_matrix);
    print_chromo(best_solution);
    print_solution(chromo_to_solution(best_solution, input_matrix));

    return 0;
}