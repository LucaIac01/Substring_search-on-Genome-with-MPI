#include "utilities.h"

// Checks if a pointer is NULL and exits with an error if so terminate the program
void null_check(void *memory_ptr){
	if (!memory_ptr)
	{
		printf("Internal error. Please, try again\n");
		exit(1);
	}
}


// Determines how many workers (num_workers) will be active based on text/search_pattern length
size_t who_is_active(int *active_flags, size_t text_length, size_t pattern_length, int cores){

	size_t active_workers = text_length/pattern_length;
	// Limit the number of active workers to the available cores
	if (active_workers > cores)
	{
		active_workers = cores;
	}

	// Set the active flags for each core (1 = active, 0 = inactive)
	for (int worker_idx = 0; worker_idx < cores; ++worker_idx)
	{
		if (worker_idx < active_workers)
		{
			active_flags[worker_idx] = 1;
		} else active_flags[worker_idx] = 0;
	}

	return active_workers;
}


char *readFile(char *filename, size_t *len) {
  // Open the file in read mode
    FILE *f = fopen(filename, "r");                       
    if (!f) {
        perror("Error opening file");                  
        return NULL;
    }

    // Move to the end of the file to get its size
    if (fseeko(f, 0, SEEK_END) != 0) {                     
        perror("fseeko failed");                          
        fclose(f);
        return NULL;
    }

    // Get current file position (i.e., file size)
    off_t size = ftello(f);                               
    if (size == -1) {
        perror("ftello failed");                     
        fclose(f);
        return NULL;
    }

    // Save the file size to output parameter
    *len = (size_t)size;        
    // Go back to the beginning of the file                          
    rewind(f);                                            

    // Allocate memory for the file + null terminator
    char *txt = (char *)malloc(*len + 1);                 
    if (!txt) {
        fprintf(stderr, "Memory allocation failed\n");   
        fclose(f);
        return NULL;
    }

    // Read the entire file into memory
    if (fread(txt, 1, *len, f) != *len) {                 
        fprintf(stderr, "File read error\n");             
        free(txt);
        fclose(f);
        return NULL;
    }

    // Null-terminate the string
    txt[*len] = '\0';                                     
    fclose(f);                                            
    return txt;                                           
}


// Splits the text into chunks and sends them to other MPI processes
char *split_dataset(char *text_data, size_t *chunklen, size_t text_length, size_t pattern_length, size_t chunk_size, int num_workers){

	char *text_chunk = (char *)malloc(sizeof(char) * (chunk_size +1));
	null_check(text_chunk);
	strncpy(text_chunk, text_data, chunk_size+1);
	text_chunk[chunk_size] = '\0';
	*chunklen = chunk_size;

	// Send text chunks to other worker processes (worker_rank > 0)
	for (int worker_idx = 1; worker_idx < num_workers; ++worker_idx)
	{
		if (worker_idx <= num_workers-2)
		{
			MPI_Send(text_data+(chunk_size*worker_idx)-pattern_length+1, chunk_size + pattern_length-1, MPI_CHAR, worker_idx, 100, MPI_COMM_WORLD);
		} else {
			// Send the remaining part to the last worker
			MPI_Send(text_data+(chunk_size*worker_idx)-pattern_length+1, text_length - chunk_size*worker_idx + pattern_length -1, MPI_CHAR, worker_idx, 101, MPI_COMM_WORLD);
		}
	}
	return text_chunk;
}


// Receives a text_chunk of the text sent by worker_rank 0 (master process)
char *receive_dataset(size_t chunk_size, size_t text_length, size_t pattern_length, size_t *chunklen, int worker_rank, int num_workers){

	// For all workers except the last one: receive a fixed-size text_chunk
	if (worker_rank < num_workers-1)
	{
		*chunklen = chunk_size + pattern_length -1;
		char *text_chunk = (char *)malloc(sizeof(char)*(*chunklen +1));
		null_check(text_chunk);
		text_chunk[*chunklen]='\0';
		MPI_Recv(text_chunk, *chunklen, MPI_CHAR, 0, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		return text_chunk;
	}

	// Last worker gets the remaining bytes
	if (worker_rank == num_workers-1)
	{
		*chunklen = text_length - worker_rank*chunk_size + pattern_length -1;
		char *text_chunk = (char *)malloc(sizeof(char)*(*chunklen + 1));
		null_check(text_chunk);
		text_chunk[*chunklen] = '\0';
		MPI_Recv(text_chunk, *chunklen, MPI_CHAR, 0, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		return text_chunk;
	}
	// Should never reach this if worker_rank is valid
	return NULL;
}