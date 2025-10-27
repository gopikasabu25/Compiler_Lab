#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 2000

char lexemes[MAX][50];
char tokens[MAX][20];
int lines[MAX];
int entryCount = 0;

void loadTable() {
    FILE *fp = fopen("sym_tab.txt", "r");
    if (!fp) {
        printf("Could not open sym_tab.txt\n");
        exit(1);
    }

    char line[100];
    fgets(line, sizeof(line), fp); // Skip header line
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s %d %s", lexemes[entryCount], &lines[entryCount], tokens[entryCount]);
        entryCount++;
    }
    fclose(fp);
}

int findInTable(char *word, const char *type) {
    for (int i = 0; i < entryCount; i++) {
        if (strcmp(lexemes[i], word) == 0 && strcmp(tokens[i], type) == 0)
            return lines[i];
    }
    return -1;
}

void appendToTable(char *word, const char *type) {
    if (findInTable(word, type) == -1) {
        int newLine = entryCount == 0 ? 1 : entryCount;
        FILE *fp = fopen("sym_tab.txt", "a");
        fprintf(fp, "%s\t\t%d\t%s\n", word, newLine, type);
        fclose(fp);

        strcpy(lexemes[entryCount], word);
        strcpy(tokens[entryCount], type);
        lines[entryCount] = newLine;
        entryCount++;
    }
}

int isKeyword(char *word) {
    char *keywords[] = {"int", "char", "float", "double", "return", "if", "else", "for", "while", "void", "main"};
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main() {
    FILE *fp = fopen("inp.txt", "r");
    if (!fp) {
        printf("Cannot open inp.txt\n");
        return 1;
    }

    loadTable();

    char ch;
    char buffer[50];
    int line = 1, j = 0;

    printf("Lexeme\t\tLine\tToken\n");

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\"') {  // Skip string literals
            while ((ch = fgetc(fp)) != EOF && ch != '\"');
            continue;
        }
        if (ch == '#') {  // Skip preprocessor directives
            while ((ch = fgetc(fp)) != EOF && ch != '\n');
            line++;
            continue;
        }
        // Handle identifiers and numbers
        if (isalnum(ch) || ch == '_') {
            buffer[j++] = ch;

            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_')) {
                buffer[j++] = ch;
            }

            buffer[j] = '\0';
            j = 0;

            if (isKeyword(buffer)) {
                printf("%s\t\t%d\tKeyword\n", buffer, line);
            }
            else if (isdigit(buffer[0])) {
                appendToTable(buffer, "number");
                int ln = findInTable(buffer, "number");
                printf("%s\t\t%d\tNumber, %d\n", buffer, line, ln);
            }
            else {
                appendToTable(buffer, "identifier");
                int ln = findInTable(buffer, "identifier");
                printf("%s\t\t%d\tIdentifier, %d\n", buffer, line, ln);
            }
        }

        // Handle arithmetic operators
        if (ch == '+')
            printf("%c\t\t%d\t Arithmetic Operator Plus Operator\n", ch, line);
        else if (ch == '-')
            printf("%c\t\t%d\t Arithmetic Operator Minus Operator\n", ch, line);
        else if (ch == '*')
            printf("%c\t\t%d\t Arithmetic Operator Multiplication Operator\n", ch, line);
        else if (ch == '/')
            printf("%c\t\t%d\t Arithmetic Operator Division Operator\n", ch, line);
        else if (ch == '%')
            printf("%c\t\t%d\t Arithmetic Operator Modulo Operator\n", ch, line);

        // Handle relational and equality operators
        else if (ch == '<' || ch == '>' || ch == '=' || ch == '!') {
            char next = fgetc(fp);
            if (ch == '<' && next == '=')
                printf("<=\t\t%d\t RElational Operator Less Than or Equal To\n", line);
            else if (ch == '>' && next == '=')
                printf(">=\t\t%d\t RElational Operator Greater Than or Equal To\n", line);
            else if (ch == '=' && next == '=')
                printf("==\t\t%d\tEqual To\n", line);
            else if (ch == '!' && next == '=')
                printf("!=\t\t%d\tNot Equal To\n", line);
            else {
                if (ch == '<')
                    printf("%c\t\t%d\t RElational Operator Less Than\n", ch, line);
                else if (ch == '>')
                    printf("%c\t\t%d\t RElational Operator Greater Than\n", ch, line);
                else if (ch == '=')
                    printf("%c\t\t%d\tAssignment Operator\n", ch, line);
                else if (ch == '!')
                    printf("%c\t\t%d\tLogical NOT\n", ch, line);
                if (next != EOF) ungetc(next, fp);
            }
        }

        // Handle punctuation
        else if (ch == ';')
            printf("%c\t\t%d\tSemicolon\n", ch, line);
        else if (ch == ',')
            printf("%c\t\t%d\tComma\n", ch, line);
        else if (ch == '(')
            printf("%c\t\t%d\tLeft Parenthesis\n", ch, line);
        else if (ch == ')')
            printf("%c\t\t%d\tRight Parenthesis\n", ch, line);
        else if (ch == '{')
            printf("%c\t\t%d\tLeft Brace\n", ch, line);
        else if (ch == '}')
            printf("%c\t\t%d\tRight Brace\n", ch, line);
        // Handle newline
        else if (ch == '\n')
            line++;
    }

    fclose(fp);
    return 0;
}
