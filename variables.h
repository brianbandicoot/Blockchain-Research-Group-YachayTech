#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "sha256.h"

time_t rawtime;
struct tm * timeinfo;
char compInp[1000];

//This is not ready yet
struct transaction{
  int index;
  char* previousTx1;
  char* previousTx2;
  char* script;
  int amount;
  char* txHash;
};

//This is not ready yet
struct wallet{
  char* publickey;
  char* privatekey;
};

//The Block structure. The sizes are not ready #Help!
struct block{
  int index;
  char timeStamp[25];
  char merkleRoot[(SHA256_BLOCK_SIZE*2)+1];
  BYTE previousHash[(SHA256_BLOCK_SIZE*2)+1];
  //if char instead of BYTE then smack error!
  BYTE blockHash[(SHA256_BLOCK_SIZE*2)-2];
  char minerAddress[37];
  long double nonce;
};

//This is not ready yet
struct blockchain{
  int size;
  char* leadingBlock;
};


/*I just defined all functions that are used in different files.
Should we put it on different header files (.h)? */
//PoW.c
void printfHash(const BYTE*);
const char *byteToBinary(int);
void PoW (struct block *, int);
void createpreInput(const struct block *,char *);

//file.c
void copyString(char*,const char*,const int);
void writeBlock(const struct block *);
void readBlock(struct block *, const int);
void readBlockchain(struct block *);
int validate(struct block *);
