#include "stdlib.h"
#include "time.h"
#include <math.h>

#include "genetic.h"
#include "params.h"

void initialize() {
    srand(time(NULL));
}

Chromo* generate_chromo(int matrix[NUM_JOBS][NUM_MACHINES]) {
    Chromo* chromo;
    chromo = (Chromo*)malloc(sizeof(Chromo));

    for (int i = 0; i < CHROMO_SIZE; i++) 
        chromo->arr[i] = i;

    for (int i = 0; i < CHROMO_SIZE; i++) {
        int j, t;
        j = rand() % (CHROMO_SIZE-i) + i;
        t = chromo->arr[j]; chromo->arr[j] = chromo->arr[i]; chromo->arr[i] = t; // Swap i and j
    }

    chromo->fitness = fitness_func(chromo, matrix);

    return chromo;
}

void print_chromo(Chromo* chromo) {
    for (int i = 0; i < CHROMO_SIZE; i++) {
        printf("%d ", chromo->arr[i]);
    }
    printf(" - %f\n", chromo->fitness);
}

Solution* chromo_to_solution(Chromo* chromo, int matrix[NUM_JOBS][NUM_MACHINES]) {
    Solution* sol;
    sol = create_dummy_solution();

    int last_used[NUM_MACHINES];

    for (int i = 0; i < NUM_MACHINES; i++) {
        last_used[i] = 0;
    }

    for (int i = 0; i < CHROMO_SIZE; i++) {

        int index = chromo->arr[i];

        int job     = floor(index / NUM_MACHINES);
        int machine = (index % NUM_JOBS);

        int start   = sol->machines[machine].jobs[last_used[machine]].end;
        int end     = start + matrix[job][machine];

        int flag = 1;

        while (flag) {
            flag = 0;
            for (int m = 0; m < NUM_MACHINES; m++) {
                if (m == machine) {
                    continue;
                } else {
                    for (int j = 0; j < NUM_JOBS + 1; j++) {
                        if (sol->machines[m].jobs[j].job_id == job) {
                            int start_p = sol->machines[m].jobs[j].start;
                            int end_p   = sol->machines[m].jobs[j].end;
                            if ((((start < start_p) && (start_p <= end)) || ((start < end_p) && (end_p <= end))) || 
                                (((start_p <= start) && (start < end_p)) && ((start_p < end) && (end <= end_p)))) {
                                start = sol->machines[m].jobs[j].end;
                                end   = start + matrix[job][machine];
                                flag  = 1;
                            }
                        }
                    }
                }
            }
        }

        last_used[machine]++;
        sol->machines[machine].jobs[last_used[machine]].start  = start;
        sol->machines[machine].jobs[last_used[machine]].end    = end;
        sol->machines[machine].jobs[last_used[machine]].job_id = job;
    }

    return sol;
}

Solution* create_dummy_solution() {
    Solution* sol;
    sol = malloc(sizeof(Solution));

    for (int i = 0; i < NUM_MACHINES; i++) {
        for (int j = 0; j < NUM_JOBS + 1; j++) {
            sol->machines[i].jobs[j].start  = 0;
            sol->machines[i].jobs[j].end    = 0;
            sol->machines[i].jobs[j].job_id = -1;
        }
    }
    
    return sol;
}

Chromo* create_dummy_chromo() {
    Chromo* chromo;
    chromo = malloc(sizeof(Chromo));

    for (int i = 0; i < CHROMO_SIZE; i++) {
        chromo->arr[i] = 0;
    }

    chromo->fitness = 0.0;
    
    return chromo;
}

