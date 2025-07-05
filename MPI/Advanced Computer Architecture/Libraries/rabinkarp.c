#include "rabinkarp.h"

void rabin_karp(char *txt, char *pattern, const size_t lentxt, const size_t lenpat, long long int *occurrences){

  size_t i,j;  
  // Number of characters in the input alphabet (ASCII)
  int P = 5;
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