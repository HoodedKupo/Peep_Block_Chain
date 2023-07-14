/*------------------------------------------------------
 * Student's Name: Nathan Koop, Keyanna Levie
 * Assignment #3
 * Lab Section: X03L, Winter 2021
 * CMPUT 201, Winter 2021
 * Lab Instructor's Name: Antal Buss
*------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#include "peepEditor.h"
#include "blockChain.h"

 
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
int CompareStrings(char * s1, char * s2, int size){
    int i = 0;
    for(i = 0; i< size; i++){
        if(s1[i] != s2[i]){
            return 0;
        }
    }
    return 1;
}

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
void SetStrings(char * s1, char * s2, int size){
    int i = 0;
    for(i = 0; i<size; i++){
        s1[i] = s2[i];
    }
}

void printTransactions(struct Transaction *trans_list, int size){
	int i, chara, type;
	//size = GetNumberTransactions(FileName);
	

	printf("Printing Transactions...\n");
	for(i=0; i <= size; ++i){
		printf("***************************\n");
		printf("Time (in sec from Jan 1, 2020): %d \n",trans_list[i].timestamp);
		/*determines if it is a deletion or insertion event*/
		if((trans_list[i].modification.event_type) == 1)
			printf("Event Type: delete\n");
		if((trans_list[i].modification.event_type) == 0)
			printf("Event Type: Insert\n");
		printf("Position: %d\n",trans_list[i].modification.position);
		/*changes ASCII value into character*/
		chara = (char)trans_list[i].modification.character;
		printf("Character Modified: %c \n", chara);
		printf("***************************\n");
	}
	
}

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
char * HashMessage(char * Message, int MessageLength, unsigned int * BytesWritten){
    char *result;
    result = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));//allocate the memory needed for the result
    EVP_MD_CTX * mdctx;
    mdctx = EVP_MD_CTX_create();//initialize the pointer
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);//Initialize mdctx to have the right value to be used in update
    EVP_DigestUpdate(mdctx, Message, MessageLength);//preform the encryption
    EVP_DigestFinal_ex(mdctx, result, BytesWritten); // stores the result of encryption to result
    EVP_MD_CTX_destroy(mdctx);
    return result;
}

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
char * MerkelHash(char ** hashes, int number_hashes, unsigned int BytesWritten){
    int NumberOfElements = number_hashes;
    int i = 0;
    int TempCounter = 0;
    while(NumberOfElements > 1){
        TempCounter = 0;
        i = 0;
        for(i = 0; i<NumberOfElements;i+=2){
            if(i+1 < NumberOfElements){ // Concatenate both hashes then hash
                char * con;
                con = malloc(2*BytesWritten);
                int j = 0;
                for(j = 0; j<BytesWritten; j++){
                    con[j] = hashes[i][j];
                }
                for(j = 0; j<BytesWritten; j++){
                    con[j+BytesWritten] = hashes[i+1][j];
                }
                char * Hash;
                Hash = HashMessage(con, 2*BytesWritten, &BytesWritten);
                for(j = 0; j<BytesWritten; j++){
                    hashes[TempCounter][j] = Hash[j];
                }
                TempCounter++;
                free(con);
            }else{// dublicate hash and hash them together
                char * con;
                con = malloc(2*BytesWritten);
                int j = 0,k = 0;
                for(k = 0; k<2; k++){
                    for(j = 0; j<BytesWritten; j++){
                        con[(BytesWritten*k)+j] = hashes[i][j];
                    }
                }
                char * Hash;
                Hash = HashMessage(con, 2*BytesWritten, &BytesWritten);
                for(j = 0; j<BytesWritten;j++){
                    hashes[TempCounter][j] = Hash[j];
                }
                TempCounter++;
                free(con);
            }
        }
        NumberOfElements = TempCounter;
    }
    return hashes[0];
}

/* HashTransactions(struct Transaction * transactions, int transactions_size)
 * hashes the list of transactions acording to the merkel root tree algorithm.
 * Parameters:
 *    transactions - a list of transactions
 *    transaction_size - the number of elements in transactions
 * Pre-conditions:
 *    transaction_size is the exact number of elements in transactions.
 * Returns:
 *    returns the 7 byte hash of all transactions*/
