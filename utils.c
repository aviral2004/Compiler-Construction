/*
Group Number - 17
ID:	2022A7PS0173P			Name: Ameesh Sethi
ID:	2022A7PS0097P			Name: Aviral Gupta
ID:	2022A7PS0146P			Name: Yatharth Singh
ID:	2022A7PS1188P			Name: Armaan Sethi
ID:	2022A7PS0052P			Name: Utkarsh Tiwari
*/

#include "utils.h"


bool append_endline(char *file_name)
{
    // open the file in read mode, if the last character is not a newline character, open the file in append mode and append a newline character
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        cyan("File not found\n");
        return 0;
    }
    fseek(fp, -1, SEEK_END);
    char ch;
    fread(&ch, 1, 1, fp);
    if (ch != '\n')
    {
        fclose(fp);
        fp = fopen(file_name, "a");
        fprintf(fp, "\n");
        fclose(fp);
    }
    else
    {
        fclose(fp);
    }
    return 1;
}