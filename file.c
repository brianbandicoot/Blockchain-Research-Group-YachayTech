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
  //Remember that the last element of a char array has to be NULL to avoid printing the next thing
  destination[size] = '\0';
}

//It writes the block B into the blockchain.dat
void writeBlock(const struct block *B){
   //Opens a blockchain.dat and append the new information
   FILE *fp;
   fp = fopen("blockchain.dat", "a+");
   fprintf(fp, "Block: %d\nMerkleRoot: %s\nPreviousHash: %s",
                B->index,
                B->merkleRoot,
                B->previousHash);
  fprintf(fp,"\nBlockHash: ");
  /*Saving the blockhash is a little bit tricky.
  We are putting a BYTE (un char) into a string in the .dat file.
  Each BYTE is saved in its hexadecimal representation*/
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    fprintf(fp,"%X",B->blockHash[i]);
  fprintf(fp, "\nMinerAddress: %s\nNonce: %Lf",
               B->minerAddress,
               B->nonce);
  fprintf(fp, "\nTimeStamp: %s\n",B->timeStamp);
  fclose(fp);
  blockNumbers++;
}

//It reads a block and saves it into the input block structure
void readBlock(FILE *fp,struct block* B){
   /*To get the data tags ex: "BlockHash", "MinerAddress", etc
   We create a temporal variable to store the tags.*/
   char name[14];

   //puts("start reading");
   fscanf(fp,"%s %d",name,&B->index);
   fscanf(fp,"%s %s\n",name,B->merkleRoot);
   //Remember that the last element of a char array has to be NULL to avoid printing the next thing
   B->merkleRoot[SHA256_BLOCK_SIZE*2] = '\0';
   fscanf(fp,"%s %s\n",name,B->previousHash);
   //Remember that the last element of a char array has to be NULL to avoid printing the next thing
   B->previousHash[SHA256_BLOCK_SIZE*2] = '\0';
   fscanf(fp,"%s %s\n",name,B->readedHash);
   //Remember that the last element of a char array has to be NULL to avoid printing the next thing
   B->readedHash[(SHA256_BLOCK_SIZE*2)-1] = '\0';
   fscanf(fp,"%s %s\n",name,B->minerAddress);
   //Remember that the last element of a char array has to be NULL to avoid printing the next thing
   B->minerAddress[36] = '\0';
   fscanf(fp,"%s %Lf\n",name,&B->nonce);
   fscanf(fp,"%s %s\n",name,B->timeStamp);
   //Remember that the last element of a char array has to be NULL to avoid printing the next thing
   B->timeStamp[24] = '\0';
   //puts("finished reading");

}
