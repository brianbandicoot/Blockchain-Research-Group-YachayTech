#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"variables.h"
#include "sha256.h"

int validate(struct block* B){
  int answer = 0;
  validateMerkle(B.blockHash);
  validateHash(B.merkleRoot);
  return answer;
}

int validateHash(BYTE* BHash){
  int answer = 0;
  createpreInput();
  BYTE hash[SHA256_BLOCK_SIZE];
  sha256_init();
  sha256_update();
  sha256_final();
  if(hash == BHash)
    answer = 1;
  return answer;
}
