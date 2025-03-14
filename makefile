# Group Number - 10
# ID:	2021A7PS1463P			Name: Dhyey Italiya
# ID:	2021A7PS2434P			Name: LAKSHIT SETHI
# ID:	2021A7PS0523P			Name: Abir Abhyankar 
# ID:	2021A7PS2414P			Name: Saksham Verma
# ID:	2021A7PS2412P			Name: Saurabh Bhandari


# Purpose: Makefile for the project

# Lexical Analyzer
lexer.o: lexer.c lexerDef.h
		@gcc -c lexer.c

# Syntax Analyzer
parser.o: parser.c lexerDef.h parserDef.h
		@gcc -c parser.c 

# Driver Code
driver.o: driver.c lexerDef.h parserDef.h utils.h lexer.h parser.h
		@gcc -c driver.c

# Utilities
utils.o: utils.c utils.h
		@gcc -c utils.c

# Trie data structure
trie.o: trie.c trie.h
		@gcc -c trie.c

# Vector data structure
vector.o: vector.c vector.h
		@gcc -c vector.c

# Compile all the .o files
compile: driver.o lexer.o parser.o utils.o trie.o vector.o
		@gcc -o exec driver.o lexer.o parser.o utils.o trie.o vector.o
		@rm *.o

# Run the executable
run: exec
		@./exec 

# Clean the .o files and the executable
clean:
		@rm -f *.o exec
		@rm -f *.o 

make rand:
	make clean && make compile && make run

# Compile, run and clean
all: compile run clean
