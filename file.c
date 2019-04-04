#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"variables.h"
#include "sha256.h"

const char* numToChar(int *x, int size){
  char result[size];
  for(int i=0; i>size; i++)
    result[i] = (char) x[i];
  return result;
}

void writeBlock(struct block * B){
  char hash[SHA256_BLOCK_SIZE];
  hash = numToChar(B->blockHash,SHA256_BLOCK_SIZE);
   FILE *fp;
   fp = fopen("blockchain.txt", "a+");
   /*fprintf(fp, "Block: %d\nMerkle Root: %s\nPrevious Hash: %s\nBlock Hash: %s\nMiner Address: %s\nNounce: %Lf\n",
                B->index,
                B->merkleroot,
                B->previousHash,
                B->blockHash,
                B->minerAddress,
                B->nonce);*/


/*   fprintf(fp, "Merkle Root: %s\n", B->merkleroot);
   fprintf(fp, "Previous Hash: %s\n", b->previousHash);
   fprintf(fp, "Block Hash: %s\n", b->blockHash);
   fprintf(fp, "Miner Address: %s\n", b->minerAddress);
   fprintf(fp, "Nounce: %Lf\n", b->nounce);*/
   fclose(fp);
}
