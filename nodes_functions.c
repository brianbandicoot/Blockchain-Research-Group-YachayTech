//This file is not ready yet
struct* Sendbitcoin(struct transaction preTx1,
                 struct transaction preTx2,
                 char* receiverAddress)
{
  struct transaction *newTx;
  newTx = malloc(struct transaction);

  if (publickey(preTx1.script) == publickey(prevTx2.script))
  {
    *newTx.previousTx1 = preTx1.txHash;
    *newTx.previousTx2 = preTx2.txHash;
    *newTx.amount = preTx1.cant + preTx2.cant;
    *newTx.script = ownship(receiverAddress,receiver.amount)
    txToPool(newTx);
  }

  else
    txToOrphan(newTx);
  return newTx;
}
