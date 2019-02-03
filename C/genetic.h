#include "params.h"

typedef struct Job {
    int start, end, job_id;
} Job;

typedef struct Machine {
    Job jobs[NUM_JOBS + 1];
} Machine;

typedef struct Solution {
    Machine machines[NUM_MACHINES];
} Solution;

typedef struct Chromo {
    int arr[CHROMO_SIZE];
    double fitness;
} Chromo;

typedef Chromo** Population;

void initialize();

Chromo* generate_chromo();
void print_chromo(Chromo* chromo);
void print_population(Population population);
void print_solution(Solution* sol);

Solution* chromo_to_solution(Chromo* chromo);
Solution* create_dummy_solution();

double fitness_func(Chromo* chromo);
double fitness_func_with_weights(Chromo* chromo);
Population generate_initial_population();
void mutation(Chromo* chromo);
Chromo* crossover(Chromo* chromo1, Chromo* chromo2);
Chromo* find_fittest(Population population);
void sort_population(Population population);
Chromo* generate_ind_crossover(Population population, double* prob_array, int population_size);
void generate_offspring(Population population);

Chromo* genetic_algo();