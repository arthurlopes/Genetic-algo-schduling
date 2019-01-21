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

typedef int* Chromo;
typedef Chromo Population[POPULATION_SIZE];

void initialize();

Chromo generate_chromo();
void print_chromo(int* chromo);

Solution* chromo_to_solution(Chromo chromo, int matrix[NUM_JOBS][NUM_MACHINES]);
Solution* create_dummy_solution();
void print_solution(Solution* sol);
Population* generate_initial_population();
void mutation(Chromo chromo);
Chromo crossover(Chromo chromo1, Chromo chromo2);