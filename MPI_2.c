# include "mpi.h"
# include <stdio.h>

int main(int argc, char *argv[])
{
        int rank, size;
        char message[100];
        MPI_Status status;
	MPI_Request request1, request2;

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
                        MPI_Isend(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request1);
                	MPI_Wait(&request1, &status);
		}
                
		for (int i = 1; i < size; i++){
                        MPI_Irecv(message, 100, MPI_CHAR, i, 1, MPI_COMM_WORLD, &request2);
                        MPI_Wait(&request2, &status);
			printf("Rank 0 got message from rank %d: %s\n", i, message);
		}
        } else{
                MPI_Irecv(message, 100, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request1);
		MPI_Wait(&request1, &status);

                printf("Rank %d got message from rank 0: %s\n", rank, message);
                fflush(stdout);

                sprintf(message, "Rank %d recieved. Thank you.", rank);
                MPI_Isend(message, 100, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &request2);
		MPI_Wait(&request2, &status);
        }

        MPI_Finalize();

        return 0;
}
