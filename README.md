A compiler for the C- minus language described in *Compiler Construction Principles And Practice*, by Kenneth Louden.

Building and Running the cm compiler
-------------------------------------
  Type the following to perform the desired action.

  To build: 
  * 'make' or 'make cm'

  To run: 
  * './cm **flags** **sourceFile**'

  To clean build files and executables:
  * 'make clean'

Compiler Flags
--------------
* -a prints a textual representation of the AST to STDOUT.
* -s prints the symbol table to STDOUT.
* -c prints the resulting ASM to STDOUT.

.cm files:
----------
Test files can be found in the tests subdirectory.