#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "peepEditor.h"
#include "blockChain.h"

int main(int argc, char *argv[]){
    int opt;

    char *peep = malloc(256);
    struct Transaction *modBuffer;
    int isTime = 0, isPeep = 0;
    char *filepeep;
    uint64_t *time_sec;
    
    
    while((opt = getopt(argc, argv, "cevpt"))!= -1){
        switch(opt){
            case 'c':
                int num;
                num = editor(peep, modBuffer);
                addTransaction(modBuffer, num);
                /*need file name to write too*/
                break;
            case 'e':
                char *filename;
                int * num_trans;
                struct Transaction *list_trans;
                char *new_peep;
                int nums;
                filename = optarg;
                list_trans = readBlockChain(filename, num_trans);
                new_peep = getPeep(list_trans, *num_trans);
                addTransaction(modBuffer, nums);
                nums = editor(peep, modBuffer);
                /*need file name to write too*/
                break;
            case 'v':
                char *fileint;
                fileint = optarg;
                int check;
                check = verifyBlockChain(fileint);
                if(check == 1)
                    printf("Integrity test passed");
                else
                    printf("Integrity test failed");
                break;
            case 'p':
                filepeep = optarg;
                isPeep = 1;
                /**/
            case 't':
                time_sec = optarg;
                isTime = 1;
                /**/
            case '?':
                printf("Invalid option");
                break;
        }
    }
    if (isPeep == 1 && isTime == 0){
        int *num_transP;
        struct Transaction *list_transP;
        list_transP = readBlockChain(filepeep, num_transP);
        char *peepP;
        peepP = getPeep(list_transP, *num_transP);
        int size_peep = GetPeepSize(peepP);
        size_peep = GetPeepSize(peepP);
        printPeep(peepP, size_peep);
    }
    if (isPeep == 1 && isTime == 1){
        int *num_transT;
        struct Transaction *list_transT;
        list_transT = readBlockChain(filepeep, num_transT);
        char *peepT = getPeepAtTime(list_transT, num_transT, *time_sec);
        peepT = getPeepAtTime(list_transT, num_transT, *time_sec);
        int size_peepT;
        size_peepT = GetPeepSize(peepT);
        printPeep(peepT, size_peepT);
    }
}
