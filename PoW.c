#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"variables.h"
#include "sha256.h"

const char *byteToBinary(int x){
    static char b[9];
    b[0] = '\0';
    for (int z = 128; z > 0; z >>= 1)
        strcat(b, ((x & z) == z) ? "1" : "0");
    return b;
}

int switchPoW (BYTE buf[SHA256_BLOCK_SIZE], int difficulty){
  int cond = 0;
  for(int i=0; i<difficulty;i++)
    if (buf[i]>0)
      cond++;

  return cond;
}

void createpreInput(struct block *preBlock,char *result){
  char sep[] = "/";
  strcpy(result, preBlock->timestamp);
  strcat(result,sep);
  strcpy(result, preBlock->merkleroot);
  strcat(result,sep);
  strcat(result,preBlock->previousHash);
  strcat(result,sep);
  strcat(result,preBlock->minerAddress);
  strcat(result,sep);
  char IB[4];
  sprintf(IB,"%d",preBlock->index);
  strcat(result,IB);
  strcat(result,sep);
}

void PoW (struct block * currentBlock, int difficulty){
  long double newNonce;
  newNonce = 0.0;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  currentBlock->timestamp = asctime(timeinfo);

  BYTE preInput[300];
  BYTE newInput[300];
  createpreInput(currentBlock,preInput);
  strcpy(newInput,preInput);
  BYTE NB[10];
  sprintf(NB,"%Lf",newNonce);
  strcat(newInput,NB);

  BYTE newHash[SHA256_BLOCK_SIZE];
  SHA256_CTX ctx;

  sha256_init(&ctx);
  sha256_update(&ctx, newInput, strlen(newInput));
  sha256_final(&ctx, newHash);

  while(switchPoW(newHash,difficulty)){
    newNonce = newNonce + 0.000001;
    strcpy(newInput,preInput);
    sprintf(NB,"%Lf",newNonce);
    strcat(newInput,NB);

    sha256_update(&ctx, newInput, strlen(newInput));
    sha256_final(&ctx, newHash);
    puts("New Hash:");
    //for(int i=0; i<SHA256_BLOCK_SIZE; i++)
      //printf("%s", byteToBinary(newHash[i]));
    printf("\n\n");
  }
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    currentBlock->blockHash[i] = newHash[i];
  currentBlock->blockHash[SHA256_BLOCK_SIZE+1] = '\0';

  currentBlock->nonce     = newNonce;
  /*puts("\nBlock Hash found:");
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    printf("%s", byteToBinary(newHash[i]));

  printf("\nNonce used: %Lf",newNonce);*/
}
