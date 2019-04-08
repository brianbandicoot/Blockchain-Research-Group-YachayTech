/* To compile run the following command on the terminal:
gcc -o test main.c PoW.c sha256.c file.c -I.
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
void printBlock(struct block * B){
  printf("Index: %d \n",B->index);
  printf("MerkleRoot: %s \n",B->merkleRoot);
  printf("PreviousHash: %s \n",B->previousHash);
  printf("BlockHash: ");
  for(int i=0; i<(SHA256_BLOCK_SIZE)*2-2; i++)
   printf("%X",B->blockHash[i]); //I am not sure if use %X or %c
  puts("");
  printf("MinerAddress: %s \n",B->minerAddress);
  printf("Nonce: %Lf \n",B->nonce);
  printf("TimeStamp: %s \n",B->timeStamp);
}

int main(void){
  //Time setup for recording the modification time of blockchain.dat each time a new block is saved
  time_t rawtime;
  struct tm * timeinfo;
  char ModificationTime[25];

  //We declare our first block
  struct block B1;
  //We haven't write a automatic implementation of the block data so we do it manually below
  B1.index = 1;
  copyString(B1.merkleRoot,"6FD924625F6AB16A19CC9807C7C506AE1813490E4BA675F843D5A10E0BAACDB8",SHA256_BLOCK_SIZE*2);
  //copyString(B1.merkleRoot,getMerkleRoot(char transaction[]),SHA256_BLOCK_SIZE*2);
  copyString(B1.previousHash,"533A8EE27BF4706C947D5DAECD07B5F8F7E4BA50DFF4C4140DD070FF7EBE9196",SHA256_BLOCK_SIZE*2);
  copyString(B1.minerAddress,"a3391c07-79be-4b23-93a1-6fb5a36e5cca",37);

  //We declare our second block
  struct block B2;
  B2.index = 2;
  copyString(B2.merkleRoot,"6FD924625F6AB16A19CC9807C7C506AE1813490E4BA675F843D5A10E0BAACDB8",SHA256_BLOCK_SIZE*2);
  copyString(B2.previousHash,"533A8EE27BF4706C947D5DAECD07B5F8F7E4BA50DFF4C4140DD070FF7EBE9196",SHA256_BLOCK_SIZE*2);
  copyString(B2.minerAddress,"a3391c07-79be-4b23-93a1-6fb5a36e5cca",37);

  //This is the Proof of Work process which gets the block hash using the block data and the nonce
  puts("Starting...");
  PoW(&B1,2);
  //PoW(&B2,2);
  printBlock(&B1);

  //We store the B1 information in blockchain.dat
  writeBlock(&B1);
  //We store the B2 information in blockchain.dat
  writeBlock(&B2);
  //We saved the timestamp of last modification of blockchain.dat
  time (&rawtime);
  timeinfo = localtime ( &rawtime );
  copyString(ModificationTime,asctime(timeinfo),25);

  //We create a new B3 to store the readed data from blockchain.dat
  struct block B3;
  //We read the blockchain.dat and store the data of block index 1
  readBlock(&B3,1);
  printBlock(&B3);



  /*
  //We check and validate data when the blockchain.dat has been modified
  struct block lastValidBlock;
  while(1){
    struct stat filestatus;
    stat("blockchain.dat",&filestatus);
    if (strncmp(ctime(&filestatus.st_mtime),ModificationTime,24)){
      printf("The File was modified\n");
      readBlockchain(&lastValidBlock);
      time (&rawtime);
      timeinfo = localtime ( &rawtime );
      copyString(ModificationTime,asctime(timeinfo),25);
      break;
    }
    //sleep(300);
  }*/
  return 0;
}
