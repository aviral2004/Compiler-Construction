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

int main()
{
    printf("Status of work:\n");
    printf("FIRST and FOLLOW Automated\nBoth lexical and syntax analysis module implemented\n");
    // Initialize the states, transitions and lookup table
    printf("Initializing the states, transitions and lookup table\n");
    setupStates();
    setupTransitions();
    setupLookupTable();
    printf("Initialization done\n");

    // Initialize the grammar
    printf("Initializing the grammar\n");
    grammar G;
    init(&G);
    printf("Initialization done\n");

    // Compute first and follow sets
    printf("Computing first and follow sets\n");
    FirstAndFollow F = ComputeFirstAndFollowSets(G);
    printf("First and follow sets computed\n");

    // Create the parse table
    printf("Creating the parse table\n");
    table T;
    createParseTable(F, &T);
    printf("Parse table created\n");

    int choice;
    while (1)
    {
        printf("\n\n0. For Exit\n");
        printf("1. For Removal of Comments\n");
        printf("2. For Printing Lexical Tokens List\n");
        printf("3. For Parsing the Source Code and printing the Parse Tree\n");
        printf("4. For Parsing the Source Code and printing total time taken\n");
        printf("ENTER YOUR CHOICE: ");
        scanf("%d", &choice); // Take the choice from the user
        switch (choice)
        {
        case 0:
            // Exit the program
            printf("Thank you for using SALAD compiler.\n");
            return 0;
        case 1:
            // Remove comments from the input file
            printf("Enter the input file name: ");
            char file_name[100];
            scanf("%s", file_name);
            bool flag = append_endline(file_name);
            if (!flag)
                break;
            printf("Enter the output file name:");
            char output_file_name[100];
            scanf("%s", output_file_name);
            printf("Removing comments\n");
            removeComments(file_name, output_file_name);
            printf("Comments removed successfully\n");
            // open the output file in read mode and print the content
            FILE *fc = fopen(output_file_name, "r");
            char c;
            while ((c = fgetc(fc)) != EOF)
                printf("%c", c);
            printf("\n");
            break;
        case 2:
            // Print the lexical tokens list
            printf("Enter the input file name: ");
            char file_name2[100];
            scanf("%s", file_name2);
            bool flag2 = append_endline(file_name2);
            if (!flag2)
                break;
            FILE *fp = fopen(file_name2, "r");
            printf("Calculating the lexical tokens list\n");
            vector v = getStream(fp);
            printf("Lexical tokens list calculated\n");
            printf("%-30s", "Line");
            printf("%-30s", "Lexeme");
            printf("%s\n", "Token");
            for (int i = 0; i < v->size; i++)
            {
                printf("%-30d", get(v, i)->lc);
                printf("%-30s", get(v, i)->lexeme);
                printf("%s\n", TOKENS[get(v, i)->tk]);
            }
            break;
        case 3:
            // Parse the source code and print the parse tree
            printf("Enter the input file name: ");
            char file_name3[100];
            scanf("%s", file_name3);
            bool flag4 = append_endline(file_name3);
            if (!flag4)
                break;
            FILE *fp3 = fopen(file_name3, "r");
            printf("Enter the output file name: ");
            char output_file_name3[100];
            scanf("%s", output_file_name3);
            FILE *f = fopen(output_file_name3, "w");
            printf("Calculating the lexical tokens list\n");
            vector v3 = getStream(fp3);
            printf("Lexical tokens list calculated\n");

            printf("Parsing the source code\n");
                parseTree *tree = parseInputSourceCode(T, F, &G, v3);
            fprintf(f, "%-30s", "Lexeme");
            fprintf(f, "%-30s", "Line Number");
            fprintf(f, "%-30s", "Token Name");
            fprintf(f, "%-30s", "Value");
            fprintf(f, "%-30s", "Parent Node Symbol");
            fprintf(f, "%-30s", "is Leaf?");
            fprintf(f, "%s\n", "Node Symbol");
            fprintf(f, "%-30s", "------");
            fprintf(f, "%-30s", "-----------");
            fprintf(f, "%-30s", "----------");
            fprintf(f, "%-30s", "-----");
            fprintf(f, "%-30s", "------------------");
            fprintf(f, "%-30s", "--------");
            fprintf(f, "%s\n", "-----------");
            printParseTree(tree, f);
            printf("Parse tree printed in the file\n");
            break;
        case 4:
            // Parse the source code and print the total time taken
            printf("Enter the input file name: ");
            char file_name4[100];
            scanf("%s", file_name4);
            FILE *fp4 = fopen(file_name4, "r");
            bool flag5 = append_endline(file_name4);
            if (!flag5)
                break;
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            printf("Calculating the lexical tokens list\n");
            vector v4 = getStream(fp4);
            printf("Lexical tokens list calculated\n");
            printf("Parsing the source code\n");
            parseInputSourceCode(T, F, &G, v4);
            printf("Parse tree calculated\n");
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU time: %f\n", total_CPU_time);
            printf("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);
            break;
        default:
            // Invalid choice
            printf("Invalid Choice\n");
            break;
        }
    }
    return 0;
}
