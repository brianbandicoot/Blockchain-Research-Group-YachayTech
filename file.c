#include<stdio.h>
#include<string.h>
#include <sys/stat.h>
#include <time.h>
//All variables and global functions are defined in variables.h
#include"variables.h"

//A strncpy function developed by ourselves to copy data from destination to source
void copyString(char destination[],const char source[],const int size){
  for(int i=0; i<size; i++)
    destination[i] = source[i];
  destination[size] = '\0';
}

//It does what the name says
void writeBlock(const struct block *B){
   FILE *fp;
   //Opens a blockchain.dat and append the new infomration
   fp = fopen("blockchain.dat", "a+");
   fprintf(fp, "Block: %d\nMerkleRoot: %s\nPreviousHash: %s",
                B->index,
                B->merkleRoot,
                B->previousHash);
  fprintf(fp,"\nBlockHash: ");
  //Saving the blockhash is a little bit tricky #Help!
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    fprintf(fp,"%X",B->blockHash[i]);
  fprintf(fp, "\nMinerAddress: %s\nNonce: %Lf",
               B->minerAddress,
               B->nonce);
  fprintf(fp, "\nTimeStamp: %s\n",B->timeStamp);
  fclose(fp);
}

//It does what the name says
void readBlock(struct block* B, const int index){
   FILE *fp;
   //To get the data tags ex: "BlockHash", "MinerAddress", etc
   char name[14];
   //To get the blockhash data as a string from blockchain.dat
   char tempHash[(SHA256_BLOCK_SIZE)*2-2];
   //It reads the first line of the blockchain.dat file
   fp = fopen("blockchain.dat", "a+");
   fscanf(fp,"%s %d",name,&B->index);
   printf("Index:%d\n",B->index);
   fscanf(fp,"%s %s\n",name,B->merkleRoot);
   B->merkleRoot[SHA256_BLOCK_SIZE*2] = '\0';
   fscanf(fp,"%s %s\n",name,B->previousHash);
   B->previousHash[SHA256_BLOCK_SIZE*2] = '\0';
   fscanf(fp,"%s %s\n",name,tempHash);
   tempHash[(SHA256_BLOCK_SIZE*2)-2] = '\0';

   //A tricky step to get the blockHash as it should be
   for(int i=0; i<(SHA256_BLOCK_SIZE)*2-2; i++)
     B->blockHash[i] = (int) tempHash[i];

   fscanf(fp,"%s %s\n",name,B->minerAddress);
   B->minerAddress[36] = '\0';
   fscanf(fp,"%s %Lf\n",name,&B->nonce);
   fscanf(fp,"%s %s\n",name,B->timeStamp);
   B->timeStamp[24] = '\0';

   fclose(fp);
}

//It says if the blockhash has not been modified by performing a SHA256 implementation
int validate(struct block *B){
  int valid = 1;
  //Check PoW.c for more information
  BYTE preInput[300];
  BYTE newInput[300];
  createpreInput(B,preInput);
  strcpy(newInput,preInput);
  BYTE NB[10];
  sprintf(NB,"%Lf",B->nonce);
  strcat(newInput,NB);

  BYTE newHash[SHA256_BLOCK_SIZE];
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, newInput, strlen(newInput));
  sha256_final(&ctx, newHash);

  puts("HashingReaded:");
  for(int i=0; i<SHA256_BLOCK_SIZE;i++)
    printf("%X",newHash[i]);
  puts("");

  //This is not working yet #Help!
  for(int i=0; i<SHA256_BLOCK_SIZE;i++)
    if(newHash[i]!=B->blockHash[i]){
      valid = 0;
      //printf("newHash: %c blockHash: %c valid:%d\n",newHash[i],B->blockHash[i],valid);
    }

  return valid;
}

//It reads and verifies the whole blockchain!
void readBlockchain(struct block *B){
  int ind=1, cond=1;
  while(1){
    //Read a single block and store data in B
    readBlock(B, ind);
    //If B is valid, then continue
    if(validate(B))
      ind++;
    else{
      cond = 0;
      break;
    }
  }
  //#Help! I don't understand how to implement this
  if(cond)
    puts("Blockchain.dat validated");
  else
    puts("Blockchain.dat is invalid");
}