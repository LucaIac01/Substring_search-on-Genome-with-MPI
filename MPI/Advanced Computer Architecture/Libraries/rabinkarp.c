#include "rabinkarp.h"

void rabin_karp(char *txt, char *pattern, const size_t lentxt, const size_t lenpat, long long int *occurrences){
  size_t i,j;                      
  long long int pat_hash = 0;           
  long long int txt_hash = 0;
  long long int h = 1;
  *occurrences = 0;

  for (i = 0; i < lenpat - 1; i++)
  		h = (h * P) % M;

  for(i = 0; i < lenpat; i++){
    	pat_hash = (P * pat_hash + *(pattern + i)) % M;
      txt_hash = (P * txt_hash + *(txt + i)) % M;
    }

  for(i = 0; i <= lentxt - lenpat; i++){
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

    
      if(i < lentxt - lenpat){
        txt_hash = (P * (txt_hash - *(txt + i) * h) + *(txt + i + lenpat)) % M;

        if(txt_hash < 0)
          txt_hash = txt_hash + M;
      }
    }
}