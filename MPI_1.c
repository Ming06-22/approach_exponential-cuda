# include "mpi.h"
# include <stdio.h>

int main(int argc, char *argv[])
{
	int rank, size;
	char message[100];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (size < 2){
		printf("Please run with at least two processes.\n");
		fflush(stdout);
		MPI_Finalize();
		
		return 0;
	}
	
	if (rank == 0){
		for (int i = 1; i < size; i++){
			sprintf(message, "Hi rank %d, I'm 余明昌 from Parallel Programming Design Course in 2023", i);
			MPI_Send(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
		for (int i = 1; i < size; i++){
			MPI_Recv(message, 100, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Rank 0 got message from rank %d: %s\n", i, message);
		}
	} else{
		MPI_Recv(message, 100, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
		printf("Rank %d got message from rank 0: %s\n", rank, message);	
		fflush(stdout);
		
		sprintf(message, "Rank %d recieved. Thank you.", rank);
		MPI_Send(message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	
	return 0;
}
