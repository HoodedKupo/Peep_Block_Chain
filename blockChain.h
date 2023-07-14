/*------------------------------------------------------
 * Student's Name: Nathan Koop, Keyanna Levie
 * Assignment #3
 * Lab Section: X03L, Winter 2021
 * CMPUT 201, Winter 2021
 * Lab Instructor's Name: Antal Buss
*------------------------------------------------------*/

#define MXTXNUM 64
#define PEEPLEN 256
#include <stdint.h>
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

/*Structure to Represent a modification [2 bytes]. (modification)
	position	- Position in the peep that the event occured: Maximum position of 256
	character	- The character that was modified  - ASCII codes 32 to 127
     event_type	- Type of event that has occured.
				  0 for Insert
				  1 for Delete*/
struct Modification{
	unsigned int position : 8;
	unsigned int character : 7;
	unsigned int event_type : 1;
}__attribute__((packed));

/*Structure to Represent a Transaction [8 bytes] (transaction) 
	timestamp	- Time that the event occurred (EPOCH time since January 1-st 2020). [6 bytes]
	modification	- The modification part of the transaction.                          [2 bytes]*/
struct Transaction;
struct Transaction{
	uint64_t timestamp : 48;/*6 bytes*/
	struct Modification modification;
}__attribute__((packed));

/*  structure to represent the header of the block.[25 bytes]
    previousHash	- Hash of the Previous block head. 0 if first          [7 bytes]
	timeStamp		- Time that the hashing of the block occured           [6 bytes]
	blockNum		- Incremental number of the block.                     [4 bytes]
	blockSize		- Size of transaction elements in the block            [1 byte]
	blockHash		- The Merkle tree hash of the transactions             [7 bytes]*/
struct Block_Header{
	char previousHash[7];
	uint64_t timeStamp: 48;//
	int blockNum;
	int blockSize: 8;//
	char blockHash[7];//
}__attribute__((packed));


/*Structure to Represent a Block  [25 bytes for the  head + up to 8*64 bytes for transactions] (block)
	
	Header          - The header of the block [25 bytes]                   
                                
	headHash		- Hash of the head elements above                      [7 bytes]
      
	transactions	- Transactions contained in the block (maximum of 64 transactions)

NOTE: a hash of length 7 bytes is obtained by taking the last 7 bytes of the sha256 hash 
      The truncation is to be applied only as the final step. 
      All intermediary hashes are full sha256 hashes

*/
struct block{
	/* Head*/
	struct Block_Header Header;

	char headHash[7];
	struct Transaction * transactions;//
};

/* CompareStrings(char * s1, char * s2, int size)
 * checks to see if two arrays of characters are equal.
 * meaning that all characters in the array are the same.
 * Parameters
 *    s1 - an array of characters
 *    s2 - an array of characters
 *    size - the size of both arrays
 * Pre-Condtions:
 *    both s1 and s2 are valid character pointers of size, size.
 * returns:
 *    1 if s1 == s2
 *    0 if s1 != s2*/
int CompareStrings(char * s1, char * s2, int size);

/* SetStrings(char * s1, char * s2, int size)
 * sets the character array in s1 equal to the character array in s2.
 * Parameters:
 *    s1 - an array of characters
 *    s2 - an array of characters
 *    size - size of both arrays
 * Pre-Condtions:
 *    both s1 and s2 are valid character pointers of size, size.
 * Post-Condtiion:
 *    s1 is equivilent to s2 but they do not point to the same array*/
void SetStrings(char * s1, char * s2, int size);


void printTransactions(struct Transaction *trans_list, int size);

/* HashMessage(char * Message, int MessageLength, unsigned int * BytesWritten)
 * Hashes message and returns the hased message. It also stores the bytes written to BytesWritten.
 * Parameters:
 *    Message - array of characters representing the message to hash
 *    MessageLength - the length of the array Message
 *    BytesWritten - the number of bytes written to the hash array.
 * returns:
 *    character array representing the hash of the message
 * Pre-Condtions:
 *    Message and BytesWritten are valid initialized pointers.
 *    MessageLength represents the exact length of Message
 * Post-Conditions:
 *    BytesWritten now contains the number of bytes in the hashed message that was returned.*/
char * HashMessage(char * Message, int MessageLength, unsigned int * BytesWritten);

/* MerkelHash(char ** hashes, int number_hashes, unsigned int BytesWritten)
 * Hashes the array of hashes in the merkelHash format.
 * Parameters:
 *    hashes - array of the hashes of each transaction
 *    number_hashes - the number of hashes in the hashes array
 *    BytesWritten - the number of bytes that each hash contains.
 * Pre-Conditions:
 *    all pointers are set up as specified and number_hashes is the exact size of hashes.
 * Post-Condtions:
 *    hashes no longer represents hashes of all transactions. Instead hashes[0] is the final result of the MerkelHash
 * Returns:
 *    returns the merkel hash of all transactions.*/
