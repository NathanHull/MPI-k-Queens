#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <queue>
#include <iostream>

#define MASTER 0

using namespace std;


// Trivially copyable (contiguous in memory) struct
// so it can be sent via MPI_Send
struct Partial {
	int nextCol;
	int board[];

	~Partial() {
		if (board) delete [] board;
	}
};


int main (int argc, char** argv) {
	if (argc != 3) {
		fprintf (stderr, "Invalid input: ./exe.out numQueens boardDim\n");
		return -1;
	}

	int numQueens = atoi (argv[1]);
	if (numQueens < 0) {
		fprintf (stderr, "Invalid input. k cannot be negative\n");
		return -1;
	}

	int dim = atoi (argv[2]);
	if (dim < 0) {
		fprintf (stderr, "Invalid input. Board dim cannot be negative\n");
	}

	printf ("Checking %i queens on a %iX%i board\n", numQueens, dim, dim);

	int i;
	
	int rank, num_nodes;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num_nodes);

	if (rank == MASTER) {
		queue<Partial> pool;
		for (i = 0; i < dim; i++) {
			struct Partial seed;
			seed.nextCol = 1;
			seed.board = malloc (sizeof (int) * dim);
			seed.board[0] = i;

			pool.push (seed);
		}

		while (!pool.empty()) {
			
		}
	} else {

	}

	MPI_Finalize ();
}


bool check (int row, int board[]) {
	return false;
}
