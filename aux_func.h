#ifndef AUX_FUNC_H
#define AUX_FUNC_H

typedef enum {
    EMPTY,
    RABBIT,
    FOX,
    ROCK
} CellType;

typedef struct {
    CellType type;
    int age;
    int hunger;
} Cell;

Cell **allocate_matrix(int rows, int cols);
void free_matrix(Cell **M);
Cell **next_gen(Cell **world, int rows, int cols, int gen, int GEN_PROC_RABBITS, int GEN_PROC_FOXES, int GEN_FOOD_FOXES, int *objs);
Cell **copy_matrix(Cell **src, int rows, int cols);

// Prints the ecosystem matrix M with a border.
// Arguments:
//   M    – matrix of chars (allocated dynamically)
//   rows – number of rows in the matrix
//   cols – number of columns in the matrix
void print_ecosystem(Cell **M, int rows, int cols);
void print_age(Cell **M, int rows, int cols);
void print_hunger(Cell **M, int rows, int cols);
void print_all(Cell **M, int rows, int cols);


#endif // AUX_FUNC_H