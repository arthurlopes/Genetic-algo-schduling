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

// typedef int* Chromo;
typedef Chromo** Population;

void initialize();

Chromo* generate_chromo(int matrix[NUM_JOBS][NUM_MACHINES]);
void print_chromo(Chromo* chromo);
void print_population(Population population);

Solution* chromo_to_solution(Chromo* chromo, int matrix[NUM_JOBS][NUM_MACHINES]);
Solution* create_dummy_solution();
void print_solution(Solution* sol);
double fitness_func(Chromo* chromo, int matrix[NUM_JOBS][NUM_MACHINES]);
Population generate_initial_population(int matrix[NUM_JOBS][NUM_MACHINES]);
void mutation(Chromo* chromo);
Chromo* crossover(Chromo* chromo1, Chromo* chromo2, int matrix[NUM_JOBS][NUM_MACHINES]);
Chromo* find_fittest(Population population, int matrix[NUM_JOBS][NUM_MACHINES]);
void sort_population(Population population);
Chromo* generate_ind_crossover(Population population, int* prob_array, int population_size, int matrix[NUM_JOBS][NUM_MACHINES]);
void generate_offspring(Population population, int matrix[NUM_JOBS][NUM_MACHINES]);
Chromo* genetic_algo(int matrix[NUM_JOBS][NUM_MACHINES]);