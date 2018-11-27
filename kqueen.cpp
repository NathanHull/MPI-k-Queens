#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DIM 8
#define MASTER 0
#define TAG 0

using namespace std;


bool checkPos (int row, int col, int board[]) {
	int i;
	bool isValid = true;

	for (i = 0; i < col; i++) {
		// same row
		if (board[i] == row) {
			isValid = false;
			printf("Same row used %i %i\n", board[i], i);
		}
		// forwardslash diagonal
		else if (board[i] + i == row + col) {
			isValid = false;
			printf("Forwardslash %i %i\n", board[i], i);
		}
		// backslash diagonal
		else if (board[i] - i == row - col) {
			isValid = false;
			printf("Backwardslash %i %i\n", board[i], i);
		}
	}
	printf("CHECKED %ix%i is %i\n", col, row, isValid);

	return isValid;
}


int checkRoutes (int row, int col, int board[], int dim) {
	// Have reached the end
	if (col == dim - 1)
		return 1;

	int solutions = 0;
	int i;
	int nextCol = col + 1;

	for (i = 0; i < dim; i++) {
		if (checkPos (i, nextCol, board)) {
		int x;
		for (x = 0; x < dim; x++) {
			printf("%i ", board[x]);
		}
		printf("\n");
			board[nextCol] = i;
			solutions += checkRoutes (i, nextCol, board, dim);
			board[nextCol] = 0;
		}
	}

	return solutions;
}


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
	dim = DIM;

	printf ("Checking %i queens on a %iX%i board\n", numQueens, dim, dim);

	int i;
	
	int rank, num_nodes;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &num_nodes);

	int buffer;
	int solutions = 0;

	if (num_nodes == 1) {
		for (i = 0; i < dim; i++) {
			int board[] = {-1, -1, -1, -1, -1, -1, -1, -1};
			board[0] = i;
			buffer = i;
			solutions += checkRoutes (buffer, 0, board, dim);
		}
	}
	else {

		if (rank == MASTER) {
			for (i = 0; i < dim; i++) {
				buffer = dim;
				MPI_Send (&buffer, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
			}
		} else {
			MPI_Recv (&buffer, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			int board[] = {-1, -1, -1, -1, -1, -1, -1, -1};
			board[0] = buffer;
			
			solutions += checkRoutes (buffer, 1, board, dim);
		}
	}

	printf("Number of solutions: %i\n", solutions);

	MPI_Finalize ();
}


