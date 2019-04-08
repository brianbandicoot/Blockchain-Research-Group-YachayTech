
struct transaction
{
  int index;
  char* previousTx1;
  char* previousTx2;
  char* script;
  int amount;
  char* txHash = sha256(previousTx1, previousTx2,
                        script,amount);
}

struct wallet
{
  char* publickey;
  char* privatekey;
}

struct block
{
  int index;
  float timestamp;
  char* merkleroot;
  char* previousHash;
  char* blockHash;
  char* minerAddress;
  long float nonce;
}

struct blockchain
{
  int size;
  char* leadingBlock;
}
