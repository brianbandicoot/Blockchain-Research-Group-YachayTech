//Written by Andres Banda (andres.banda@yachaytech.edu.ec)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//All variables and global functions are defined in variables.h
#include"variables.h"
//This library is for the SHA256 function implementation
#include "sha256.h"


// This function takes Hashed transactions and pair them in order to construct the tree
unsigned char *Merkleroot(unsigned char *TranHash[], int size){

	//If the number of Transactions is odd we need to duplicate the last transaction. Then, we create a pointers array according to this condition. This time we will store in a the hash of the concatenated transactions.
	//TODO: Implement the condition when odd number of transactions is provided.
	unsigned char *a[size/2];

	//cadena stores the concatenation
	unsigned char *cadena;
	int TransactionHashLen = SHA256_BLOCK_SIZE * 2 + 1;
	cadena = malloc(TransactionHashLen);

	//newHash and ctx are variables required to use Sha256 Hash
	BYTE newHash[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;

	for(int i=0;i<size;i+=2){
		// --- Begin concatenation process ---
		for(int j = 0; j<SHA256_BLOCK_SIZE; j++){
			cadena[j]=TranHash[i][j];
			cadena[j+SHA256_BLOCK_SIZE]=TranHash[i+1][j];
		}
		// --- End concatenation process ---

		/*//-- printf to test --//
		printf("\nConcatenation:");
		for(int i = 0; i<SHA256_BLOCK_SIZE*2; i++)
			printf("%X", cadena[i]);
		//-- end printf to test --//*/

		//Hash the concatenated transactions
		sha256_init(&ctx);
		BYTE HashInput[SHA256_BLOCK_SIZE*2+1];
		strcpy(HashInput,cadena);
		sha256_update(&ctx, HashInput, strlen(HashInput));
		sha256_final(&ctx, newHash);

		/*//-- printf to test --//
		printf("\nConcatenation Hash: \t");
		for(int i = 0; i<SHA256_BLOCK_SIZE; i++)
			printf("%X", newHash[i]);
		//-- end printf to test --//*/

		//Hash of the transaction pair is placed in the array a
		a[i/2] = malloc(SHA256_BLOCK_SIZE);
		for(int j = 0; j<SHA256_BLOCK_SIZE+1; j++)
			a[i/2][j]=newHash[j];

	}
	//If we have more than two nodes, we continue creating parents(concatenated hash) recursively.
	if(size/2>1)
		Merkleroot(a, size/2);

	//The base case is when the last two hashes are concatenated
	else{
		merkleRoot=malloc(SHA256_BLOCK_SIZE);
		for(int i = 0; i<SHA256_BLOCK_SIZE; i++){
			merkleRoot[i]=a[0][i];}
		return merkleRoot;
	}
}

//This function hash the transactions and pass them to the Merkleroot function
unsigned char *hashTransactions(unsigned char *TransactionPool[], int size){
	//newHash and ctx are variables required to use SHA256
	BYTE newHash[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	//Hashed transactions will be stored in a
	unsigned char *a[size];

  for(int i=0; i < size; i++){
		//--- Hash sha256 process -- //
		sha256_init(&ctx);
		BYTE hashInput[strlen(TransactionPool[i])];
		strcpy(hashInput,TransactionPool[i]);
		sha256_update(&ctx, hashInput, strlen(hashInput));
		sha256_final(&ctx, newHash);
		//--- End Hash process --//

		/*//--printf to test--//
		printf("\nTransaction %d: %s \n", i+1, TransactionPool[i]);
		printf("Hash: \t");
		for(int i = 0; i<SHA256_BLOCK_SIZE; i++){
			printf("%X", newHash[i]);}
		//--end printf to test--//*/

		a[i] = malloc(SHA256_BLOCK_SIZE);
		for(int j = 0; j<SHA256_BLOCK_SIZE; j++)
			a[i][j]=newHash[j];
  }
	Merkleroot(a, size);
}
