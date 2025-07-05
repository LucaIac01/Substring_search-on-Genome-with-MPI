#include <stdio.h>		
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

void rabin_karp(char *txt, char *pattern, const size_t lentxt, const size_t lenpat, long long int *occurrences);

char *readFile(char *filename, size_t *len) {
    
    FILE *f = fopen(filename, "r");
    fseeko(f, 0, SEEK_END);
    *len = ftello(f);
    rewind(f);
    char *txt = (char *)malloc(*len + 1);

    if (fread(txt, 1, *len, f) != *len) {
        fprintf(stderr, "Error, wrong input\n");
        free(txt);
        fclose(f);
        return NULL;
    }

    txt[*len] = '\0';
    fclose(f);
    return txt;
}


void rabin_karp(char *txt, char *pattern, const size_t lentxt, const size_t lenpat, long long int *occurrences){

  size_t i,j;  
  // Number of characters in the input alphabet (ASCII)
  int P = 256;
  // A prime number for modulo operations to reduce collisions
  int M = 101;
  // Hash value for pattern
  long long int pat_hash = 0;
  // Hash value for current window of text
  long long int txt_hash = 0;
  // High-order digit multiplier
  long long int h = 1;
       
  *occurrences = 0;


	// Precompute h = pow(P, M-1) % q
  for (i = 0; i < lenpat - 1; i++)
  		h = (h * P) % M;

  // Compute initial hash values for pattern and first window of text
  for(i = 0; i < lenpat; i++){
    	pat_hash = (P * pat_hash + *(pattern + i)) % M;
      txt_hash = (P * txt_hash + *(txt + i)) % M;
    }

  // Slide the pattern over text one by one
  for(i = 0; i <= lentxt - lenpat; i++){
    
  	 // If hash values match, check characters one by one
     if(pat_hash == txt_hash){
      	for(j = 0; j < lenpat; j++){
          if(*(txt + i + j) != *(pattern + j)){
            break;
          }
        }
        if(j == lenpat){
          (*occurrences)++;
        }
      }

      // Calculate hash value for the next window
      if(i < lentxt - lenpat){       
        txt_hash = (P * (txt_hash - *(txt + i) * h) + *(txt + i + lenpat)) % M;
        // Ensure hash value is non-negative
        if(txt_hash < 0)
          txt_hash = txt_hash + M;
      }
    }
}


int main(int argc, char const *argv[])
{
	clock_t begin = clock();
	size_t txtlen;
	size_t patlen;
	if (argc != 3)
	{
		printf("Error, insert file name\n");
		exit(1);
	}
	long long int occurrences = 0;
	char *pattern = readFile((char*)argv[2], &patlen);
  if (pattern[patlen - 1] == '\n') {
    pattern[--patlen] = '\0';
}
	char *txt = readFile((char*)argv[1], &txtlen);

	rabin_karp(txt, pattern, txtlen, patlen, &occurrences);
	printf("OCCURENCES FOUND:%lld\n", occurrences);


	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%.5f\n", time_spent);


	return 0;
}