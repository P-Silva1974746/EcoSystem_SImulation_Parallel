#include <stdio.h>
#include <stdlib.h>
#include "aux_func.h"
#include <string.h>
#include <omp.h>

Cell **allocate_matrix(int rows, int cols) {
    Cell **M = malloc(rows * sizeof(Cell *));
    Cell *data = malloc(rows * cols * sizeof(Cell));
    for (int i = 0; i < rows; i++) {
        M[i] = &data[i * cols];
    }

    for (int i = 0; i < rows * cols; i++) {
        data[i].type   = EMPTY;
        data[i].age    = 0;
        data[i].hunger = 0;
    }
    
    return M;
}

char symbol (CellType type) {
     switch(type){
        case EMPTY: return ' ';
        case RABBIT: return 'R';
        case FOX: return 'F';
        case ROCK: return '*';
    }
}

void print_ecosystem(Cell **M, int rows, int cols){
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        printf("|");
        for (int  j = 0; j < cols; j++)
        {   
            printf("%c", symbol((M[i][j]).type));
        }
        printf("|\n"); 
    }
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
}

void print_age(Cell **M, int rows, int cols){
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        printf("|");
        for (int  j = 0; j < cols; j++)
        {   
            if ((M[i][j]).type == EMPTY) printf(" ");
            else if ((M[i][j]).type == ROCK) printf("*");
            else printf("%d", (M[i][j]).age);
        }
        printf("|\n"); 
    }
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
}

void print_hunger(Cell **M, int rows, int cols){
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
    for (int i = 0; i < rows; i++)
    {
        printf("|");
        for (int  j = 0; j < cols; j++)
        {   
            if ((M[i][j]).type == EMPTY) printf(" ");
            else if ((M[i][j]).type == ROCK) printf("*");
            else if ((M[i][j]).type == RABBIT) printf("R");
            else printf("%d", (M[i][j]).hunger);
        }
        printf("|\n"); 
    }
    for (int  i = 0; i < cols+2; i++)printf("-");
    printf("\n");
}

void print_all(Cell **M, int R, int C) {
    // primeira linha de bordas
    for (int j = 0; j < C + 2; j++) putchar('-');
    printf("   "); // espaço entre ecosystem e age
    for (int j = 0; j < C + 2; j++) putchar('-');
    printf(" "); // espaço entre age e hunger
    for (int j = 0; j < C + 2; j++) putchar('-');
    putchar('\n');

    for (int i = 0; i < R; i++) {
        // ----- ecosystem -----
        putchar('|');
        for (int j = 0; j < C; j++) {
            putchar(symbol(M[i][j].type));
        }
        putchar('|');

        // 3 espaços
        printf("   ");

        // ----- age -----
        putchar('|');
        for (int j = 0; j < C; j++) {
            if (M[i][j].type == EMPTY) putchar(' ');
            else if (M[i][j].type == ROCK) putchar('*');
            else printf("%d", M[i][j].age);
        }
        putchar('|');

        // 1 espaço
        putchar(' ');

        // ----- hunger -----
        putchar('|');
        for (int j = 0; j < C; j++) {
            if (M[i][j].type == EMPTY) putchar(' ');
            else if (M[i][j].type == ROCK) putchar('*');
            else if (M[i][j].type == RABBIT) putchar('R');
            else printf("%d", M[i][j].hunger);
        }
        putchar('|');

        putchar('\n');
    }

    // última linha de bordas
    for (int j = 0; j < C + 2; j++) putchar('-');
    printf("   ");
    for (int j = 0; j < C + 2; j++) putchar('-');
    printf(" ");
    for (int j = 0; j < C + 2; j++) putchar('-');
    putchar('\n');
}

void free_matrix(Cell **M) {
    if (!M) return;
    free(M[0]);
    free(M);
}

Cell **copy_matrix(Cell **src, int rows, int cols) {
    Cell **dst = allocate_matrix(rows, cols);

    // this copies an continous memory block it works as an copy since the alloca_matrix puts everything in the one contious block (data)
    memcpy(dst[0], src[0], rows * cols * sizeof(Cell));

    return dst;
}


//usar na funçao choose_adjacent para garantir casas validas
static int is_valid(int x, int y, int rows, int cols){
    return x>=0 && x<rows && y>=0 && y<cols;
}

