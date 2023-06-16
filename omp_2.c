# include<stdio.h>
# include<stdlib.h>
# include<omp.h>

int main(int argc, char *argv[]){
	int num_of_thread = atoi(argv[1]);
	int num_of_k = atoi(argv[2]);
	double pi = 0.0;
	double start_time, end_time;
	double partial_sum[num_of_thread];
	for (int i = 0; i < num_of_thread; i++){
		partial_sum[i] = 0;
	}
	
	start_time = omp_get_wtime();
	
	# pragma omp parallel for num_threads(num_of_thread)
		for (int k = 0; k < num_of_k; k++){
			double factor = (k % 2 == 0)? 1.0: -1.0;
			double partial = 4 * factor / (2 * k + 1);
			partial_sum[omp_get_thread_num()] += partial;
			# pragma omp critical
			pi += partial;
		}

	end_time = omp_get_wtime();

	for (int i = 0; i < num_of_thread; i++){
		printf("Processor %d out of %d sum: %f\n", i, num_of_thread, partial_sum[i]);
	}

	printf("Fianl estimated results with n=%d: %f\n", num_of_k, pi);
	printf("Author: B0928007 余明昌\n");
	printf("Execution time: %f seconds\n", end_time - start_time);
	
	return 0;
}

