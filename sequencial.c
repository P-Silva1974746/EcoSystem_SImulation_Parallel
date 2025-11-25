#include <stdio.h>
#include <stdlib.h>
#include "aux_func.h"
#include <string.h>

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

        if (strcmp(type, "FOX") == 0) world[r][c].type = FOX;
        else if (strcmp(type, "RABBIT") == 0) world[r][c].type = RABBIT;
        else if (strcmp(type, "ROCK") == 0) world[r][c].type = ROCK;
        else {
            printf("Erro: entidade desconhecida '%s'.\n", type);
            exit(EXIT_FAILURE);
        }
    }

    //print da matriz inicial
    //print_ecosystem(world, ROWS, COLS);
    //printf("\n");

    for(int g=0; g<N_GEN; g++){
        Cell **next = next_gen(world, ROWS,COLS, g, GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES);
        free_matrix(world);
        world = next;

        //print da matriz a cada geraçao
        printf("Generation %d\n", g+1);
        print_ecosystem(world, ROWS, COLS);
        printf("\n");
    }

    //imprimir o output pedido

    int total_obj_count=0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (world[i][j].type!=EMPTY) total_obj_count++;    
        }  
    }
    
    // printf("%d %d %d %d %d %d %d\n", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, 0, ROWS, COLS, total_obj_count);
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLS; j++)
    //     {
    //         switch(world[i][j].type){
    //             case RABBIT:
    //                 printf("RABBIT %d %d\n", i, j); 
    //                 break;
    //             case FOX:
    //                 printf("FOX %d %d\n", i, j); 
    //                 break;
    //             case ROCK:
    //                 printf("ROCK %d %d\n", i, j); 
    //                 break;
    //         }
    //     }
    // }
    

    free_matrix(world);
    return 0;

}