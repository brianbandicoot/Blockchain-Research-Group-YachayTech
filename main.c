/* To compile run the following command on the terminal:
gcc -fno-stack-protector -o test main.c PoW.c sha256.c file.c validate.c merkleroot.c wallet.c -lm -I.
*/

/*If you will run the executable to add another transaction to an existing blockchain.dat then you need to modify:
  1. blockNumbers: how many blocks are already in blockchain.dat
  2. B.index: what number of block are you mining
  3. TransactionPool: what transactions are you adding
  4. MinerAddress: where is the reward going */

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
  puts("\nStart Block Printing ----------------------------------------------------------\n");
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
  puts("\nFinish Block Printing ----------------------------------------------------------\n");
}

int main(void){
  //Initialize block counter with zero
  blockNumbers = 0;

  //We declare our block
  struct block B1;
  B1.index = 2;

  //Users wallet----------------------------------------------------
  struct wallet Alice;
  Alice.p=3, Alice.q=7, Alice.k=11, Alice.j=5, Alice.n=21;
  strcpy(Alice.wallet,"19901234");
  generateWallet(&Alice);

  struct wallet Bob;
  Bob.p=11, Bob.q=3, Bob.k=7, Bob.j=3, Bob.n=33;
  strcpy(Bob.wallet,"19914321");
  generateWallet(&Bob);
  //Users wallet--------------------------------------------------------

  //Generating transactions---------------------------------------------
  puts("Starting transactions----------------------------------------------------------\n");
  struct transaction tx1;
  char newTx1[60];
  generateTx(&tx1,Alice,Bob,19); //Change the ammount of money to be send
  concatenateTx(&tx1,newTx1);

  struct transaction tx2;
  char newTx2[60];
  generateTx(&tx1,Alice,Bob,5);
  concatenateTx(&tx2,newTx2);

  struct transaction tx3;
  char newTx3[60];
  generateTx(&tx3,Bob,Bob,10);
  concatenateTx(&tx3,newTx3);

  struct transaction tx4;
  char newTx4[60];
  generateTx(&tx4,Bob,Alice,3);
  concatenateTx(&tx4,newTx4);
  //Generating transactions---------------------------------------------

  //Get all pending transactions
  //We need to implement an append to the transactionPool to avoid creating too many transactions
	BYTE *transactionPool[] = {newTx1,newTx2,newTx3,newTx4};
	int n=sizeof(transactionPool)/sizeof(*transactionPool);

  puts("Starting merkleRoot----------------------------------------------------------\n");
	hashTransactions(transactionPool,n);

  //In order to store the merkleRoot in the block structure we need to do a little trick
  char calculatedMerkle[SHA256_BLOCK_SIZE*2];
  for(int i=0,j=0,k=0; i<(SHA256_BLOCK_SIZE*2);i++){
    /*merkleRoot[i] in %X will print something like 3A. This output has two elements: 3 & A.
    That is why we need to spend the char array pointer calculatedMerkle[j] in a even sequence.
    calculatedMerkle[i] will grab the first element while calculatedMerkle[i+1] will grab the second*/
    sprintf(&calculatedMerkle[j],"%X",merkleRoot[i]);
    j=j+2;
    /*Some elements will not be ASCII readable characters (so they will output 0).
    Imagine that merkleRoot[i] = 5. Then calculatedMerkle[i] = ? and calculatedMerkle[i+1] = 5*/
    if(calculatedMerkle[i] !=0){
      //Save each element of calculatedMerkle in the block structure
      B1.merkleRoot[k] = calculatedMerkle[i];
      //The readedHash pointer will move only if calculatedMerkle[i] is different than zero to avoid an array translation with merkleroot.
      k++;
    }
  }
  B1.merkleRoot[(SHA256_BLOCK_SIZE*2)-1]='\0';

  //Get the Genesis Block to have the previousHash
  struct block previousBlock;
  readBlockchain(&previousBlock,B1.index-1); //Change this value if it is not the second block you are adding
  copyString(B1.previousHash,previousBlock.readedHash,SHA256_BLOCK_SIZE*2);
  copyString(B1.minerAddress,"a3391c07-79be-4b23-93a1-6fb5a36e5cca",37);

  //This is the Proof of Work process which gets the block hash using the block data and the nonce
  puts("\nStarting PoW-----------------------------------------------------------------\n");
  PoW(&B1,1); //Change difficulty if blocks where created in less than 10 minutes
  printBlock(&B1);
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
      readBlockchain(&lastValidBlock,B1.index);
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
