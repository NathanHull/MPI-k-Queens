all: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 1 ./exe 8

mp: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 4 ./exe 8
