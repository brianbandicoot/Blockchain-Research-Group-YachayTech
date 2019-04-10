# Blockchain-Research-Group-YachayTech

We are a undergraduate student group focused on Blockchain technology.
To learn about the core features we are trying to build our own blockchain
from strach, replicating the first Bitcoin Blokchain concept.

To read our code:
1. Start in main.c, look at the functions and how the blocks are declared
2. You might need to visit variables.h to learn about the struct block definition
3. When you reach PoW(&B), look at the PoW.c code to understand its internal functioning
4. When you reach write(&B) or read(&B), look at the file.c code to understand how are we saving the block data
 (Posdata: Remember that the code is INCOMPLETE, sorry for any trouble or misunderstanding)

To try our code:

1. Download the files
2. Unzip the files in one folder
3. Run this in the command line: gcc -o test main.c PoW.c sha256.c file.c -I.
4. Run this in the command line: ./test
5. See how it goes :)

If you have any suggestions to our code or you want to collaborate with us please
do not hesitate in writing a comment or contact us to nicolas.serrano@yachaytech.edu.ec
