#include<stdio.h>
#include <memory.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
//#include"PoW.h"
#include"variables.h"
#include "sha256.h"

int main(void)
{
  BYTE hash1[SHA256_BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
                                   0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
  struct block B1;
  B1.index = 567;
  B1.merkleroot   = "5FD924625F6AB16A19CC9807C7C506AE1813490E4BA675F843D5A10E0BAACDB8";
  B1.previousHash = "533A8EE27BF4706C947D5DAECD07B5F8F7E4BA50DFF4C4140DD070FF7EBE9196";
  B1.minerAddress = "b3391c07-79be-4b23-93a1-6fb5a36e5cca";
  puts("Starting...");
  PoW(&B1,2);
  printf("Time: %s \n",B1.timestamp);

}
