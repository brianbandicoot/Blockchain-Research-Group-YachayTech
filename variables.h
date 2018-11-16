
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
  char *blockHash;
  char *minerAddress;
  int nonce;
};

struct blockchain
{
  int size;
  char* leadingBlock;
};

void PoW (struct block *, int);
