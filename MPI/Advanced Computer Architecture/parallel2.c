#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>
#include "./Libraries/utilities.h"
#include "./Libraries/rabinkarp.h"


// Main function
int main(int argc, char *argv[])
{
int worker_rank;
int size;
int isActive = 0;
int num_workers;
char *text_data;
char *search_pattern;
char *data_chunk;
size_t text_length;
size_t pattern_length;
size_t chunklen;
size_t chunk_size = 0;
long long int occurrences = 0;
long long int total = 0;

// Initialize MPI environment
MPI_Init(&argc, &argv);
// Get the worker_rank of the current process
MPI_Comm_rank(MPI_COMM_WORLD, &worker_rank); 								
// Get total number of processes
MPI_Comm_size(MPI_COMM_WORLD, &size); 								

int worker_flags[size]; 

// Master read file and calculate number of workers to use
if (worker_rank == 0)
{
	text_data = readFile(argv[1], &text_length);							
	search_pattern = readFile(argv[2], &pattern_length);							
	if (search_pattern[pattern_length - 1] == '\n')
    search_pattern[--pattern_length] = '\0';
	null_check(text_data);									
	null_check(search_pattern);									
	num_workers = who_is_active(worker_flags, text_length, pattern_length, size); 					
}
	clock_t begin = clock(); 
	MPI_Bcast(&num_workers, 1, MPI_INT, 0, MPI_COMM_WORLD);					
	MPI_Scatter(worker_flags, 1, MPI_INT, &isActive, 1, MPI_INT, 0, MPI_COMM_WORLD);		
	MPI_Bcast(&text_length, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);				
	MPI_Bcast(&pattern_length, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);				

// Active core
if (isActive) {
	chunk_size = text_length/num_workers;
	// Master splits and sends chunks to workers								
	if (worker_rank == 0) {
		data_chunk = split_dataset(text_data, &chunklen, text_length, pattern_length, chunk_size, num_workers); 	
		null_check(data_chunk);
		free(text_data);
		for (int worker_id = 1; worker_id < num_workers; ++worker_id){
			MPI_Send(search_pattern, pattern_length, MPI_CHAR, worker_id, 105, MPI_COMM_WORLD); 		
		}	
	} else {		// Worker receives assigned text data_chunk
		search_pattern = (char *)malloc(sizeof(char)*(pattern_length+1));
		null_check(search_pattern);
		search_pattern[pattern_length]='\0';
		data_chunk = receive_dataset(chunk_size, text_length, pattern_length, &chunklen, worker_rank, num_workers); 	
		null_check(data_chunk);
		MPI_Recv(search_pattern, pattern_length, MPI_CHAR, 0, 105, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		}

		// Perform Rabin-Karp search on assigned data_chunk
		rabin_karp(data_chunk, search_pattern, chunklen, pattern_length, &occurrences);
	
		free(search_pattern); 
		free(data_chunk); 
	}

printf("OCCURRENCES worker_rank %d: %lld\n",worker_rank, occurrences);
	
MPI_Reduce(&occurrences, &total, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);		
clock_t end = clock(); 										

if (worker_rank == 0) {
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Total occurrences %lld\n", total);
	printf("Time required %lf\n", time_spent);
	printf("Program executed by %d cores over %d\n", num_workers, size);
}
	
// Clean up and close MPI environment
MPI_Finalize();	

return 0;
}
