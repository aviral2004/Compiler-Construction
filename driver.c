/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/
#include "lexerDef.h"
#include "parserDef.h"
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include <time.h>

void printStatus() {
    printf("\n=== Implementation Status ===\n");
    printf("✓ FIRST and FOLLOW sets automated\n");
    printf("✓ Both lexical and syntax analysis modules implemented\n");
    printf("✓ All modules compile successfully\n");
    printf("✓ Parse tree construction working\n");
    printf("✓ Working with all test cases\n");
    printf("==========================\n\n");
}

void printUsage(char* programName) {
    printf("Usage: %s <input_source_file> <parse_tree_output_file>\n", programName);
    printf("Example: %s testcase.txt parsetree.txt\n", programName);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    char* input_file = argv[1];
    char* parse_tree_file = argv[2];

    // Print implementation status
    printStatus();

    // Initialize the states, transitions and lookup table
    printf("Initializing the states, transitions and lookup table...\n");
    setupStates();
    setupTransitions();
    setupLookupTable();
    printf("Initialization done\n");

    // Initialize the grammar
    printf("Initializing the grammar...\n");
    grammar G;
    init(&G);
    printf("Initialization done\n");

    // Compute first and follow sets
    printf("Computing first and follow sets...\n");
    FirstAndFollow F = ComputeFirstAndFollowSets(G);
    printf("First and follow sets computed\n");

    // Create the parse table
    printf("Creating the parse table...\n");
    table T;
    createParseTable(F, &T);
    printf("Parse table created\n\n");

    int choice;
    while (1)
    {
        printf("\n=== Compiler Construction Project: Group 17 ===\n");
        printf("0. Exit\n");
        printf("1. Remove comments and display comment-free code\n");
        printf("2. Display token list from lexical analysis\n");
        printf("3. Parse source code and generate parse tree\n");
        printf("4. Display time taken for lexical and syntax analysis\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
        case 0:
            printf("Exiting program.\n");
            return 0;

        case 1:
            {
                bool flag = append_endline(input_file);
                if (!flag) break;
                
                printf("=== Comment-free source code ===\n\n");
                // Create a temporary file for processing
                char temp_file[] = "temp_comment_free.txt";
                removeComments(input_file, temp_file);
                
                // Read and display the comment-free code
                FILE *fc = fopen(temp_file, "r");
                char c;
                while ((c = fgetc(fc)) != EOF)
                    printf("%c", c);
                fclose(fc);
                remove(temp_file); // Clean up temporary file
                printf("\n=== End of comment-free code ===\n");
            }
            break;

        case 2:
            {
                bool flag = append_endline(input_file);
                if (!flag) break;
                
                FILE *fp = fopen(input_file, "r");
                printf("Performing lexical analysis...\n\n");
                vector v = getStream(fp);
                
                // Print token list with proper formatting
                printf("%-8s | %-20s | %-30s\n", "Line", "Token", "Lexeme");
                printf("-------------------------------------------------------\n");
                for (int i = 0; i < v->size; i++) {
                    printf("%-8d | %-20s | %-30s\n", 
                        get(v, i)->lc,
                        TOKENS[get(v, i)->tk],
                        get(v, i)->lexeme);
                }
                fclose(fp);
            }
            break;

        case 3:
            {
                bool flag = append_endline(input_file);
                if (!flag) break;
                
                FILE *fp = fopen(input_file, "r");
                FILE *f = fopen(parse_tree_file, "w");
                
                printf("Performing lexical and syntax analysis...\n");
                vector v = getStream(fp);
                
                parseTree *tree = parseInputSourceCode(T, F, &G, v);
                
                // Write parse tree to file
                fprintf(f, "%-30s%-20s%-30s%-30s%-30s%-20s%-30s\n",
                    "Lexeme", "Line Number", "Token Name", "Value",
                    "Parent Node Symbol", "is Leaf?", "Node Symbol");
                fprintf(f, "%s\n", "--------------------------------------------------------------------------------------------------------");
                printParseTree(tree, f);
                
                printf("\nParse tree has been written to: %s\n", parse_tree_file);
                fclose(fp);
                fclose(f);
            }
            break;

        case 4:
            {
                bool flag = append_endline(input_file);
                if (!flag) break;
                
                FILE *fp = fopen(input_file, "r");
                clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                
                printf("Measuring performance...\n\n");
                start_time = clock();
                
                // Perform lexical and syntax analysis
                vector v = getStream(fp);
                parseInputSourceCode(T, F, &G, v);
                
                end_time = clock();
                total_CPU_time = (double)(end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
                
                printf("Performance Results:\n");
                printf("Total CPU time (ticks): %.2f\n", total_CPU_time);
                printf("Total CPU time (seconds): %.4f\n", total_CPU_time_in_seconds);
                
                fclose(fp);
            }
            break;

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
    return 0;
}
