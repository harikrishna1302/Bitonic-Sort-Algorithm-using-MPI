#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>     
#include <time.h>           
#include "mpi.h"        

void High(int bit);
void Low(int bit);
int Comparison(const void * x, const void * y);

#define MASTER 0        
#define NUMBER 10   

int * array;
int array_size;
int process_rank;
int num_processes;
double start_time;
double end_time;


int main(int argc, char * argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

	array_size = atoi(argv[1]) / num_processes;
	array = (int *)malloc(array_size * sizeof(int));
	srand(time(NULL)); 
	int i, j;
	for (i = 0; i < array_size; i++) {
		array[i] = rand() % (atoi(argv[1]));
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	int stages = (int)(log2(num_processes));
	if (process_rank == MASTER) {
		printf("Number of Processes is: %d\n", num_processes);
		timer_start = MPI_Wtime();
	}

	qsort(array, array_size, sizeof(int), Comparison);
	for (i = 0; i < stages; i++) {
		for (j = i; j >= 0; j--) {
			
			printf("%d ",process_rank>>(i+1));
			if (((process_rank >> (i + 1)) % 2 == 0 && (process_rank >> j) % 2 == 0) || ((process_rank >> (i + 1)) % 2 != 0 && (process_rank >> j) % 2 != 0)) {
				Low(j);
			}
			else {
				High(j);
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (process_rank == MASTER) {
		end_time = MPI_Wtime();

		printf("Sample sorted array for verification(10 elements)\n");
	
		for (i = 0; i < array_size; i++) {
			if ((i % (array_size / NUMBER)) == 0) {
				printf("%d ", array[i]);
			}
		}
		printf("Time in seconds : %f\n", end_time - start_time);
	}
	
	free(array);
	MPI_Finalize();
	return 0;
}
void High(int j) {
	int i, max;

	
	int counter_receive;
	int * buffer_recieve = (int *)malloc((array_size + 1) * sizeof(int));
	MPI_Recv(&max, 1, MPI_INT, process_rank ^ (1 << j), 0, MPI_COMM_WORLD,  MPI_STATUS_IGNORE );

	
	int counter_send = 0;
	int * buffer_send = (int *)malloc((array_size + 1) * sizeof(int));
	MPI_Send(&array[0], 1, MPI_INT, process_rank ^ (1 << j), 0, MPI_COMM_WORLD);

	
	for (i = 0; i < array_size; i++) {
		if (array[i] < max) {
			buffer_send[counter_send + 1] = array[i];
			counter_send++;
		}
		else {
			break;      
		}
	}
	MPI_Recv(buffer_recieve, array_size, MPI_INT,	process_rank ^ (1 << j), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
	counter_receive = buffer_recieve[0];
	buffer_send[0] = counter_send;
	MPI_Send(buffer_send, counter_send,	MPI_INT, process_rank ^ (1 << j), 0, MPI_COMM_WORLD);

	for (i = 1; i < counter_receive + 1; i++) {
		if (buffer_recieve[i] > array[0]) {
			array[0] = buffer_recieve[i];
		}
		else {
			break;      
		}
	}

	qsort(array, array_size, sizeof(int), ComparisonFunc);
	free(buffer_send);
	free(buffer_recieve);
}

void Low(int j) {
	int i, min;

	
	int counter_send = 0;
	int * buffer_send = (int *)malloc((array_size + 1) * sizeof(int));
	MPI_Send(&array[array_size - 1],1,MPI_INT,process_rank ^ (1 << j),0,MPI_COMM_WORLD);
	int counter_receive;
	int * buffer_recieve = (int *)malloc((array_size + 1) * sizeof(int));
	MPI_Recv(&min,1,MPI_INT,process_rank ^ (1 << j),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

	
	for (i = 0; i < array_size; i++) {
		if (array[i] > min) {
			buffer_send[counter_send + 1] = array[i];
			counter_send++;
		}
		else {
			break;      
		}
	}

	buffer_send[0] = counter_send;
	MPI_Send(buffer_send,counter_send,MPI_INT,process_rank ^ (1 << j),0,MPI_COMM_WORLD);
	MPI_Recv(buffer_recieve,array_size,MPI_INT,process_rank ^ (1 << j),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

	for (i = 1; i < buffer_recieve[0] + 1; i++) {
		if (array[array_size - 1] < buffer_recieve[i]) {
			
			array[array_size - 1] = buffer_recieve[i];
		}
		else {
			break;      
		}
	}
	qsort(array, array_size, sizeof(int), ComparisonFunc);
	free(buffer_send);
	free(buffer_recieve);

}

int Comparison(const void * x, const void * y) {
	return (*(int *)x - *(int *)y);
}