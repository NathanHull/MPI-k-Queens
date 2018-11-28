#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define TAG 0


bool checkPos (int row, int col, int board[]) {
	int i;
	bool isValid = true;

	for (i = 0; i < col; i++) {
		// same row
		if (board[i] == row)
			isValid = false;
		// forwardslash diagonal
		else if (board[i] + i == row + col)
			isValid = false;
		// backslash diagonal
		else if (board[i] - i == row - col)
			isValid = false;
	}

	return isValid;
}


int checkRoutes (int row, int col, int board[], int dim) {
	int i;

	// Have reached the end
	if (col == dim - 1) {
		for (i = 0; i < dim; i++) {
			printf("%i ", board[i]);
		}
		printf("\n");
		return 1;
	}

	int solutions = 0;
	int nextCol = col + 1;

	for (i = 0; i < dim; i++) {
		if (checkPos (i, nextCol, board)) {
			board[nextCol] = i;
			solutions += checkRoutes (i, nextCol, board, dim);
			board[nextCol] = 0;
		}
	}

	return solutions;
}


int main (int argc, char** argv) {
	if (argc != 2) {
		fprintf (stderr, "Invalid input: ./exe.out k\n");
		return -1;
	}

	int dim = atoi (argv[1]);
	if (dim < 1) {
		fprintf (stderr, "Invalid input. k cannot be negative\n");
		return -1;
	}

	int rank, num_nodes;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num_nodes);

	int i;
	int buffer;
	int solutions = 0;

	// Serial implementation
	if (num_nodes == 1) {
		printf ("Checking %i queens on a %iX%i board\n", dim, dim, dim);

		for (i = 0; i < dim; i++) {
			int* board = (int*) malloc (dim * sizeof(int));
			board[0] = i;
			solutions += checkRoutes (i, 0, board, dim);
			free (board);
		}
		printf("Number of solutions: %i\n", solutions);
	}




	// Distributed implementation
	else {
		// too large of a board, need to split up work
		if (num_nodes < dim) {
			for (i = 0; i < dim; i++) {
				if (i % num_nodes == rank) {
					int* board = (int*) malloc(dim * sizeof(int));
					board[0] = i;
					solutions += checkRoutes (i, 0, board, dim);
					free (board);
				}
			}
		}
		// otherwise can just assign one row to each node
		else {
			int* board = (int*) malloc(dim * sizeof(int));
			board[0] = rank;
			solutions = checkRoutes (rank, 0, board, dim);
			free (board);
		}

		// reduce
		if (rank == MASTER) {
			printf ("Checking %i queens on a %iX%i board\n", dim, dim, dim);
			for (i = 1; i < num_nodes; i++) {
				MPI_Recv (&buffer, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				solutions += buffer;
			}
			printf("Number of solutions: %i\n", solutions);

		} else {
			MPI_Send (&solutions, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize ();
}
