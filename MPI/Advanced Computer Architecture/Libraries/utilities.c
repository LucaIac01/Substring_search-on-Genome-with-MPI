#include "utilities.h"

void null_check(void *ptr){
	if (!ptr)
	{
		printf("Internal error. Please, try again\n");
		exit(1);
	}
}


size_t who_is_active(int *flag, size_t txtlen, size_t patlen, int cores){

	size_t actives = txtlen/patlen;
	if (actives > cores)
	{
		actives = cores;
	}

	for (int i = 0; i < cores; ++i)
	{
		if (i < actives)
		{
			flag[i] = 1;
		} else flag[i] = 0;
	}

	return actives;
}


char *readFile(const char *filename, size_t *len) {
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


char *split_dataset(char *txt, size_t *chunklen, size_t txtlen, size_t patlen, size_t offset, int executors){

	char *chunk = (char *)malloc(sizeof(char) * (offset +1));
	null_check(chunk);
	strncpy(chunk, txt, offset+1);
	chunk[offset] = '\0';
	*chunklen = offset;

		for (int i = 1; i < executors; ++i)
		{
			if (i <= executors-2)
			{
					MPI_Send(txt+(offset*i)-patlen+1, offset + patlen-1, MPI_CHAR, i, 100, MPI_COMM_WORLD);
			} else {
					MPI_Send(txt+(offset*i)-patlen+1, txtlen - offset*i + patlen -1, MPI_CHAR, i, 101, MPI_COMM_WORLD);
			}
		}
	return chunk;
}


char *receive_dataset(size_t offset, size_t txtlen, size_t patlen, size_t *chunklen, int rank, int executors){

	if (rank < executors-1)
	{
		*chunklen = offset + patlen -1;
		char *chunk = (char *)malloc(sizeof(char)*(*chunklen +1));
		null_check(chunk);
		chunk[*chunklen]='\0';
		MPI_Recv(chunk, *chunklen, MPI_CHAR, 0, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		return chunk;
	}

	if (rank == executors-1)
	{
		*chunklen = txtlen - rank*offset + patlen -1;
		char *chunk = (char *)malloc(sizeof(char)*(*chunklen + 1));
		null_check(chunk);
		chunk[*chunklen] = '\0';
		MPI_Recv(chunk, *chunklen, MPI_CHAR, 0, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		return chunk;
	}
	return NULL;
}