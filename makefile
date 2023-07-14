#------------------------------------------------------
# Student's Name: Nathan Koop, Keyanna Levie
# Assignment #3
# Lab Section: X03L, Winter 2021
# CMPUT 201, Winter 2021
# Lab Instructor's Name: Antal Buss
#------------------------------------------------------

CC = gcc
CFLAGS = -Wall -ansi -std=c99 -lcrypto
all: peep

peep: peepEditor.o blockChain.o
	$(CC) $(CFLAGS) $^ -o $@
	chmod -cf 777 ./$@

peepEditor.o: peepEditor.c peepEditor.h blockChain.h
	$(CC) -c $<

blockChain.o: blockChain.c blockChain.h peepEditor.h
	$(CC) -c $<

clean:
	rm *.o

A3.tar.gz:
	tar -czvf A3.tar.gz blockChain.c blockChain.h peep.c peepEditor.c peepEditor.h
	chmod -cf 666 A3.tar.gz
