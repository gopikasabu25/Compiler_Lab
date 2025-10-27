%{
#include<stdio.h>
int words=0,lines=0,charc=0;
FILE *yyin;
%}
%%
[\t]+;
\n {lines++;charc++;}
[A-Za-z0-9]+ {words++;charc+=yyleng;}
. {charc++;}
%%

int main()
{
yyin=fopen("text.txt","r");
yylex();
fclose(yyin);
printf("no of lines: %d\n",lines);
printf("no of words:%d\n",words);
printf("no of chars:%d\n",charc);
return 0;
}
int yywrap()
{
return 1;
}