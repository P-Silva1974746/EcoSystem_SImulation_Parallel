#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void print_ecosystem(char **M, int rows, int cols){
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        printf("|");
        for (int  j = 0; j < cols; j++)
        {   
            if (j==cols-1)printf("%c", M[i][j]);
            else printf("%c", M[i][j]);  
        }
        printf("|\n"); 
    }
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
}

char **read_input(int *gen_proc_rabbits, int *gen_proc_foxes, int *gen_food_rabbits, int *n_gen, int *rows, int *cols, int  *n){
    int * temp[]= {gen_proc_rabbits, gen_proc_foxes, gen_food_rabbits, n_gen, rows, cols, n};

    for (int i = 0; i < sizeof(temp)/sizeof(temp[0]); i++)
    {
        if (scanf("%d", temp[i])!=1)
        {
            printf("Invalid input: argument number %d was not read properly\n", i+1);
            exit(EXIT_FAILURE);
        }
    }

    char **M = malloc(*rows * sizeof(char *));
    for (int i = 0; i < *rows; i++) {
        M[i] = malloc(*cols * sizeof(char));
        memset(M[i], ' ', *cols);  // initialize
    }

    for (int i = 0; i < *n; i++)
    {
        char str[7];
        if (scanf("%7s", str)!=1)
        {
            printf("Invalid input: argument number %d was not read properly", i+1);
            exit(EXIT_FAILURE);
        }
        int r,c;
        scanf("%d %d", &r, &c);

        if(strcmp(str, "FOX") == 0) M[r][c]='F';
        else if (strcmp(str, "RABBIT") == 0) M[r][c]='R';
        else if(strcmp(str, "ROCK") == 0) M[r][c]='*';
    }
    
    print_ecosystem(M, *rows, *cols);
    return M;
}

// Uncomment if want to debug something in this file
// int main (int agrc, char *argv[]){
//     int gen_proc_rabbits, gen_proc_foxes, gen_food_rabbits, n_gen, rows, cols, n;
//     char **M = read_input(&gen_proc_rabbits, &gen_proc_foxes, &gen_food_rabbits, &n_gen, &rows, &cols, &n);

    
// }