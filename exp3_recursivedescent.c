#include<stdio.h>
#include<string.h>
#include<ctype.h>
char input[20];
int err=0,i=0;
void E();
void EPRIME();
void T();
void TPRIME();
void F();
void FPRIME();
void E()
{
    T();
    EPRIME();
}
void EPRIME()
{
    if(input[i]=='+' || input[i]=='-')
    {
     i++;
     T();
     EPRIME();
    }
}
void T()
{
    F();
    TPRIME();
}
void F()
{
    if(input[i]=='(')
    {
     i++;
     E();
     if(input[i]==')')
     {
         i++;
     }
     else
     {
         err=1;
         printf("Missing Parantensis:");
     }
    }
    else if(isalnum(input[i]))
    {
      i++;
    }
    else
    {
        err=1;
        printf("Missing Operand");
    }
}
void TPRIME()
{
    if(input[i]=='*'||input[i]=='/')
    {
        i++;
        F();
        TPRIME();
    }
}
void main()
{
    printf("enter the expression:");
    scanf("%s",input);
    E();
    int len=strlen(input);
    if(i==len&&err==0)
    {
        printf("ACCEPTED");
    }
    else
    {
        printf("NOT ACCEPTED");
    }

}