//coloca as coordenadas da célula escolhida seguindo a regra (G+X+Y) mod P em *n_row e *n_col
static void choose_adjacent(CellType target, Cell **world, int r, int c, int *n_row, int *n_col, int rows, int cols, int gen){
    int directions [4][2] = {-1,0, 0,1, 1,0, 0,-1};
    int valid_dirs [4][2];

    int valid_count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (is_valid(r+directions[i][0], c+directions[i][1], rows, cols) && world[r+directions[i][0]][c+directions[i][1]].type==target){
            valid_dirs[valid_count][0] = r+directions[i][0];
            valid_dirs[valid_count][1] = c+directions[i][1];
            valid_count++;
            
        }
    }

    if (valid_count>0){
        *n_row = valid_dirs[(gen+r+c)%valid_count][0];
        *n_col = valid_dirs[(gen+r+c)%valid_count][1];
    }else{
        *n_row = r;
        *n_col = c;
    } 
}


static void move_rabbits(Cell **world, Cell **tmp_world, Cell **new_world, int rows, int cols, int gen, int GEN_PROC_RABBITS, int *objs, double *time){
    double i_time = omp_get_wtime();
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(world[i][j].type==ROCK) tmp_world[i][j] = new_world[i][j] = world[i][j];
            else if (world[i][j].type==FOX) tmp_world[i][j] = world[i][j];         
            else if(world[i][j].type == RABBIT){
                //printf("ITERATION %d %d\n", i,j);
                int new_x, new_y;
                choose_adjacent(EMPTY, world, i, j, &new_x, &new_y, rows, cols, gen);
                //printf("CHOOSEN ADJACENT %d %d\n", new_x, new_y);
                int age;
                if(world[i][j].age >= GEN_PROC_RABBITS && !(new_x==i && new_y==j)) age = 0;
                else age = world[i][j].age;

                //procriação
                if(world[i][j].age >= GEN_PROC_RABBITS && !(new_x==i && new_y==j)){
                    //printf("Procreated\n");
                    (*objs)++;
                    tmp_world[i][j].type = new_world[i][j].type = RABBIT;
                    tmp_world[i][j].age = new_world[i][j].age = 0;
                    tmp_world[i][j].hunger = new_world[i][j].hunger = 0;
                }

                //conflito: mantem o coelho mais velho
                if(tmp_world[new_x][new_y].type==RABBIT){
                    (*objs)--;
                    if(age <= (tmp_world[new_x][new_y].age-1)) continue; // in this case where it is younger than the one already in that position we  
                                                                                // skip this iteration and it dies since it world[i][j] is not copied 
                                                                                // to a position in new_world
                                                                                // int the case where it is older we continue the iteration and overwrite
                                                                                // the values in new_world[new_x][new_y] making so that the that was there
                                                                                // dies
                }
                
                //move o coelho
                tmp_world[new_x][new_y] = new_world[new_x][new_y] = world[i][j];
                tmp_world[new_x][new_y].age++;
                new_world[new_x][new_y].age++;
                //printf("Age: %d\n", world[i][j].age);

                if(world[i][j].age >= GEN_PROC_RABBITS && !(new_x==i && new_y==j)) tmp_world[new_x][new_y].age = new_world[new_x][new_y].age = 0;

 
            }
        }
    }
    double f_time = omp_get_wtime();
    *time+=(f_time-i_time);
}

