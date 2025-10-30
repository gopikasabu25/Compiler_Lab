%{
#include<stdio.h>
#include<string.h>
#include<ctype.h>
char str[100];
int i=0;
%}
%%
[A-Za-z] {str[i++]=tolower(yytext[0]);}
\n {str[i]='\0';return 0;}
.  { }
%%
int main()
{
printf("enter string");
yylex();
printf("%s",str);
return 0;
}
int yywrap()
{
return 1;
}