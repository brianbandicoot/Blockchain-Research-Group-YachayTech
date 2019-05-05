#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"variables.h"
#include "sha256.h"

/*It says if the block has not been modified by performing a SHA256 over the input block.
If the valid value is 1 then everything is OK, if it is 0 then it has been modified*/
int validate(const struct block *B){
  //puts("Validate");
  //PHASE 1:
  //Create a preInput as if we were doing PoW
  BYTE preInput[300];
  BYTE newInput[300];
  createpreInput(B,preInput);
  //puts("AfterInput");
  strcpy(newInput,preInput);
  BYTE NB[10];
  //We already same the nonce, so we should concatenate it into the preInput string
  sprintf(NB,"%Lf",B->nonce);
  strcat(newInput,NB);
  //PHASE 2:
  //Execute the SHA256 function and save the results in newHash
  BYTE newHash[SHA256_BLOCK_SIZE];
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, newInput, strlen(newInput));
  sha256_final(&ctx, newHash);

  //PHASE 3:
  //The valid number will be the output of this function. 1 if OK, 0 if it mas modified
  int valid = 1;
  /*The blockHash produced on PHASE 2 will output a BYTE (un char) array.
  In order to compared with our readedHash we need to transform it into a char array.*/
  char calculatedHash[SHA256_BLOCK_SIZE*2];
  for(int i=0,j=0,k=0; j<(SHA256_BLOCK_SIZE*2);i++){
    /*newHash[i] in %X will print something like 3A. This output has two elements: 3 & A.
    That is why we need to spend the char array pointer calculatedHash[j] in a even sequence.
    calculatedHash[i] will grab the first element while calculatedHash[i+1] will grab the second*/
    sprintf(&calculatedHash[j],"%X",newHash[i]);
    j=j+2;
    /*Some elements will not be ASCII readable characters (so they will output 0).
    Imagine that newHash[i] = 5. Then calculatedHash[i] = ? and calculatedHash[i+1] = 5*/
    if(calculatedHash[i] !=0){
      //Compare if any element of calculatedHash is different from the readedHash.
      if(calculatedHash[i]!=B->readedHash[k])
        valid = 0;
      //The readedHash pointer will move only if calculatedHash[i] is different than zero to avoid an array translation with readedHash.
      k++;
    }
  }

  return valid;
}

//It reads and verifies the whole blockchain
void readBlockchain(struct block *B, const int size){
  //ind will be block indexes and cond will be a control switch
  int ind=0,cond=1;
  //Read a single block and store data in B
  //It reads the first line of the blockchain.dat file
  FILE *fp;
  fp = fopen("blockchain.dat", "a+");
  while(ind<size){
    readBlock(fp,B);
    //If B is valid, then continue to the next index
    if(validate(B)){
      printf("Block %d validated\n",ind+1);
      ind++;
    }
    //If it is not valid, deactive the cond switch and stop the reading
    else{
      cond = 0;
      break;
    }
  }
  fclose(fp);
  //puts("finished closing");

  //#Help! I don't understand how to implement this
  if(cond)
    puts("Blockchain.dat valid");
  else
    puts("Blockchain.dat is invalid");
}
