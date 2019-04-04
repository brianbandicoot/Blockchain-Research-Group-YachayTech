#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "sha256.h"


time_t rawtime;
struct tm * timeinfo;

struct transaction
{
  int index;
  char* previousTx1;
  char* previousTx2;
  char* script;
  int amount;
  char* txHash;
};

struct wallet
{
  char* publickey;
  char* privatekey;
};

struct block
{
  int index;
  char *timestamp;
  char *merkleroot;
  char *previousHash;
  int blockHash[SHA256_BLOCK_SIZE+1];
  char *minerAddress;
  long double nonce;
};

struct blockchain
{
  int size;
  char* leadingBlock;
};

void PoW (struct block *, int);
void writeBlock(struct block * );
const char *byteToBinary(int);
