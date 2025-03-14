# Group Number - 17
# ID:	2022A7PS0173P			Name: Ameesh Sethi
# ID:	2022A7PS0097P			Name: Aviral Gupta
# ID:	2022A7PS0146P			Name: Yatharth Singh
# ID:	2022A7PS1188P			Name: Armaan Sethi
# ID:	2022A7PS0052P			Name: Utkarsh Tiwari


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