char * MerkelHash(char ** hashes, int number_hashes, unsigned int BytesWritten);

/* HashTransactions(struct Transaction * transactions, int transactions_size)
 * hashes the list of transactions acording to the merkel root tree algorithm.
 * Parameters:
 *    transactions - a list of transactions
 *    transaction_size - the number of elements in transactions
 * Pre-conditions:
 *    transaction_size is the exact number of elements in transactions.
 * Returns:
 *    returns the 7 byte hash of all transactions*/
char * HashTransactions(struct Transaction * transactions, int transactions_size);

/* ConcatenateHashes(char ** hashes, int Number_Hashes, int Size_of_Hash)
 * concatenates all hashes in hashes into one larger character array.
 * Parameters:
 *    hashes - a list of hashes.
 *    Number_Hashes - the size of the hashes array
 *    Size_of_Hash - the size of each element in the hashes array.
 * Pre-Condtions:
 *    Number_Hashes and Size_of_Hash acuratly represent what they are suppose to
 *    hashes is a valid double array
 * Returns:
 *    an array of characters of length Number_Hashes*Size_of_Hash which contains all hashes concatenated together.*/
char * ConcatenateHashes(char ** hashes, int Number_Hashes, int Size_of_Hash);

/* HashHead(struct Block_Header currentBlock)
 * hashes each individual component of the header, concatenates them, then hashes that string 
 * before returning the last 7 bytes of the hash to represent the HeadHash.
 * Parameters:
 *    currentBlock - the header that is going to be hashed
 * Returns:
 *    the 7 byte hash for the header of the block.*/
char * HashHead(struct Block_Header currentBlock);

/* verifyBlockChain(char * FileName)
 * iterates through the file and checks if all hashes are correct by re-hashing all hashes and checks if they
 * are the same hashes in the file
 * Parameters:
 *    FileName - the name of the file containing the block chain you wish to verify
 * Returns:
 *    1 if the block chain is valid
 *    0 if the block chain is invalid.*/
int verifyBlockChain(char * FileName);

/* GetNumberTransactions(char * FileName)
 * counts the total number of transactions in all blocks in the block chain contained in file.
 * Parameters:
 *    FileName - name of the file you wish to get the number of transactions of
 * Returns:
 *    the number of transactions in all blocks in the block chain.*/
int GetNumberTransactions(char * FileName);

/* readBlockChain(char * FileName, int * number_transactions)
 * reads through the block chain and returns a list of transactions in all blocks.
 * Parameters:
 *    FileName - the file you wish to read from.
 *    number_transactions - the integer that will contain the number of transactions after the function concludes.
 * Pre-Conditions:
 *    number_transactions is a valid initialized integer pointer
 * Post-Conditions:
 *    number_transactions contains the number of transactions in the returned transaction list.
 * Returns:
 *    returns a list of all transactions in all blocks in the block chain.*/
struct Transaction * readBlockChain(char * FileName, int * number_transactions);

/* GetDigits(uint64_t i)
 * returns the number of digits in i.
 * example GetDigits(152) returns 3.
 * Parameter:
 *    i - number you want to get the number of digits from.
 * Returns:
 *    returns the number of digits in i.*/
int GetDigits(uint64_t i);

/* WriteBlockChain(char * FileName, struct block NewBlock)
 * writes NewBlock to the end of the block chain inside of File.
 * Parameters:
 *    FileName - name of the file that contains the block chain you wish to concatenate NewBlock to.
 *    NewBlock - the block that is to be concatinated to the end of the block chain in file.*/
void WriteBlockChain(char * FileName, struct block NewBlock);

/* getPeep(struct Transaction * transactions, int number_transactions)
 * reconstructs the peep based on the list of transactions provided.
 * Parameters:
 *    transactions - list of transactions needed to reform the peep
 *    number_transactions - the number of elements in transactions
 * Pre-Condtions:
 *    all transactions are valid. number_transactions is the exact number of elements in transactions.
 * Returns:
 *    the list of characters representing the peep.*/
char * getPeep(struct Transaction * transactions, int number_transactions);

/* getPeepAtTime(struct Transaction * transactions, int number_transactions, uint64_t date)
 * reconstrutcs the peep as it was at a certain time.
 * Parameters:
 *    transactions - the list of transactions
 *    number_transaction - the number of elements in transactions
 *    date - the time you want to build the peep to.*/
char * getPeepAtTime(struct Transaction * transactions, int number_transactions, uint64_t date);

#endif