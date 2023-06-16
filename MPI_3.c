# include "mpi.h"
# include <stdio.h>
# include <stdlib.h>

int main(int argc, char *argv[])
{
        int rank, size;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	char send_message[size][100];
	char* receive_message = (char*)malloc(100 * sizeof(char));
	
        if (size < 2){
                printf("Please run with at least two processes.\n");
                fflush(stdout);
                MPI_Finalize();

                return 0;
        }

        if (rank == 0){
                for (int i = 0; i < size; i++){
			sprintf(send_message[i], "Hi rank %d, I'm 余明昌 from Parallel Programming Design Course in 2023", i);
		}
	}
	MPI_Scatter(send_message, 100, MPI_CHAR, receive_message, 100, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	if (rank == 0){
		MPI_Gather(receive_message, 100, MPI_CHAR, send_message, 100, MPI_CHAR, 0, MPI_COMM_WORLD);
		for (int i = 1; i < size; i++){
			printf("Rank 0 got message from rank %d: %s\n", i, send_message[i]);
		}
        } else{
                printf("Rank %d got message from rank 0: %s\n", rank, receive_message);
                fflush(stdout);
		
		sprintf(receive_message, "Rank %d received. Thank you.", rank);
		MPI_Gather(receive_message, 100, MPI_CHAR, send_message, 100, MPI_CHAR, 0, MPI_COMM_WORLD);
        }

        MPI_Finalize();

        return 0;
}
