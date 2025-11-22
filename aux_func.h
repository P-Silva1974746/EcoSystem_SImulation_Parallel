#ifndef AUX_FUNC_H
#define AUX_FUNC_H

// Prints the ecosystem matrix M with a border.
// Arguments:
//   M    – matrix of chars (allocated dynamically)
//   rows – number of rows in the matrix
//   cols – number of columns in the matrix
void print_ecosystem(char **M, int rows, int cols);

// Reads all input parameters and initializes the ecosystem matrix.
// Arguments must be pointers so the function can write values into them:
//   gen_proc_rabbits – generations needed for rabbits to reproduce
//   gen_proc_foxes   – generations needed for foxes to reproduce
//   gen_food_rabbits – rabbits needed for a fox to eat
//   n_gen            – number of generations to simulate
//   rows, cols       – size of the ecosystem grid
//   n                – number of objects to place (FOX, RABBIT, ROCK)
//
// Return value:
//   A pointer to the dynamically allocated matrix M (char **)
char **read_input(
    int *gen_proc_rabbits,
    int *gen_proc_foxes,
    int *gen_food_rabbits,
    int *n_gen,
    int *rows,
    int *cols,
    int *n
);

#endif // AUX_FUNC_H
