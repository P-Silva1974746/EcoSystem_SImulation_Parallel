run-seq:
	gcc -o seq sequencial.c aux_func.c && ./seq <input.txt >output.txt