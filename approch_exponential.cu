# include<stdio.h>
# include<stdlib.h>
# include<math.h>
# include<cuda_runtime.h>

__global__ static void sum_of_e(double *result, int power_of_e, int n, int thread_num, int block_num){
	double temp = 0.0;
	const int thread_id = threadIdx.x;
	const int block_id = blockIdx.x;
	double f;

	for (int i = block_id * thread_num + thread_id; i <= n; i += block_num * thread_num){
		f = 1.0;
		for (int j = 2; j <= i; j++){
			f *= j;
		}
		temp += pow(power_of_e, i) / f;
	}
	result[block_id * thread_num + thread_id] = temp;
}

int main(int argc, char *argv[]){
	int block_num = atoi(argv[1]);
	int thread_num = atoi(argv[2]);	
	int power_of_e = atoi(argv[3]);
	int n = atoi(argv[4]);

	double result = exp(power_of_e);
	printf("   Real              value   : %.50f\n", result);
	
	double result_cpu = 1.0;
	double f;
	clock_t start_cpu = clock();
	for (int i = 1; i <= n; i++){
		f = 1.0;
		for (int j = 2; j <= i; j++){
			f *= j;
		}
		result_cpu += pow(power_of_e, i) / f;
	}
	clock_t end_cpu = clock();

	printf("CPU time: %Lf, CPU value: %.50f\n", (long double)(end_cpu - start_cpu) / CLOCKS_PER_SEC, result_cpu);

	double *result_gpu;
	cudaMalloc((void**) &result_gpu, sizeof(double) * thread_num * block_num);
	clock_t start_gpu = clock();
	sum_of_e<<<block_num, thread_num>>>(result_gpu, power_of_e, n, thread_num, block_num);
	clock_t end_gpu = clock();
	
	double result_of_gpu[thread_num * block_num];
	cudaMemcpy(&result_of_gpu, result_gpu, sizeof(double) * thread_num * block_num, cudaMemcpyDeviceToHost);
	cudaFree(result_gpu);

	double final_e = 0.0;
	for (int i = 0; i < thread_num * block_num; i++){
		final_e += result_of_gpu[i];
	}

	printf("GPU time: %Lf, GPU value: %.50f\n", (long double)(end_gpu - start_gpu) / CLOCKS_PER_SEC, final_e);

	return 0;
}
