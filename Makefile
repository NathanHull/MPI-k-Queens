k=8
np=4

all: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 1 ./exe 8

mp: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	mpirun -np 8 -H arch01,arch02,arch03,arch04,arch05,arch06,arch07,arch08 ./exe 8

mp1: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	time mpirun -np 1 -H arch01,arch02,arch03,arch04,arch05,arch06,arch07,arch08 ./exe 15

big: kqueen.cpp
	mpic++ kqueen.cpp -o exe
	time mpirun -np 1 -H arch03,arch04,arch05,arch06 ./exe ${k}
	sleep 5
	time mpirun -np 4 -H arch03,arch04,arch05,arch06 ./exe ${k}
