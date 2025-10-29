%{
#include<string.h>
#include<stdio.h>

char str[100];
int i=0;
int j;
%}
%%
[^\n] {str[i++]=yytext[0];}
\n {str[i]='\0'; return 0;}
%%
int main()
{
printf("enter the string:");
yylex();
int len=strlen(str);
int k=len-1;
int flag=0;
for(j=0;j<len;j++)
{
if(str[j]!=str[k])
{
 flag=1;
 break;
 }
 k--;
 }
 if(flag==0)
 {
 printf("palindrome:");
 }
 else
 {
 printf("not palindrome");
 }
 return 0;
 }
 int yywrap() {
    return 1;
}