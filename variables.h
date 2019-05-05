#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "sha256.h"

time_t rawtime;
struct tm * timeinfo;
//Counter to know the number of blocks in the blockchain
int blockNumbers;
//hash corresponding to merkleroot
unsigned char *merkleRoot;

struct transaction{
  int w1[8];
  int key1[3];
  int value;
  int w2[8];
  int key2[3];
  char hash[SHA256_BLOCK_SIZE*2];
};

struct wallet{
  int p;
  int q;
  int k;
  int j;
  int n;
  char wallet[8];
  int ewallet[8];
  int dwallet[8];
};

//The Block structure. The sizes are not ready #Help!
struct block{
  int index;
  char timeStamp[25];
  char merkleRoot[(SHA256_BLOCK_SIZE*2)+1];
  BYTE previousHash[(SHA256_BLOCK_SIZE*2)+1];
  BYTE blockHash[SHA256_BLOCK_SIZE];
  char readedHash[SHA256_BLOCK_SIZE*2];
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
void readBlock(FILE*,struct block *);
void readBlockchain(struct block *, const int);
int validate(const struct block *);

//merkleroot.c
unsigned char *hashTransactions(unsigned char **, int);

//wallet.c
void concatenateTx(const struct transaction *,BYTE *);
void generateWallet(struct wallet *);
void printWallet(struct wallet *);
void generateTx(struct transaction *,struct wallet,struct wallet, long int);
