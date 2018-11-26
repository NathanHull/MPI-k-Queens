all: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 1 ./exe 8 8
