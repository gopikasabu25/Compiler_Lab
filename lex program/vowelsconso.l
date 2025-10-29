%{
#include<stdio.h>
#include<string.h>
FILE *yyin;
int vowels = 0;
int conso  = 0;
int lines  = 0;
%}

%%
[\t]+              ;
[aeiouAEIOU]       { vowels++; }
[A-Za-z]           {
                      if (!strchr("aeiouAEIOU", yytext[0]))
                          conso++;
                    }
\n                 { lines++; }
.                  ;
%%

int main() {
    yyin = fopen("parag.txt", "r");
    if (!yyin) {
        printf("Error: Cannot open file.\n");
        return 1;
    }
    yylex();
    fclose(yyin);

    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", conso);
    printf("Lines: %d\n", lines);

    return 0;
}

int yywrap() {
    return 1;
}
