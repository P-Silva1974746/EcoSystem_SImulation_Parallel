run-seq:
	gcc -o seq -fopenmp sequencial.c aux_func.c && ./seq <ecosystem_examples/input200x200 >output.txt

run-parallel:
	gcc -o par -fopenmp parallel.c aux_func_parallel.c && ./par <ecosystem_examples/input200x200 >output.txt