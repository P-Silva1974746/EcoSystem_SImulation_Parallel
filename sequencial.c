#include <stdio.h>
#include <stdlib.h>
#include "aux_func.h"
#include <string.h>
#include <omp.h>

int main (int agrc, char *argv[]){
    int GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, N_GEN, ROWS, COLS, N;
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

    //print da matriz inicial
    //printf("Generation %d\n", 0);
    //print_ecosystem(world, ROWS, COLS);
    //print_age(world, ROWS, COLS);
    //print_hunger(world, ROWS, COLS);
    //print_all(world, ROWS, COLS);
    //printf("\n");

    double i_time = omp_get_wtime();
    double total_time_rabbits = 0;
    double total_time_foxes = 0;

    for(int g=0; g<N_GEN; g++){
        Cell **next = next_gen(world, ROWS,COLS, g, GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, &N, &total_time_foxes, &total_time_rabbits);
        free_matrix(world);
        world = next;

        //print da matriz a cada geraçao
        //printf("Generation %d\n", g+1);
        //print_ecosystem(world, ROWS, COLS);
        // print_age(world, ROWS, COLS);
        //print_hunger(world, ROWS, COLS);
        //print_all(world, ROWS, COLS);
        //printf("\n");
        
        
    }

    double f_time = omp_get_wtime();
    printf("Time for output: %f\n", f_time-i_time);
    printf("Average time for move_foxes: %f\n", total_time_foxes/N_GEN);
    printf("Average time for move_rabbits: %f\n", total_time_rabbits/N_GEN);

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