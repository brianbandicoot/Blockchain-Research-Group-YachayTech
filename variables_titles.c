struct title
{
  char[9] date;
  char[256] hash;
  char* data; //descripcion del titulo
  char[100] studentPublicKey;
  char[100] universityPublicKey;
};

struct student
{
  char[100] publicKey;
  char[100] privateKey;

};


struct title searchTitle(int index)
{
  return title;
}
