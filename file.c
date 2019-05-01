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

//It writes the block B into the blockchain.dat
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

   //It reads the first line of the blockchain.dat file
   fp = fopen("blockchain.dat", "a+");
   fscanf(fp,"%s %d",name,&B->index);
   printf("Index:%d\n",B->index);
   fscanf(fp,"%s %s\n",name,B->merkleRoot);
   B->merkleRoot[SHA256_BLOCK_SIZE*2] = '\0';
   fscanf(fp,"%s %s\n",name,B->previousHash);
   B->previousHash[SHA256_BLOCK_SIZE*2] = '\0';
   fscanf(fp,"%s %s\n",name,B->tempHash);
   B->tempHash[(SHA256_BLOCK_SIZE*2)-1] = '\0';
   printf("tempHash %s\n",B->tempHash);

   //A tricky step to get the blockHash as it should be
   //for(int i=0; i<(SHA256_BLOCK_SIZE)*2-2; i++)
     //B->blockHash[i] = tempHash[i];

   printf("blockHash %s\n",B->blockHash);
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
  puts("");
  printf("ReadInput: %s \n",newInput);
  puts("");
  printf("strcmp: %d\n",strcmp(newInput,compInp));

  BYTE newHash[SHA256_BLOCK_SIZE];
  char newHash2[SHA256_BLOCK_SIZE];
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    newHash2[i] = 'Y';
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, newInput, strlen(newInput));
  sha256_final(&ctx, newHash);

  //sprintf(newHash2,"%X",newHash); It is not good
  int j=0;
  for(int i=0; i<(SHA256_BLOCK_SIZE);i++){
    sprintf(&newHash2[j],"%X",newHash[i]);
    j=j+2;
    //printf("%c",newHash2[i]);
  }
  puts("HashingReaded ");
  for(int i=0; i<(SHA256_BLOCK_SIZE*2);i++)
    printf("%c",newHash2[i]);

  puts("");
  for(int i=0; i<(SHA256_BLOCK_SIZE*2);i++){
    printf("nh:%d th:%d \n",newHash2[i],B->tempHash[i]);
  }


  puts("");
  /*for(int i=0; i<(SHA256_BLOCK_SIZE);i++){
    printf("Nico");
    if(newHash[i]==B->blockHash[i]){
      printf("nH2: %X   bH:%X \n",newHash[i],B->blockHash[i]);
      valid = 0;
    }
  }*/

  //printf("%s \n",newHash2);
  puts("");

  valid = strcmp(newHash,B->blockHash);

  //This is not working yet #Help!
  //for(int i=0; i<SHA256_BLOCK_SIZE;i++)
    //if(newHash[i]!=B->blockHash[i]){
      //valid = 0;
      //printf("newHash: %c blockHash: %c valid:%d\n",newHash[i],B->blockHash[i],valid);
    //}

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
