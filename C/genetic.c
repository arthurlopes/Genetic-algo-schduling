#include "stdlib.h"
#include "time.h"
#include <math.h>

#include "genetic.h"
#include "params.h"

void initialize() {
    srand(time(NULL));
}

Chromo generate_chromo() {
    Chromo chromo;
    chromo = malloc(sizeof(int) * CHROMO_SIZE);

    for (int i = 0; i < CHROMO_SIZE; i++) 
        chromo[i] = i;

    for (int i = 0; i < CHROMO_SIZE; i++) {
        int j, t;
        j = rand() % (CHROMO_SIZE-i) + i;
        t = chromo[j]; chromo[j] = chromo[i]; chromo[i] = t; // Swap i and j
    }

    return chromo;
}

void print_chromo(int* chromo) {
    for (int i = 0; i < CHROMO_SIZE; i++) {
        printf("%d ", chromo[i]);
    }
    printf("\n");
}

Solution* chromo_to_solution(Chromo chromo, int matrix[NUM_JOBS][NUM_MACHINES]) {
    Solution* sol;
    sol = create_dummy_solution();

    int last_used[NUM_MACHINES];

    for (int i = 0; i < NUM_MACHINES; i++) {
        last_used[i] = 0;
    }

    for (int i = 0; i < CHROMO_SIZE; i++) {

        int index = chromo[i];

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
                            if ((((start < start_p) && (start_p < end)) || ((start < end_p) && (end_p < end))) || 
                                (((start_p < start) && (start < start_p)) && ((start_p < end) && (end < start_p)))) {
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

double fitness_func(Chromo chromo, int matrix[NUM_JOBS][NUM_MACHINES]) {
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

    return 1/max;
}

Population* generate_initial_population() {
    Population* pop;
    pop = malloc(sizeof(Population));

    for (int i = 0; i < POPULATION_SIZE; i++) {
        *pop[i] = generate_chromo();
    }

    return pop;
}

void mutation(Chromo chromo) {
    int index1 = rand() % CHROMO_SIZE;
    int index2 = rand() % CHROMO_SIZE;

    int aux        = chromo[index1];
    chromo[index1] = chromo[index2];
    chromo[index2] = aux;
}

Chromo crossover(Chromo chromo1, Chromo chromo2) {

}
