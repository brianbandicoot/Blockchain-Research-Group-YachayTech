#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"variables.h"
#include "sha256.h"

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';
    for (int z = 128; z > 0; z >>= 1)
        strcat(b, ((x & z) == z) ? "1" : "0");
    return b;
}

int GenerateNonce()
{
  srand(time(NULL));
  return rand();
}

int switchPoW (BYTE buf[SHA256_BLOCK_SIZE], int difficulty)
{
  int cond = 0;
  for(int i=0; i<difficulty;i++)
    if (buf[i]>0)
      cond++;

  return cond;
}

void createpreInput(struct block *preBlock,char *result)
{
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

void PoW (struct block * currentBlock, int difficulty)
{
  int newNonce;
  newNonce = GenerateNonce();

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  currentBlock->timestamp = asctime(timeinfo);

  BYTE preInput[300];
  BYTE newInput[300];
  createpreInput(currentBlock,preInput);
  strcpy(newInput,preInput);
  BYTE NB[4];
  sprintf(NB,"%d",newNonce);
  strcat(newInput,NB);

  BYTE newHash[SHA256_BLOCK_SIZE];
  SHA256_CTX ctx;

  sha256_init(&ctx);
  sha256_update(&ctx, newInput, strlen(newInput));
  sha256_final(&ctx, newHash);

  while(switchPoW(newHash,difficulty))
  {
    newNonce = GenerateNonce();
    strcpy(newInput,preInput);
    sprintf(NB,"%d",newNonce);
    strcat(newInput,NB);

    sha256_update(&ctx, newInput, strlen(newInput));
    sha256_final(&ctx, newHash);
    puts("New Hash:");
    for(int i=0; i<SHA256_BLOCK_SIZE; i++)
      printf("%s", byte_to_binary(newHash[i]));
    printf("\n\n");
  }

  currentBlock->blockHash = newHash;
  currentBlock->nonce     = newNonce;
  puts("Block Hash found:");
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    printf("%s", byte_to_binary(newHash[i]));

  printf("Nonce used: %d",newNonce);
}
