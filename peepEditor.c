#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "peepEditor.h"
#include "blockChain.h"

#define PEEPLEN 256


/* GetTimeStamp(void)
 * returns the timestamp at the present time
 * in the form of a uint64_t.
*/
uint64_t GetTimeStamp(void){
    time_t stamp;
    stamp = time(NULL);
    stamp = stamp - (40*365*24*60*60);/* subtract the 40 years so get the time ellasped from jan 1 2020*/
    return (uint64_t)stamp;
}

/* GetPeepSize(char *peep)
 * returns the size of the peep.
 * peep - the array of the characters representing the peep.*/
int GetPeepSize(char * peep){
    int i = 0;
    while (1==1){
        if(peep[i] != '\0'){
            i++;
        }else{
            break;
        }
    }
    return i;
}

/* printPeep(char *peep, int peep_size)
 * displays peep as a list of characters
 * Parameters:
 *    peep - pointer to the array of characters representing peep
 *    peep_size - the size of the peep.
 * Pre-Conditions:
 *    peep_size is the exact size of the peep.
 * Post-Conditions:
 *    peep is displayed as a list of characters*/
void printPeep(char * peep, int peep_size)
{
    int i = 0;
    for(i = 0; i<peep_size; i++)
    {
        printf("%c",peep[i]);
    }
    printf("\n");
}

/* Insert(char* peep, int peep_size, int position, char* Character)
 * Inserts character into the peep at the specified location
 * Parameters:
 *    peep - the array of characters representing the peep.
 *    peep_size - the number of elements in peep.
 *    position - the position to insert the character (must be within the range of peep)
 *    Character - pointer to the character to insert.
 * return:
 *     - if the function was not able to complete, IE position was not valid, Then
 *      a Transaction with a timestamp of 0 is returned
 *    - else a transaction with the proper timestamp is returned.
 * Pre-Conditions:
 *     peep_size - the number of elements in the peep array
 * Post-Conditions:
 *     character is inserted at position in the peep. */
struct Transaction Insert(char* peep, int peep_size, int position, char Character){
    int ichar;
    struct Transaction returnValue;
    returnValue.timestamp = 0;
    if(position > peep_size)
    {
        printf("Indicated Position Exceeds the size of peep\n");
        return returnValue;
    }
    if(position < 0)
    {
        printf("Indicated Position must be a positive integer\n");
        return returnValue;
    }
    ichar = (int)Character;
    if(ichar<32 || ichar > 127)
    {
        printf("Invalid Character\n");
        return returnValue;
    }
    if(peep_size >= PEEPLEN)
    {
        printf("You cannot insert into a full peep\n");
        return returnValue;
    }

    int i = 0;
    char previousChar, toInsert = Character;
    for(i = position; i<peep_size+1; i++){
        previousChar = peep[i];
        peep[i] = toInsert;
        toInsert = previousChar;
    }

    struct Modification mod;
    mod.position = position;
    mod.character = Character;
    mod.event_type = 0;
    returnValue.modification = mod;
    uint64_t timestamp = GetTimeStamp();
    returnValue.timestamp = timestamp;
    return returnValue;
}

/* Remove(char * peep, int peep_size, int position)
 * removes the character at the given position.
 * returns:
 *    - if the function was not able to complete, IE position was not valid, Then
 *      a Transaction with a timestamp of 0 is returned
 *    - else a transaction with the proper timestamp is returned.
 * Parameters:
 *    peep - the character array that represents peep.
 *    peep_size - the number of elements in the peep array
 *    position - the position of the element ot remove
 * Pre-Conditions:
 *    peep_size is the size of peep no more no less
 * Post-Conditions:
 *    character at position position is removed from peep
 *
*/
struct Transaction Remove(char* peep, int peep_size, int position){
    struct Transaction returnValue;
    returnValue.timestamp = 0;
    if(position >= peep_size){
        printf("Indicated Position Exceeds the size of peep\n");
        return returnValue;
    }
    if(position < 0){
        printf("Indicated Position must be a positive integer\n");
        return returnValue;
    }

    /* Remove the character from position in the peep */
    int i = 0;
    for(i = position; i<peep_size; i++){
        if(i == peep_size-1){
            peep[i] = '\0';
        }else{
            peep[i] = peep[i+1];
        }
    }

    /*Create the transaction*/
    struct Modification mod;
    mod.position = position;
    mod.character = 0; /*character type does not matter in this transaction*/
    mod.event_type = 1;
    returnValue.modification = mod;
    uint64_t timestamp = GetTimeStamp();
    returnValue.timestamp = timestamp;
    return returnValue;
}

/* unsigned int editor(char *peep, struct Transaction *modBuffer)
 * gives the user a menu in order to edit the peep
 * returns:
 *    - number of transactions in the session
 * Parameters:
 *    peep - the character array that represents peep.
 *    modBuffer - an array of struct transactions
 * Pre-Conditions:
 *    modBuffer is empty
 * Post-Conditions:
 *    modBuffer contains transactions if applicable
 *
*/

unsigned int editor(char *peep, struct Transaction *modBuffer)
{
    int menu = 0, position, peep_size, num_trans = 0;
    char Chara, *Character;
    struct Transaction transa;
    char c;
    peep_size = GetPeepSize(peep);
    while (menu != 4)
    {
        printf("PEEP EDITOR MENU\nChoose an Option\n1. View Characters\n2. Insert\n3. Delete\n4. Save and Exit");
        scanf("%d", &menu);
        if(menu == 1)
        {
            printPeep(peep, peep_size);
            menu = 0;
        }else if(menu == 2) {
            printf("Postion to insert: ");
            scanf("%d", &position);
            
            while((c=getchar()) != '\n' && c != EOF){}
            printf("Character to insert: ");
            scanf("%c", &Chara);
            Character = &Chara;
            transa = Insert(peep, peep_size, position, Chara);
            if (transa.timestamp != 0)
            {
                peep_size += 1;
                modBuffer[num_trans] = transa;
                num_trans += 1;
            }
            menu = 0;
        }else if(menu == 3){
            printf("Position to delete: ");
            scanf("%d", &position);
            position -= 1;
            transa = Remove(peep, peep_size, position);
            if (transa.timestamp != 0)
            {
                peep_size -= 1;
                modBuffer[num_trans] = transa;
                num_trans += 1;
            }
            menu = 0;
        }else if(menu == 4){
            return num_trans;
        }else{
            c = getchar();
            while(c == '\n'){
                c = getchar();
            }
        }
    }
}
