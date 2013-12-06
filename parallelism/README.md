dagan '13
-------------------------------------
shared memory and message passing solutions to some common algorithms

compile mpi :
	mpicc -o file file.c
run mpi :
    	mpiexec -n <num of proc> ./exec

compile pthread w/ gcc :
	gcc -o file file.c -lpthread


======
note : 
mpi_transpose.c is wildly dysfunctional and is probably not working
