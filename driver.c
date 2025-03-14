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

    // Initialize the states, transitions and lookup table
    white("Initializing the states, transitions and lookup table\n");
    initialize_states();
    initialize_transitions();
    initialize_lookup_table();
    green("Initialization done\n");

    // Initialize the grammer
    white("Initializing the grammer\n");
    grammer G;
    grammer_init(&G);
    green("Initialization done\n");

    // Compute first and follow sets
    white("Computing first and follow sets\n");
    FirstAndFollow F = ComputeFirstAndFollowSets(G);
    green("First and follow sets computed\n");

    // Create the parse table
    white("Creating the parse table\n");
    table T;
    createParseTable(F, &T);
    green("Parse table created\n");

    int choice;
    while (1)
    {
        yellow("\n\n0. For Exit\n");
        yellow("1. For Removal of Comments\n");
        yellow("2. For Printing Lexical Tokens List\n");
        yellow("3. For Parsing the Source Code and printing the Parse Tree\n");
        yellow("4. For Parsing the Source Code and printing total time taken\n");
        yellow("ENTER YOUR CHOICE: ");
        scanf("%d", &choice); // Take the choice from the user
        switch (choice)
        {
        case 0:
            // Exit the program
            cyan("Thank you for using SALAD compiler.\n");
            return 0;
        case 1:
            // Remove comments from the input file
            yellow("Enter the input file name: ");
            char file_name[100];
            scanf("%s", file_name);
            bool flag = append_endline(file_name);
            if (!flag)
                break;
            yellow("Enter the output file name:") char output_file_name[100];
            scanf("%s", output_file_name);
            white("Removing comments\n");
            removeComments(file_name, output_file_name);
            green("Comments removed successfully\n");
            // open the output file in read mode and print the content
            FILE *fc = fopen(output_file_name, "r");
            char c;
            while ((c = fgetc(fc)) != EOF)
                printf("%c", c);
            printf("\n");
            break;
        case 2:
            // Print the lexical tokens list
            yellow("Enter the input file name: ");
            char file_name2[100];
            scanf("%s", file_name2);
            bool flag2 = append_endline(file_name2);
            if (!flag2)
                break;
            FILE *fp = fopen(file_name2, "r");
            white("Calculating the lexical tokens list\n");
            vector v = getStream(fp);
            green("Lexical tokens list calculated\n");
            red("%-30s", "Line");
            red("%-30s", "Lexeme");
            red("%s\n", "Token");
            for (int i = 0; i < v->size; i++)
            {
                magenta("%-30d", get(v, i)->lc);
                green("%-30s", get(v, i)->lexeme);
                blue("%s\n", TOKENS[get(v, i)->tk]);
            }
            break;
        case 3:
            // Parse the source code and print the parse tree
            yellow("Enter the input file name: ");
            char file_name3[100];
            scanf("%s", file_name3);
            bool flag4 = append_endline(file_name3);
            if (!flag4)
                break;
            FILE *fp3 = fopen(file_name3, "r");
            yellow("Enter the output file name: ");
            char output_file_name3[100];
            scanf("%s", output_file_name3);
            FILE *f = fopen(output_file_name3, "w");
            white("Calculating the lexical tokens list\n");
            vector v3 = getStream(fp3);
            green("Lexical tokens list calculated\n");

            white("Parsing the source code\n")
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
            green("Parse tree printed in the file\n");
            break;
        case 4:
            // Parse the source code and print the total time taken
            yellow("Enter the input file name: ");
            char file_name4[100];
            scanf("%s", file_name4);
            FILE *fp4 = fopen(file_name4, "r");
            bool flag5 = append_endline(file_name4);
            if (!flag5)
                break;
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            white("Calculating the lexical tokens list\n");
            vector v4 = getStream(fp4);
            green("Lexical tokens list calculated\n");
            white("Parsing the source code\n");
            parseInputSourceCode(T, F, &G, v4);
            green("Parse tree calculated\n");
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            magenta("Total CPU time: %f\n", total_CPU_time);
            magenta("Total CPU time in seconds: %f\n", total_CPU_time_in_seconds);
            break;
        default:
            // Invalid choice
            red("Invalid Choice\n");
            break;
        }
    }
    return 0;
}