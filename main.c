/* To compile run the following command on the terminal:
gcc -o test main.c PoW.c sha256.c file.c validate.c -I.
*/

#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <sys/stat.h>
//All variables and global functions are defined in variables.h
#include"variables.h"
//This library is for the SHA256 function implementation
#include "sha256.h"


//It prints all the information of a block (All data that is stored at blockchain.dat)
void printBlock(const struct block * B){
  puts("\nStart Block Printing ----------------------------------------------------------");
  printf("Index: %d \n",B->index);
  printf("MerkleRoot: %s \n",B->merkleRoot);
  printf("PreviousHash: %s \n",B->previousHash);
  //If it was a readed block then this 4 lines will print defined trash
  printf("BlockHash: ");
  for(int i=0; i<SHA256_BLOCK_SIZE; i++)
    printf("%X",B->blockHash[i]);
  puts("");
  printf("ReadedHash: %s\n", B->readedHash);
  printf("MinerAddress: %s \n",B->minerAddress);
  printf("Nonce: %Lf \n",B->nonce);
  printf("TimeStamp: %s \n",B->timeStamp);
  puts("Finish Block Printing ----------------------------------------------------------\n");
}

int main(void){
  //Initialize block counter with zero
  blockNumbers = 0;

  //We declare our first block
  struct block B1;
  //We haven't write a automatic implementation of the block data so we do it manually below
  B1.index = 1;
  copyString(B1.merkleRoot,"6FD924625F6AB16A19CC9807C7C506AE1813490E4BA675F843D5A10E0BAACDB8",SHA256_BLOCK_SIZE*2);
  //copyString(B1.merkleRoot,getMerkleRoot(char transaction[]),SHA256_BLOCK_SIZE*2);
  copyString(B1.previousHash,"533A8EE27BF4706C947D5DAECD07B5F8F7E4BA50DFF4C4140DD070FF7EBE9196",SHA256_BLOCK_SIZE*2);
  copyString(B1.minerAddress,"a3391c07-79be-4b23-93a1-6fb5a36e5cca",37);

  //This is the Proof of Work process which gets the block hash using the block data and the nonce
  puts("Starting...");
  PoW(&B1,2);
  //printBlock(&B1);
  //We store the blocks on in blockchain.dat
  writeBlock(&B1);
  //We saved the timestamp of last modification of blockchain.dat
  char ModificationTime[25];
  time (&rawtime);
  timeinfo = localtime ( &rawtime );
  copyString(ModificationTime,asctime(timeinfo),25);

  //We check and validate data when the blockchain.dat has been modified
  puts("Waiting for file modification");
  //lastValidBlock will store the current block information to be validated
  struct block lastValidBlock;
  while(1){
    //Get the filestatus for modification time
    struct stat filestatus;
    stat("blockchain.dat",&filestatus);
    //If the modification time is equal to file modification time then 0 otherwise 1
    if (strncmp(ctime(&filestatus.st_mtime),ModificationTime,24)){
      puts("The File was modified");
      //start reading and validating the blockchain
      readBlockchain(&lastValidBlock,blockNumbers);
      //set up a new modification time to know that the blockchain was validated
      time (&rawtime);
      timeinfo = localtime ( &rawtime );
      copyString(ModificationTime,asctime(timeinfo),25);
      break;
    }
    //Send the process to sleep. It will be better to having it in a I/O device queue waiting for disk modification #Help!
    //sleep(300); //The number of seconds the computer will go to sleep/wait
  }

  return 0;
}