char * HashTransactions(struct Transaction * transactions, int transactions_size){
    int i = 0;
    unsigned int BytesWritten;
    char ** hashes;
    //initialize the double array
    
    hashes = malloc(sizeof(char*)*transactions_size);
    for(i = 0; i<transactions_size; i++){
        hashes[i] = malloc(32);
    }
    

    //hash all transactions into double array
    for(i = 0; i<transactions_size; i++){
        //Hash Modification
        char * Sposition, *Sevent_type;
        Sposition = malloc(GetDigits(transactions[i].modification.position)+1);
        sprintf(Sposition, "%d",transactions[i].modification.position);
        Sevent_type = malloc(GetDigits(transactions[i].modification.event_type)+1);
        sprintf(Sevent_type, "%d",transactions[i].modification.event_type);
        char *Hashposition, *Hashcharacter, *Hashevent, *HashModification;
        char *ConcatenatedHashes;
        Hashposition = HashMessage(Sposition, GetDigits(transactions[i].modification.position)+1, &BytesWritten);
        char character = transactions[i].modification.character;
        Hashcharacter = HashMessage(&character, 1, &BytesWritten);
        Hashevent = HashMessage(Sevent_type, GetDigits(transactions[i].modification.event_type)+1, &BytesWritten);
        ConcatenatedHashes = malloc(BytesWritten*3);
        int j = 0;
        for(j = 0; j < BytesWritten; j++){
            ConcatenatedHashes[j] = Hashposition[j];
        }
        for(j = 0; j<BytesWritten; j++){
            ConcatenatedHashes[BytesWritten + j] = Hashcharacter[j];
        }
        for(j = 0; j<BytesWritten; j++){
            ConcatenatedHashes[(2*BytesWritten)+j] = Hashevent[j];
        }
        HashModification = HashMessage(ConcatenatedHashes, 3*BytesWritten, &BytesWritten);
        free(ConcatenatedHashes);

        //Hash timestamp
        char * StimeStamp, *HashtimeStamp, *transactionHash;
        StimeStamp = malloc(GetDigits(transactions[i].timestamp)+1);
        sprintf(StimeStamp, "%d",transactions[i].timestamp);
        HashtimeStamp = HashMessage(StimeStamp, GetDigits(transactions[i].timestamp)+1, &BytesWritten);

        //Hash the modification and the timestamp together
        ConcatenatedHashes = malloc(BytesWritten * 2);
        for(j = 0; j<BytesWritten; j++){
            ConcatenatedHashes[j] = HashModification[j];
        }
        for(j = 0; j<BytesWritten; j++){
            ConcatenatedHashes[BytesWritten + j] = HashtimeStamp[j];
        }
        transactionHash = HashMessage(ConcatenatedHashes, BytesWritten*2, &BytesWritten);
        hashes[i] = transactionHash;
        free(ConcatenatedHashes);
        free(Sposition);
        free(StimeStamp);
        free(Sevent_type);
    }
    //call MerkelHash
    char * result;
    char * returnResult;
    returnResult = malloc(7);
    result = MerkelHash(hashes, transactions_size, 32);
    for(i = 0; i<7; i++){
        returnResult[i] = result[32-(7-i)];
    }
    for(i = 0; i<transactions_size; i++){
        free(hashes[i]);
    }
    free(hashes);
    return result;
}

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
char * ConcatenateHashes(char ** hashes, int Number_Hashes, int Size_of_Hash){
    int i = 0, j = 0;
    char * result;
    result = malloc(Number_Hashes * Size_of_Hash);
    for(i = 0; i<Number_Hashes; i++){
        for(j = 0; j<Size_of_Hash; j++){
            result[j+(Size_of_Hash*i)] = hashes[i][j];
        }
    }
    return result;
}

