# Peep_Block_Chain
simulates the mechanics of a simple block chain Architecture in which users can edit a string of 256 characters, the peep.
## Description
the peep is a string of 256 characters that can be edited by the user by using the peepEditor (see usage section).\
each time the user edits a character a transaction is created. If enough transactions are made or the user exits the editor, a block of transactions is made.\
This block is then added to the block chain by hasing the contents of the block and the header of the previous block. \
The user can then verify the block chain by running the verification program.
## Usage
run the command 'make all' to compile all needed executables.\
to edit the peep run the peep executable with the specified flags:\
-c to create the block chain.\
-e to access the peep editor.\
-v to verify the integrity of the block chain.\
-p to get the current peep.\
-t [timeInSeconds] to get the peep at a specific point in time.
