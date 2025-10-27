%{
#include<stdio.h>
int line=1;
FILE *yyin;
%}
liter [0-9][0-9]*
sliter \"[a-zA-z][a-zA-Z0-9]*\"
ident [a-zA-Z][a-zA-Z0-9]*
inva [0-9][a-zA-Z0-9]*
rel >=|>|<=|<
ari [\+\-\*/]
key int|void|char|if|else|case|return|main
punct [\{\}\(\),;]
%%
{liter} {printf("%s\t%d\tLiteral\n",yytext,line);}
{sliter} {char aa[100],bb[100];
	strcpy(aa,yytext);
	int i;
	for(i=1;i<strlen(aa)-1;i++)
	{
	bb[i-1]=aa[i];
	}
	bb[i-1] = '\0';
	printf("%s\t%d\tLiteral\n",bb,line);}

{key} {printf("%s\t%d\tKeyword\n",yytext,line);}
{ident} {printf("%s\t%d\tIdentifier\n",yytext,line);}
{inva} {printf("%s\tInvalid\n",yytext);}
{rel} {printf("%s\t%d\tRelational operator ,",yytext,line);
	if(strcmp(yytext,">=")==0){printf("GE");}
	if(strcmp(yytext,">")==0){printf("GT");}
	if(strcmp(yytext,"<=")==0){printf("LE");}
	if(strcmp(yytext,"<")==0){printf("LT");}
	printf("\n");
	}
	
{ari} {printf("%s\t%d\tARithmetic operator ,",yytext,line);
	if(strcmp(yytext,"+")==0){printf("ADD");}
	if(strcmp(yytext,"-")==0){printf("sub");}
	if(strcmp(yytext,"*")==0){printf("mul");}
	if(strcmp(yytext,"/")==0){printf("div");}
	printf("\n");
	}
"=" {printf("%s\t%d\tAssignment OPerator\n",yytext,line);}
\n {line++;}

{punct}                 { printf("%s\t%d\tPunctuation\n", yytext, line); }

. {}
%%
int main()
{
yyin=fopen("input.txt","r");
printf("\n Lexeme\tToken\tlINE No\n");
yylex();
}
int yywrap()
{
return 1;
}