/* HashHead(struct Block_Header currentBlock)
 * hashes each individual component of the header, concatenates them, then hashes that string 
 * before returning the last 7 bytes of the hash to represent the HeadHash.
 * Parameters:
 *    currentBlock - the header that is going to be hashed
 * Returns:
 *    the 7 byte hash for the header of the block.*/
char * HashHead(struct Block_Header currentBlock){
    char t[7];
    char *StimeStamp, *SblockNum, *SblockSize;
    char *HashpreviousHash, *HashtimeStamp, *HashblockNum, *HashblockSize, *HashblockHash;
    unsigned int BytesWritten;
    StimeStamp = malloc(GetDigits(currentBlock.timeStamp)+1);
    SblockNum = malloc(GetDigits(currentBlock.blockNum)+1);
    SblockSize = malloc(GetDigits(currentBlock.blockSize)+1);
    sprintf(StimeStamp, "%d",currentBlock.timeStamp);
    sprintf(SblockNum, "%d",currentBlock.blockNum);
    sprintf(SblockSize, "%d",currentBlock.blockSize);
    HashpreviousHash = HashMessage(currentBlock.previousHash, 7, &BytesWritten);
    HashtimeStamp = HashMessage(StimeStamp,GetDigits(currentBlock.timeStamp)+1, &BytesWritten);
    HashblockNum = HashMessage(SblockNum, GetDigits(currentBlock.blockNum)+1, &BytesWritten);
    HashblockSize = HashMessage(SblockSize, GetDigits(currentBlock.blockSize)+1, &BytesWritten);
    HashblockHash = HashMessage(currentBlock.blockHash, 7, &BytesWritten);
    char ** HashArray;
    char * ConcatenatedHashes, *result, *HashedCon;
    HashArray = malloc(sizeof(char *) *5);
    HashArray[0] = HashpreviousHash;
    HashArray[1] = HashtimeStamp;
    HashArray[2] = HashblockNum;
    HashArray[3] = HashblockSize;
    HashArray[4] = HashblockHash;
    ConcatenatedHashes = ConcatenateHashes(HashArray, 5, BytesWritten);
    HashedCon = HashMessage(ConcatenatedHashes, BytesWritten*5, &BytesWritten);

    int i = 0;
    for(i = 0; i<5; i++){
        free(HashArray[i]);
    }
    free(HashArray);
    result = malloc(7);
    for(i = 0; i<7; i++){
        result[i] = HashedCon[BytesWritten-(7-i)];
    }
    return result;
}

/* verifyBlockChain(char * FileName)
 * iterates through the file and checks if all hashes are correct by re-hashing all hashes and checks if they
 * are the same hashes in the file
 * Parameters:
 *    FileName - the name of the file containing the block chain you wish to verify
 * Returns:
 *    1 if the block chain is valid
 *    0 if the block chain is invalid.*/
int verifyBlockChain(char * FileName){
    FILE * fptr;
    fptr = fopen(FileName, "rb");
    if(fptr == NULL){
        printf("invalid file name");
        return 0;
    }
    struct Block_Header Header;
	char headHash[7];

	struct Transaction * transactions;//
    int position = 0;
    unsigned int BytesRead;
    char PreviousHash[7];
    while(1){
        BytesRead = fread(&Header, 25, 1, fptr);// read in the header
        if(BytesRead != 25){
            break;
        }
        fread(headHash, 1, 7, fptr);
        transactions = malloc(Header.blockSize * sizeof(struct Transaction));
        fread(transactions, sizeof(struct Transaction), Header.blockSize, fptr);
        
        //Check previous hash validity
        if(position == 0){
            if(Header.previousHash != "0"){
                return 0;
            }
        }else{
            if(PreviousHash != Header.previousHash){
                return 0;
            }
        }
        SetStrings(PreviousHash, Header.previousHash, 7);

        //Check blockHash Validity
        char* currentBlockHash;
        currentBlockHash = HashTransactions(transactions, Header.blockSize);
        if(currentBlockHash != Header.blockHash){
            return 0;
        }

        //Check headHash Validity
        currentBlockHash = HashHead(Header);
        if(currentBlockHash != headHash){
            return 0;
        }
        position++;
    }
    return 1;
}


