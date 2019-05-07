//Written by Pedro Altamirano (pedro.altamirano@yachaytech.edu.ec)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "variables.h"
#include "sha256.h"

void concatenateTx(const struct transaction *tx,BYTE *result){
  //The input will be separated by a "/"
  strcpy(result,"w1:");
  char sep[] = "/";
  char str[8];
  for(int i=0; i<8; i++){
    sprintf(str , "%d", tx->w1[i]);
    strcat(result, str);
  }
  strcat(result,sep);
  strcat(result, "value:");
  char val[4];
  sprintf(val,"%d",tx->value);
  strcat(result,val);
  //strcat(result, tx->value);
  strcat(result,sep);
  strcat(result, "w2:");
  for(int i=0; i<8; i++){
    char str[8];
    sprintf(str , "%d", tx->w2[i]);
    strcat(result, str);
  }
}

void generateWallet(struct wallet *w){
  unsigned long long E;
  unsigned long long P;
  for(int i=0; i<8; i++){
    E = pow(((int)w->wallet[i]-48), w->j);
    w->ewallet[i] = E % w->n;
    P = pow(w->ewallet[i], w->k);
    w->dwallet[i] = P % w->n;
  }
}

void printWallet(struct wallet *w){
  printf("Keys:\n");
  printf("p: %d\nq: %d\nj: %d\nk: %d\nn: %d\n",w->p,w->q,w->j,w->k,w->n);
  printf("pub: (%d, %d), priv: %d\n",w->n,w->k,w->j);
  printf("wallet: %s \n", w->wallet);
  puts("Ewallet: ");
  for(int x=0; x<8; x++) printf("%d", w->ewallet[x]);
  puts("\nDwallet: ");
  for(int x=0; x<8; x++) printf("%d", w->dwallet[x]);
  printf("\n");
}

void generateTx(struct transaction *tx,struct wallet w1,struct wallet w2, long int value){
  for(int i=0; i<8; i++){
    tx->w1[i] = w1.ewallet[i];
    tx->w2[i] = w2.ewallet[i];
  }
  tx->key1[0] = w1.n;
  tx->key1[1] = w1.k;
  tx->key1[2] = w1.j;
  tx->value = value;
  tx->key2[0] = w2.n;
  tx->key2[1] = w2.k;
  tx->key2[2] = w2.j;
}
