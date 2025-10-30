%{
#include <stdio.h>
%}

%%
[a-zA-Z0-9]+ {
    int i,flag = 1;
    int j=yyleng-1;
    for(i = 0;i <= j; i++) {
        if(yytext[i] != yytext[j]) {
            flag = 0;
            break;
        }
        j--;
    }
    if(flag)
        printf("Given string is Palindrome\n");
    else
        printf("Given string is not Palindrome\n");
}
\n    ; // Ignore newlines
.     ; // Ignore other characters
%%

int main() {
    printf("Enter a string: (ctrl+D)");
    yylex();
    return 0;
}
int yywrap() {
    return 1;
}
