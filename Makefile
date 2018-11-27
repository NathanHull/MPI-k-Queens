all: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 1 ./exe 8

mp: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 8 -H arch02,arch03,arch04,arch05,arch06,arch07,arch08,arch09 ./exe 8

mp1: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 6 -H arch04,arch05,arch06,arch07,arch08,arch09 ./exe 8