static void move_foxes(Cell **world, Cell **new_world, int rows, int cols, int gen, int GEN_PROC_FOXES, int GEN_FOOD_FOXES, int *objs, double *time){
    //printf("-----------CALCULATING GEN %d----------- \n", gen+1);
    double i_time = omp_get_wtime();
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(world[i][j].type == FOX){
                //printf("ITERATION %d %d\n", i,j);
                Cell fox = world[i][j];
                fox.age++;
                fox.hunger++;

                //printf("Age:%d Hunger:%d\n",fox.age, fox.hunger);

                int age;
                int new_x, new_y;
                // chose one adjacent cell that has a RABBIT in the world, this is because all foxes move at the same time as such their percetion has 
                // to be of the world before any fox moves conflicts will then be resolved
                //printf("Before calculating adjacent col with a RABBIT in world\n");
                choose_adjacent(RABBIT, world, i, j, &new_x, &new_y, rows, cols, gen);
                //printf("After calculating adjacent col with a RABBIT in world\n");
                //printf("CHOOSEN ADJACENT %d %d\n", new_x, new_y);

                //morre de fome
                if(fox.hunger >= GEN_FOOD_FOXES && (new_x==i && new_y==j)) {
                    (*objs)--;
                    continue;
                }

                // if there is no adjacent cell of the type target choose_adjacent will just put the current coordenates in *new_x and *new_y
                // so if they aren't the same it means there was one RABBIT adjacent  
                if ( !(new_x==i && new_y==j)){

                    //TODO: perguntar ao professor o que acontece quando existir um conflito entre FOX's numa posicao onde tinha um RABBIT,
                    //      a fome pode ser ignorada uma vez que ela seria resetada de qualquer das formas dps da FOX comer o RABBIT independentemente
                    //      de qual for, assim sendo a unica coisa importante e a idade

                    if(world[i][j].age >= GEN_PROC_FOXES) age = 0;
                    else age = world[i][j].age;

                    //procriação
                    if(world[i][j].age >= GEN_PROC_FOXES){
                        (*objs)++;
                        new_world[i][j].type = FOX;
                        new_world[i][j].age = 0;
                        new_world[i][j].hunger = 0;
                    }

                    //conflito: mantém raposa mais velha ou com menos fome
                    if(new_world[new_x][new_y].type==FOX){
                        (*objs)--;
                        // the current fox moving is younger than the one already in position so we skip the iteration and it dies
                        if (age < new_world[new_x][new_y].age-1) continue;
                        else if(world[i][j].hunger > new_world[new_x][new_y].hunger-1) continue;
                    }

                    //come o coelho
                    fox.hunger = 0;
                    (*objs)--;
                    if(world[i][j].age >= GEN_PROC_FOXES) fox.age = 0;
                    
                    // move of the oldest fox
                    new_world[new_x][new_y] = fox;
 
                } else {
                    //printf("Before calculating adjacent col with a EMPTY in world\n");
                    choose_adjacent(EMPTY,world, i, j, &new_x, &new_y, rows, cols, gen);
                    //printf("After calculating adjacent col with a EMPTY in world\n");

                    //printf("CHOOSEN ADJACENT %d %d\n", new_x, new_y);

                    if( !(new_x==i && new_y==j)){

                        if(world[i][j].age >= GEN_PROC_FOXES) age = 0;
                        else age = world[i][j].age;

                        //procriação
                        if(world[i][j].age >= GEN_PROC_FOXES){
                            (*objs)++;
                            new_world[i][j].type = FOX;
                            new_world[i][j].age = 0;
                            new_world[i][j].hunger = 0;
                        }

                        //conflito: mantém raposa mais velha ou com menos fome
                        if(new_world[new_x][new_y].type==FOX){
                            (*objs)--;
                            if (age < new_world[new_x][new_y].age-1) continue;
                            else if (world[i][j].hunger > new_world[new_x][new_y].hunger-1) continue;
                        }

                        if(world[i][j].age >= GEN_PROC_FOXES) fox.age = 0;
                        new_world[new_x][new_y] = fox;

                    } else {
                        //printf("There is nowhere to go \n");
                        new_world[i][j] = fox; //fica no lugar
                    }
                }
            }
        }
    }
    double f_time = omp_get_wtime();
    *time+=(f_time-i_time);
}

//funçao principal - calcula a matriz inteira da proxima geraçao
Cell **next_gen(Cell **world, int rows, int cols, int gen, int GEN_PROC_RABBITS, int GEN_PROC_FOXES, int GEN_FOOD_FOXES, int *objs, double *time_foxes, double *time_rabbits){
    Cell **world_after_rabbits_move = allocate_matrix(rows, cols);
    Cell **new_world = allocate_matrix(rows, cols);
    //printf("Before moving rabbits\n");
    //mover coelhos primeiro (aqui tambem copia todas as rochas)
    move_rabbits(world, world_after_rabbits_move, new_world, rows, cols, gen, GEN_PROC_RABBITS, objs, time_rabbits);
    //printf("After moving rabbits\n");
    
    //printf("After rabbits move\n");
    //print_ecosystem(world_after_rabbits_move, rows,cols);
    //printf("\n");
    
    //mover raposas depois
    //printf("Moving foxes\n");
    move_foxes(world_after_rabbits_move, new_world, rows, cols, gen, GEN_PROC_FOXES, GEN_FOOD_FOXES, objs, time_foxes);
    //printf("After moving foxes\n");

    free_matrix(world_after_rabbits_move);
    return new_world;

    
}