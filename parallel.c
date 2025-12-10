#include <stdio.h>
#include <stdlib.h>
#include "aux_func_parallel.h"
#include <string.h>
#include <omp.h>

int main (int agrc, char *argv[]){
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, ROWS, COLS, N;
    int NUM_WORKERS = 1;
    //recebe o input das variaveis
    if (scanf("%d %d %d %d %d %d %d",
            &GEN_PROC_RABBITS,
            &GEN_PROC_FOXES,
            &GEN_FOOD_FOXES,
            &N_GEN,
            &ROWS,
            &COLS,
            &N) != 7)
        {
            printf("Erro: input inválido.\n");
            exit(EXIT_FAILURE);
        }
    
    Cell **world = allocate_matrix(ROWS, COLS);

    //inicializa os tipos de cada cell
    for (int i = 0; i < N; i++)
    {
        char type[7];
        if (scanf("%7s", type)!=1)
        {
            printf("Invalid input: argument number %d was not read properly", i+1);
            exit(EXIT_FAILURE);
        }
        int r,c;
        scanf("%d %d", &r, &c);

        if (strcmp(type, "ROCK") == 0) world[r][c].type = ROCK;
        else if (strcmp(type, "RABBIT") == 0) world[r][c].type = RABBIT;
            else if (strcmp(type, "FOX") == 0) world[r][c].type = FOX;
            else {
                printf("Erro: entidade desconhecida '%s'.\n", type);
                exit(EXIT_FAILURE);
            }
    }


    // cria a matriz de lockes
    omp_lock_t **locks = malloc(ROWS * sizeof *locks);
    for (int i = 0; i < ROWS; i++) {
        locks[i] = malloc(COLS * sizeof *locks[i]);

        for (int j = 0; j < COLS; j++)omp_init_lock(&locks[i][j]);
    }
    omp_lock_t counter_lock;
    omp_init_lock(&counter_lock);

    double i_time = omp_get_wtime();
    double total_time_rabbits = 0;
    double total_time_foxes = 0;

    omp_set_num_threads(NUM_WORKERS);
    for(int g=0; g<N_GEN; g++){
        Cell **next = next_gen(world, ROWS,COLS, g, GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, &N, locks, &counter_lock, &total_time_foxes, &total_time_rabbits);
        free_matrix(world);
        world = next;    
    }

    double f_time = omp_get_wtime();
    printf("Time for output: %f\n", f_time-i_time);
    printf("Average time for move_foxes: %f\n", total_time_foxes/N_GEN);
    printf("Average time for move_rabbits: %f\n", total_time_rabbits/N_GEN);
    printf("Maximum number of threads %d\n", omp_get_max_threads());



    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) omp_destroy_lock(&locks[i][j]);
        free(locks[i]);
    }
    free(locks);
    omp_destroy_lock(&counter_lock);

    //imprimir o output pedido
    printf("%d %d %d %d %d %d %d\n", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, 0, ROWS, COLS, N);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            switch(world[i][j].type){
                case RABBIT:
                    printf("RABBIT %d %d\n", i, j); 
                    break;
                case FOX:
                    printf("FOX %d %d\n", i, j); 
                    break;
                case ROCK:
                    printf("ROCK %d %d\n", i, j); 
                    break;
            }
        }
    }
    

    free_matrix(world);
    return 0;

}