#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
//All variables and global functions are defined in variables.h
#include"variables.h"
//This library is for the SHA256 function implementation
#include "sha256.h"

//This function is to print a Hash in BINARY (It looks nicer!)
void printfHash(const BYTE* hash){
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    printf("%s", byteToBinary(hash[i]));
  printf("\n\n");
}

//Convert an int into binary
const char *byteToBinary(int x){
    static char b[9];
    b[0] = '\0';
    for (int z = 128; z > 0; z >>= 1)
        strcat(b, ((x & z) == z) ? "1" : "0");
    return b;
}

//Search for spaces ' ' and replace them with # (Useful for writing information)
void replaceSpaces(char string[], const int size){
  for(int i=0; i<size; i++)
    if(string[i] == ' ')
      string[i] = '#';
}

//This indicates if the difficulty parameter has been reached or not
const int switchPoW (const BYTE buf[], const int difficulty){
  int cond = 0;
  for(int i=0; i<difficulty;i++)
    //If there is any zero at the beginning then it is not valid (check PoW function as well)
    if (buf[i]>0)
      cond++;

  return cond;
}

//Creates the input for the SHA256 function
void createpreInput(const struct block *preBlock,char *result){
  //The input will be separated by a "/"
  char sep[] = "/";

  strcpy(result, preBlock->timeStamp);
  strcat(result,sep);
  strcat(result, preBlock->merkleRoot);
  strcat(result,sep);
  strcat(result,preBlock->previousHash);
  strcat(result,sep);
  strcat(result,preBlock->minerAddress);
  strcat(result,sep);

  //A tricky step to concatenate a number in the result string
  char IB[4];
  sprintf(IB,"%d",preBlock->index);
  strcat(result,IB);
  strcat(result,sep);
  puts("CreatingInput");
}

//Proof of Work function
void PoW (struct block * currentBlock, const int difficulty){
  //I still don't understand how to put a general Nonce with N bytes #Help!
  long double newNonce;
  newNonce = 0.0;

  //PHASE 1:
  //Setting up the timeStamp of the block
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  copyString(currentBlock->timeStamp,asctime(timeinfo),24);
  replaceSpaces(currentBlock->timeStamp,24);

  //PHASE 2:
  //Setting up the input and putting the first nonce (just to try if it works)
  BYTE preInput[300];
  BYTE newInput[300];
  createpreInput(currentBlock,preInput);
  strcpy(newInput,preInput);
  //PHASE 3:
  /*To concatenate the initial nonce to the preInput we have to transform it into
  BYTE (un char) and then concatenate it with the preInput string*/
  BYTE NB[10];
  sprintf(NB,"%Lf",newNonce);
  strcat(newInput,NB);

  //PHASE 4:
  //This is all SHA256 implementation #IDontUnderstandItYet
  BYTE newHash[SHA256_BLOCK_SIZE];
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, newInput, strlen(newInput));
  sha256_final(&ctx, newHash);

  //If the difficulty has not been reach, then continue
  while(switchPoW(newHash,difficulty)){
    //PHASE 3:
    //Change the nonce, the only variable parameter of the input
    newNonce = newNonce + 0.000001;
    strcpy(newInput,preInput);
    sprintf(NB,"%Lf",newNonce);
    strcat(newInput,NB);

    //PHASE 4:
    //Apply SHA256 again
    sha256_init(&ctx);
    sha256_update(&ctx, newInput, strlen(newInput));
    sha256_final(&ctx, newHash);
    /*puts("New Hash:");
    printfHash(newHash);*/
  }

  //PHASE 5:
  puts("PoWBlock:");
  for(int i=0; i<SHA256_BLOCK_SIZE; i++){
    //After finding the right hash, put it into the block structure
    currentBlock->blockHash[i] = newHash[i];
    //Print only the found hash
    printf("%X",currentBlock->blockHash[i]);
  }
  puts("");

  //Save the nonce as well (for later verification)
  currentBlock->nonce = newNonce;
}
