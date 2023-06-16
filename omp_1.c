# include<stdio.h>
# include<stdlib.h>
# include<omp.h>

int main(int argc, char *argv[]){
	int num_of_thread = atoi(argv[1]);
	int num_of_k = atoi(argv[2]);
	double pi = 0.0;
	double start_time, end_time;

	start_time = omp_get_wtime();
	
	# pragma omp parallel num_threads(num_of_thread)
	{
		double partial = 0;
		for (int k = omp_get_thread_num(); k < num_of_k; k += num_of_thread){
			double factor = (k % 2 == 0)? 1.0: -1.0;
			partial += factor / (2 * k + 1);
		}
		printf("Processor %d out of %d sum: %f\n", omp_get_thread_num(), num_of_thread, partial);
		# pragma omp critical
		pi += 4.0 * partial;
	}
	
	end_time = omp_get_wtime();

	printf("Final estimated results with n=%d: %f\n", num_of_k, pi);
	printf("Author: B0928007 余明昌\n");
	printf("Execution time: %f seconds\n", end_time - start_time);

	return 0;
}

