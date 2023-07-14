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
#ifndef PEEPEDITOR_H
#define PEEPEDITOR_H
/* edits the peep, saves transactions in modBuffer, and returns the number of transactions written in th ebuffer*/
unsigned int editor(char * peep, struct Transaction * modBuffer);
struct Transaction Remove(char* peep, int peep_size, int position);
struct Transaction Insert(char* peep, int peep_size, int position, char *Character);

uint64_t GetTimeStamp(void);



void printPeep(char * peep, int size);
int GetPeepSize(char * peep);
/* the sturcture to represent a modification, this structure is only 2 bytes in memory
 * position: 8 bits to represent the position the modification occured at (up to 256)
 * character: 7 bits to represent the ascii code of the character in the modification (up to 127)
 * event_type: 1 bit to represent the modification type (1 for Remove, 0 for Insert)*/
/*struct Modification{
	unsigned int position : 8;
	unsigned int character : 7;
	unsigned int event_type : 1;
}__attribute__((packed));*/

/* structure to represent a Transaction
 * timestamp: 6 bytes to represent the time the modification occured.
 * modification: the modification that occured at the given time.*/
/*struct Transaction{
	uint64_t timestamp : 48;/*6 bytes*/
//	struct Modification modification;
//}__attribute__((packed));
#endif