/* GetNumberTransactions(char * FileName)
 * counts the total number of transactions in all blocks in the block chain contained in file.
 * Parameters:
 *    FileName - name of the file you wish to get the number of transactions of
 * Returns:
 *    the number of transactions in all blocks in the block chain.*/
int GetNumberTransactions(char * FileName){
    FILE * fptr;
    fptr = fopen(FileName, "rb");
    if(fptr == NULL){
        return 0;
    }
    int BytesRead = 0, numberTransactions = 0;

    struct Block_Header Header;
	char headHash[7];

    while (1){
        BytesRead = fread(&Header, 1, 25, fptr);
        if(BytesRead != 25){
            break;
        }
        fread(headHash, 1, 7, fptr);
        struct Transaction * transactions;
        transactions = malloc(sizeof(struct Transaction) * Header.blockSize);
        numberTransactions += Header.blockSize;
        fread(transactions, sizeof(struct Transaction), Header.blockSize, fptr);
        free(transactions);
    }
    fclose(fptr);
    return numberTransactions;
}

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
struct Transaction * readBlockChain(char * FileName, int * number_transactions){
    FILE * fptr;
    int size = GetNumberTransactions(FileName);
    *number_transactions = size;
    int BytesRead = 0, currentTransactionNum = 0;
    fptr = fopen(FileName, "rb");
    struct Transaction * resultTransactions;
    resultTransactions = malloc(size * sizeof(struct Transaction));
    if(fptr == NULL){
        return resultTransactions;
    }
    struct Block_Header Header;

	char headHash[7];
    
    while (1){
        BytesRead = fread(&Header, 25, 1, fptr);
        if(BytesRead != 25){
            break;
        }
        fread(headHash, 1, 7, fptr);
        struct Transaction * transactions;
        transactions = malloc(sizeof(struct Transaction) * Header.blockSize);
        fread(transactions, sizeof(struct Transaction), Header.blockSize, fptr);
        int i = 0;
        for(i = 0; i< Header.blockSize; i++){
            resultTransactions[currentTransactionNum] = transactions[i];
            currentTransactionNum++;
        }
        free(transactions);
    }
    return resultTransactions;
}

/* GetDigits(uint64_t i)
 * returns the number of digits in i.
 * example GetDigits(152) returns 3.
 * Parameter:
 *    i - number you want to get the number of digits from.
 * Returns:
 *    returns the number of digits in i.*/
int GetDigits(uint64_t i){
    int NumDigits = 0;
    if(i< 0){
        i = i*-1;
    }
    while(i > 0){
        NumDigits++;
        i = (i - i%10)/10;
    }
    return NumDigits;
}

/* WriteBlockChain(char * FileName, struct block NewBlock)
 * writes NewBlock to the end of the block chain inside of File.
 * Parameters:
 *    FileName - name of the file that contains the block chain you wish to concatenate NewBlock to.
 *    NewBlock - the block that is to be concatinated to the end of the block chain in file.*/
void WriteBlockChain(char * FileName, struct block NewBlock){
    FILE * fptr;
    FILE * temp;
    fptr = fopen(FileName, "rb");
    temp = fopen("temp.bin","wb");
    
    struct Block_Header Header;

	char headHash[7] = "0";
    struct Transaction * transactions;

    unsigned int BytesRead = 0;
    int BlockNumber = 0;
    if(fptr != NULL){
        while(1){
            BytesRead = fread(&Header, 1, 25, fptr);
            if(BytesRead != 25){
                break;
            }
            fread(headHash, 1, 7, fptr);
            transactions = malloc(sizeof(struct Transaction)*Header.blockSize);
            fread(transactions, sizeof(struct Transaction), Header.blockSize, fptr);
            fwrite(&Header, 25, 1, temp);
            fwrite(headHash, 1, 7, temp);
            printf("CUM\n");
            printTransactions(transactions, Header.blockSize);
            fwrite(transactions, sizeof(struct Transaction), Header.blockSize, temp);
            free(transactions);
            BlockNumber++;
        }
        fclose(fptr);
    }
    SetStrings(NewBlock.Header.previousHash, headHash, 7);
    NewBlock.Header.blockNum = BlockNumber;
    SetStrings(NewBlock.headHash, HashHead(NewBlock.Header), 7);
    fwrite(&NewBlock.Header, 25, 1, temp);
    fwrite(NewBlock.headHash, 1, 7, temp);
    printf("CUM\n");
    printTransactions(NewBlock.transactions, NewBlock.Header.blockSize);
    fwrite(NewBlock.transactions, sizeof(struct Transaction), NewBlock.Header.blockSize, temp);
    fclose(temp);
    remove(FileName);
    rename("temp.bin",FileName);
}

