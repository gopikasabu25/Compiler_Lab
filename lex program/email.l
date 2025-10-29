%{
#include <stdio.h>
int valid=0;
%}

%%
[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4} {
        valid=1;
        
}
.*   { }

\n    {return 0; }  

%%

int main() {
    printf("Enter text:\n");
    yylex();
    if(valid==1)
    printf("valid");
    else
    printf("not valid");
    return 0;
}

int yywrap() {
    return 1;
}