void print_solution(Solution* sol) {
    
    printf("------------------------------------------------------------------------------\n");
    for (int i = 0; i < NUM_MACHINES; i++) {
        for (int j = 0; j < NUM_JOBS + 1; j++) {
            printf("(%d, %d, %d) ", sol->machines[i].jobs[j].start, sol->machines[i].jobs[j].end, sol->machines[i].jobs[j].job_id);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------------------------\n");

}

double fitness_func(Chromo* chromo, int matrix[NUM_JOBS][NUM_MACHINES]) {
    Solution* sol;
    int max = 0;

    if (chromo == NULL) {
        return 0.0;
    }

    sol = chromo_to_solution(chromo, matrix);

    for (int i = 0; i < NUM_MACHINES; i++) {
        if (max < sol->machines[i].jobs[NUM_JOBS].end) {
            max = sol->machines[i].jobs[NUM_JOBS].end;
        }
    }

    return 1.0/max;
}

Population generate_initial_population(int matrix[NUM_JOBS][NUM_MACHINES]) {
    Chromo** pop;
    pop = malloc(sizeof(Chromo*) * POPULATION_SIZE);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        pop[i] = generate_chromo(matrix);
    }

    return pop;
}

void mutation(Chromo* chromo) {
    int index1 = rand() % CHROMO_SIZE;
    int index2 = rand() % CHROMO_SIZE;

    int aux             = chromo->arr[index1];
    chromo->arr[index1] = chromo->arr[index2];
    chromo->arr[index2] = aux;
}

int chromo_contains(Chromo* chromo, int value) {
    for (int i = 0; i < CHROMO_SIZE; i++)
        if (chromo->arr[i] == value)
            return 1;
    
    return 0;
}

Chromo* crossover(Chromo* chromo1, Chromo* chromo2, int matrix[NUM_JOBS][NUM_MACHINES]) {
    Chromo* new_chromo;
    int *chromo2_ext;
    new_chromo  = malloc(sizeof(Chromo));
    chromo2_ext = malloc(sizeof(int) * 2 * CHROMO_SIZE);
    for (int i = 0; i < CHROMO_SIZE; i++)
        new_chromo->arr[i] = -1;
    
    for (int i = 0; i < CHROMO_SIZE * 2; i++)
        chromo2_ext[i] = chromo2->arr[i % (CHROMO_SIZE)];

    int index1 = rand() % CHROMO_SIZE;
    int index2 = (rand() % (CHROMO_SIZE - index1)) + index1;

    // printf("%d %d\n", index1, index2);
    
    for (int i = index1; i <= index2; i++) {
        new_chromo->arr[i] = chromo1->arr[i];
    }

    for (int i = index2; i < CHROMO_SIZE; i++) {
        int index_p = i;
        while (chromo_contains(new_chromo, chromo2_ext[index_p])) {
            index_p++;
            if (index_p == CHROMO_SIZE * 2) {
                break;
            }
        }
        new_chromo->arr[i] = chromo2_ext[index_p];
    }

    for (int i = 0; i < index1; i++) {
        int index_p = i;
        while (chromo_contains(new_chromo, chromo2_ext[index_p])) {
            index_p++;
            if (index_p == CHROMO_SIZE * 2) {
                break;
            }
        }
        new_chromo->arr[i] = chromo2_ext[index_p];
    }

    new_chromo->fitness = fitness_func(new_chromo, matrix);

    return new_chromo;
}

Chromo* find_fittest(Population population, int matrix[NUM_JOBS][NUM_MACHINES]) {
    Chromo* fittest = population[0];
    int fit = fitness_func(fittest, matrix);

    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (population[i]->fitness > fit) {
            fittest = population[i];
            fit = population[i]->fitness;
        }
    }

    return fittest;
}

int comparator(const void *p, const void *q) {
    Chromo* c1 = *(Chromo**)p;
    Chromo* c2 = *(Chromo**)q;

    if ((c1->fitness - c2->fitness) > 0) {
        return -1;
    } else if ((c1->fitness - c2->fitness) < 0) {
        return 1;
    } else {
        return 0;
    }
}

void sort_population(Population population) {
    qsort((void**)population, POPULATION_SIZE, sizeof(Chromo*), comparator);
}

void print_population(Population population) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        print_chromo(population[i]);
    }
}

Chromo* generate_ind_crossover(Population population, int* prob_array, int population_size, int matrix[NUM_JOBS][NUM_MACHINES]) {
    Chromo* new_chromo;
    new_chromo  = malloc(sizeof(Chromo));

    int parent1 = -1;
    while (parent1 == -1) {
        int index = rand() % population_size;
        if (rand()%100 < prob_array[index] * 100) {
            parent1 = index;
        }
    }

    int parent2 = -1;
    while (parent2 == -1) {
        int index = rand() % population_size;
        if (rand()%100 < prob_array[index] * 100) {
            parent2 = index;
        }
    }

    new_chromo = crossover(population[parent1], population[parent2], matrix);

    if (rand()%100 < MUTATION_PROB * 100) {
        mutation(new_chromo);
    }

    return new_chromo;
}

void get_prob(Population population, int* prob_array) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        prob_array[i] = (population[i]->fitness - population[POPULATION_SIZE-1]->fitness)/(population[0]->fitness-population[POPULATION_SIZE-1]->fitness);
    }
}

void generate_offspring(Population population, int matrix[NUM_JOBS][NUM_MACHINES]) {
    int* prob_array;
    prob_array = malloc(sizeof(int) * POPULATION_SIZE);

    sort_population(population);
    get_prob(population, prob_array);

    for (int i = POPULATION_SIZE*KILL_RATIO; i < POPULATION_SIZE; i++) {
        Chromo* new_ind = generate_ind_crossover(population, prob_array, i, matrix);
        population[i] = new_ind;
    }

}

Chromo* genetic_algo(int matrix[NUM_JOBS][NUM_MACHINES]) {
    int cur_iter = 1;
    Chromo* best_solution = create_dummy_chromo();
    Population population = generate_initial_population(matrix);

    while (cur_iter < MAX_ITER) {
        printf("Generation %d\n", cur_iter);

        Chromo* best_gen = find_fittest(population, matrix);
        if (best_gen->fitness > best_solution->fitness) {
            best_solution = best_gen;
        }

        generate_offspring(population, matrix);
        cur_iter++;
    }

    return best_solution;
}