/* getPeep(struct Transaction * transactions, int number_transactions)
 * reconstructs the peep based on the list of transactions provided.
 * Parameters:
 *    transactions - list of transactions needed to reform the peep
 *    number_transactions - the number of elements in transactions
 * Pre-Condtions:
 *    all transactions are valid. number_transactions is the exact number of elements in transactions.
 * Returns:
 *    the list of characters representing the peep.*/
char * getPeep(struct Transaction * transactions, int number_transactions){
    char * peep;
    peep = malloc(256);
    int i = 0, peep_size = 0;
    for(i = 0; i<number_transactions; i++){
        peep_size = GetPeepSize(peep);
        if(transactions[i].modification.event_type == 0){
            char character = transactions[i].modification.character;
            Insert(peep, peep_size, transactions[i].modification.position, &character);
            
        }else{
            Remove(peep, peep_size, transactions[i].modification.position);
        }
    }
    return peep;
}

/* getPeepAtTime(struct Transaction * transactions, int number_transactions, uint64_t date)
 * reconstrutcs the peep as it was at a certain time.
 * Parameters:
 *    transactions - the list of transactions
 *    number_transaction - the number of elements in transactions
 *    date - the time you want to build the peep to.*/
char * getPeepAtTime(struct Transaction * transactions, int number_transactions, uint64_t date){
    char * peep;
    peep = malloc(256);
    int i = 0, peep_size = 0;
    for(i = 0; i<number_transactions;i++){
        if(transactions[i].timestamp > date){
            break;
        }else{
            peep_size = GetPeepSize(peep);
            if(transactions[i].modification.event_type == 0){
                char character = (char)transactions[i].modification.character;
                Insert(peep, peep_size, transactions[i].modification.position, &character);
            }else{
                Remove(peep, peep_size, transactions[i].modification.position);
            }
        }
    }
    return peep;
}



int main(void){
    struct Transaction * trans;
    int num;
    trans = readBlockChain("nonexistant.bin", &num);
    return;
    struct Transaction t1;
    struct Transaction t2;
    t1.timestamp = GetTimeStamp();
    t2.timestamp = GetTimeStamp();
    struct Modification m1, m2;
    m1.position = 0;
    m1.character = 'a';
    m1.event_type = 0;
    m2.position = 0;
    m2.character = 'b';
    m2.event_type = 0;
    t1.modification = m1;
    t2.modification = m2;
    struct Transaction * transactions;
    transactions = malloc(sizeof(struct Transaction)*2);
    transactions[0] = t1;
    transactions[1] = t2;
    struct Block_Header Header;
    struct block b;
    Header.previousHash[0] = '0';
    Header.timeStamp = GetTimeStamp();
    Header.blockNum = 0;
    Header.blockSize = 2;
    SetStrings(Header.blockHash, HashTransactions(transactions,2),7);
    b.transactions = transactions;
    b.Header = Header;
    WriteBlockChain("test.bin",b);

    struct Transaction * cookie;
    int size = 0;
    cookie = readBlockChain("test.bin",&size);
    int sanS = 0;
    sanS = verifyBlockChain("test.bin");
    char * SaNs;
    int SaNS = 0;
    SaNs = getPeep(cookie, size);
    printf("R%d", cookie[0].timestamp);
    printf("MUC\n");
    printTransactions(cookie, size);
    SaNS = GetPeepSize(SaNs);
    printPeep(SaNs,SaNS);
}